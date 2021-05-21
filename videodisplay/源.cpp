#include <stdio.h>
#include <assert.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <shader_s.h>
#include <libyuv.h>
#include <thread>
#include <chrono>


typedef  unsigned char BYTE;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
// settings
const unsigned int SCR_WIDTH = 720;
const unsigned int SCR_HEIGHT = 720;

// compatibility with newer API
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55,28,1)
#define av_frame_alloc avcodec_alloc_frame
#define av_frame_free avcodec_free_frame
#endif

#define SDL_AUDIO_BUFFER_SIZE 1024
#define MAX_AUDIO_FRAME_SIZE 192000

struct SwrContext* audio_convert_ctx = NULL;

typedef struct PacketQueue {
    AVPacketList* first_pkt, * last_pkt;
    int nb_packets;
    int size;
    SDL_mutex* mutex;
    SDL_cond* cond;
} PacketQueue;

PacketQueue audioq;

int quit = 0;

void packet_queue_init(PacketQueue* q) {
    memset(q, 0, sizeof(PacketQueue));
    q->mutex = SDL_CreateMutex();
    q->cond = SDL_CreateCond();
}

int packet_queue_put(PacketQueue* q, AVPacket* pkt) {

    AVPacketList* pkt1;
    if (av_dup_packet(pkt) < 0) {
        return -1;
    }
    pkt1 = (AVPacketList*)av_malloc(sizeof(AVPacketList));
    if (!pkt1)
        return -1;
    pkt1->pkt = *pkt;
    pkt1->next = NULL;

    SDL_LockMutex(q->mutex);

    if (!q->last_pkt) {
        q->first_pkt = pkt1;
    }
    else {
        q->last_pkt->next = pkt1;
    }

    q->last_pkt = pkt1;
    q->nb_packets++;
    q->size += pkt1->pkt.size;
    SDL_CondSignal(q->cond);

    SDL_UnlockMutex(q->mutex);
    return 0;
}

int packet_queue_get(PacketQueue* q, AVPacket* pkt, int block)
{
    AVPacketList* pkt1;
    int ret;

    SDL_LockMutex(q->mutex);

    for (;;) {

        if (quit) {
            ret = -1;
            break;
        }

        pkt1 = q->first_pkt;
        if (pkt1) {
            q->first_pkt = pkt1->next;
            if (!q->first_pkt)
                q->last_pkt = NULL;
            q->nb_packets--;
            q->size -= pkt1->pkt.size;
            *pkt = pkt1->pkt;
            av_free(pkt1);
            ret = 1;
            break;
        }
        else if (!block) {
            ret = 0;
            break;
        }
        else {
            SDL_CondWait(q->cond, q->mutex);
        }
    }
    SDL_UnlockMutex(q->mutex);
    return ret;
}

int audio_decode_frame(AVCodecContext* aCodecCtx, uint8_t* audio_buf, int buf_size) {

    static AVPacket pkt;
    static uint8_t* audio_pkt_data = NULL;
    static int audio_pkt_size = 0;
    static AVFrame frame;

    int len1, data_size = 0;

    for (;;) {
        while (audio_pkt_size > 0) {
            int got_frame = 0;
            len1 = avcodec_decode_audio4(aCodecCtx, &frame, &got_frame, &pkt);
            if (len1 < 0) {
                /* if error, skip frame */
                audio_pkt_size = 0;
                break;
            }
            audio_pkt_data += len1;
            audio_pkt_size -= len1;
            data_size = 0;
            if (got_frame) {
                //fprintf(stderr, "channels:%d, nb_samples:%d, sample_fmt:%d \n", aCodecCtx->channels, frame.nb_samples, aCodecCtx->sample_fmt);
                /*
            data_size = av_samples_get_buffer_size(NULL,
                                   aCodecCtx->channels,
                                   frame.nb_samples,
                                   aCodecCtx->sample_fmt,
                                   1);
                */
                data_size = 2 * 2 * frame.nb_samples;

                assert(data_size <= buf_size);
                swr_convert(audio_convert_ctx,
                    &audio_buf,
                    MAX_AUDIO_FRAME_SIZE * 3 / 2,
                    (const uint8_t**)frame.data,
                    frame.nb_samples);

                //memcpy(audio_buf, frame.data[0], data_size);
            }
            if (data_size <= 0) {
                /* No data yet, get more frames */
                continue;
            }
            /* We have data, return it and come back for more later */
            return data_size;
        }
        if (pkt.data)
            av_free_packet(&pkt);

        if (quit) {
            return -1;
        }

        if (packet_queue_get(&audioq, &pkt, 1) < 0) {
            return -1;
        }
        audio_pkt_data = pkt.data;
        audio_pkt_size = pkt.size;
    }
}

void audio_callback(void* userdata, Uint8* stream, int len) {

    AVCodecContext* aCodecCtx = (AVCodecContext*)userdata;
    int len1, audio_size;

    static uint8_t audio_buf[(MAX_AUDIO_FRAME_SIZE * 3) / 2];
    static unsigned int audio_buf_size = 0;
    static unsigned int audio_buf_index = 0;

    while (len > 0) {
        if (audio_buf_index >= audio_buf_size) {
            /* We have already sent all our data; get more */
            audio_size = audio_decode_frame(aCodecCtx, audio_buf, sizeof(audio_buf));
            if (audio_size < 0) {
                /* If error, output silence */
                audio_buf_size = 1024; // arbitrary?
                memset(audio_buf, 0, audio_buf_size);
            }
            else {
                audio_buf_size = audio_size;
            }
            audio_buf_index = 0;
        }
        len1 = audio_buf_size - audio_buf_index;
        if (len1 > len)
            len1 = len;
        fprintf(stderr, "index=%d, len1=%d, len=%d\n",
            audio_buf_index,
            len,
            len1);
        memcpy(stream, (uint8_t*)audio_buf + audio_buf_index, len1);
        len -= len1;
        stream += len1;
        audio_buf_index += len1;
    }
}

int main(int argc, char* argv[]) {

    /////
        // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader chromaShader("base.vs", "trails1.frag");
    Shader erodeShader("base.vs", "trails2.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions        // texture coords
         1.f,  1.f, 0.0f,   1.0f, 1.0f, // top right
         1.f, -1.f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.f,  -1.f, 0.0f,   0.0f, 0.0f, // bottom left
         -1.f, 1.f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int texture1;
    {
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }

    unsigned int texture2;
    {
        glGenTextures(1, &texture2);
        glBindTexture(GL_TEXTURE_2D, texture2); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }


    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;


    chromaShader.use();
    chromaShader.setInt("texture1", 0);
    chromaShader.setInt("texture2", 1);

    erodeShader.use();
    erodeShader.setInt("texture1", 0);
    erodeShader.setInt("texture2", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    float Radius = 5.0;


    /////
    int  		  ret = -1;
    int             i, videoStream, audioStream;

    AVFormatContext* pFormatCtx = NULL;

    //for video decode
    AVCodecContext* pCodecCtxOrig = NULL;
    AVCodecContext* pCodecCtx = NULL;
    AVCodec* pCodec = NULL;

    struct SwsContext* sws_ctx = NULL;

    AVPicture* pict = NULL;
    AVFrame* pFrame = NULL;
    AVPacket        packet;
    int             frameFinished;

    //for audio decode
    AVCodecContext* aCodecCtxOrig = NULL;
    AVCodecContext* aCodecCtx = NULL;
    AVCodec* aCodec = NULL;


    int64_t in_channel_layout;
    int64_t out_channel_layout;

    //for video render
    int		  w_width = 640;
    int 		  w_height = 480;

    int             pixformat;
    SDL_Rect        rect;

    //SDL_Window* win;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    //for event
    SDL_Event       event;

    //for audio
    SDL_AudioSpec   wanted_spec, spec;

    if (argc < 2) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Usage: command <file>");
        return ret;
    }

    // Register all formats and codecs
    av_register_all();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL - %s\n", SDL_GetError());
        return ret;
    }

    // Open video file
    if (avformat_open_input(&pFormatCtx, argv[1], NULL, NULL) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open multi-media file");
        goto __FAIL; // Couldn't open file
    }

    // Retrieve stream information
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't find stream information ");
        goto __FAIL;
    }

    // Dump information about file onto standard error
    av_dump_format(pFormatCtx, 0, argv[1], 0);

    // Find the first video stream
    videoStream = -1;
    audioStream = -1;

    for (i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO &&
            videoStream < 0) {
            videoStream = i;
        }
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO &&
            audioStream < 0) {
            audioStream = i;
        }
    }

    if (videoStream == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, " Didn't find a video stream ");
        goto __FAIL; // Didn't find a video stream
    }

    if (audioStream == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, " Didn't find a audio stream ");
        goto __FAIL; // Didn't find a video stream
    }

    aCodecCtxOrig = pFormatCtx->streams[audioStream]->codec;
    aCodec = avcodec_find_decoder(aCodecCtxOrig->codec_id);
    if (!aCodec) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unsupported codec! ");
        goto __FAIL; // Didn't find a video stream
    }

    // Copy context
    aCodecCtx = avcodec_alloc_context3(aCodec);
    if (avcodec_copy_context(aCodecCtx, aCodecCtxOrig) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't copy codec context! ");
        goto __FAIL; // Didn't find a video stream
    }

    // Set audio settings from codec info
    wanted_spec.freq = aCodecCtx->sample_rate;
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.channels = aCodecCtx->channels;
    wanted_spec.silence = 0;
    wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;
    wanted_spec.callback = NULL;// audio_callback;
    wanted_spec.userdata = aCodecCtx;

    if (SDL_OpenAudio(&wanted_spec, &spec) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open audio device - %s!", SDL_GetError());
        goto __FAIL;
    }

    avcodec_open2(aCodecCtx, aCodec, NULL);

    packet_queue_init(&audioq);

    in_channel_layout = av_get_default_channel_layout(aCodecCtx->channels);
    out_channel_layout = in_channel_layout; //AV_CH_LAYOUT_STEREO;
    fprintf(stderr, "in layout:%lld, out layout:%lld \n", in_channel_layout, out_channel_layout);

    audio_convert_ctx = swr_alloc();
    if (audio_convert_ctx) {
        swr_alloc_set_opts(audio_convert_ctx,
            out_channel_layout,
            AV_SAMPLE_FMT_S16,
            aCodecCtx->sample_rate,
            in_channel_layout,
            aCodecCtx->sample_fmt,
            aCodecCtx->sample_rate,
            0,
            NULL);
    }
    swr_init(audio_convert_ctx);

    SDL_PauseAudio(0);

    // Get a pointer to the codec context for the video stream
    pCodecCtxOrig = pFormatCtx->streams[videoStream]->codec;

    // Find the decoder for the video stream
    pCodec = avcodec_find_decoder(pCodecCtxOrig->codec_id);
    if (pCodec == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unsupported codec!");
        goto __FAIL;
    }

    // Copy context
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to copy context of codec!");
        goto __FAIL;
    }

    // Open codec
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open audio decoder!");
        goto __FAIL;
    }

    // Allocate video frame
    pFrame = av_frame_alloc();

    w_width = pCodecCtx->width;
    w_height = pCodecCtx->height;

    fprintf(stderr, "width:%d, height:%d\n", w_width, w_height);

    //win = SDL_CreateWindow("Media Player",
    //    SDL_WINDOWPOS_UNDEFINED,
    //    SDL_WINDOWPOS_UNDEFINED,
    //    w_width, w_height,
    //    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    //if (!win) {
    //    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window!");
    //    goto __FAIL;
    //}

    //renderer = SDL_CreateRenderer(win, -1, 0);
    //if (!renderer) {
    //    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create renderer!");
    //    goto __FAIL;
    //}

    //pixformat = SDL_PIXELFORMAT_IYUV;
    //texture = SDL_CreateTexture(renderer,
    //    pixformat,
    //    SDL_TEXTUREACCESS_STREAMING,
    //    w_width,
    //    w_height);
    //if (!texture) {
    //    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create Texture!");
    //    goto __FAIL;
    //}

    // initialize SWS context for software scaling
    sws_ctx = sws_getContext(pCodecCtx->width,
        pCodecCtx->height,
        pCodecCtx->pix_fmt,
        pCodecCtx->width,
        pCodecCtx->height,
        AV_PIX_FMT_YUV420P,
        SWS_BILINEAR,
        NULL,
        NULL,
        NULL);

    pict = (AVPicture*)malloc(sizeof(AVPicture));
    avpicture_alloc(pict,
        AV_PIX_FMT_YUV420P,
        pCodecCtx->width,
        pCodecCtx->height);

    // Read frames and save first five frames to disk
    while (av_read_frame(pFormatCtx, &packet) >= 0) {
        // Is this a packet from the video stream?
        if (packet.stream_index == videoStream) {
            // Decode video frame
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

            // Did we get a video frame?
            if (frameFinished) {

                // Convert the image into YUV format that SDL uses	
                sws_scale(sws_ctx, (uint8_t const* const*)pFrame->data,
                    pFrame->linesize, 0, pCodecCtx->height,
                    pict->data, pict->linesize);

                ///////

                const BYTE* srcY = pict->data[0];
                const BYTE* srcU = pict->data[1];
                const BYTE* srcV = pict->data[2];

                BYTE* rgbdstp = new BYTE[pCodecCtx->width* pCodecCtx->height*4];

                libyuv::I420ToARGB(srcY, pict->linesize[0], srcU, pict->linesize[1], srcV, pict->linesize[2], rgbdstp, pCodecCtx->width*4, pCodecCtx->width, -pCodecCtx->height);


                for (int i = 0; i < pCodecCtx->width * pCodecCtx->height * 4; i+=4) {
                    auto tempr = rgbdstp[i];
                    rgbdstp[i] = rgbdstp[i + 2];
                    rgbdstp[i + 2] = tempr;
                }

                //bmp_write(rgbdstp, 720, 720, "111");

                //glActiveTexture(GL_TEXTURE0);
                //glBindTexture(GL_TEXTURE_2D, texture1);

                //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pCodecCtx->width, pCodecCtx->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbdstp);//png图像需使用RGBA

                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, texture2);

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pCodecCtx->width, pCodecCtx->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbdstp);//png图像需使用RGBA

                        // input
        // -----
                    processInput(window);

                    // render
                    // ------
                    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT);


                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT);
                    chromaShader.use();
                    chromaShader.setInt("texture1", 0);
                    //ourShader.setFloatv2("samplingRadius", Radius, 0);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
                    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
                    GLfloat temp[4] = { 0.0, 0.0, 0.0, 0.0 };
                    glClearBufferfv(GL_COLOR, 0, temp);
                    glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
                    // 将输出重新写入到输入的过程
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

                    //pass2
                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);// 如果a通道为0，会出现图像轮廓渐渐变小，然后消失?
                    glClear(GL_COLOR_BUFFER_BIT);
                    erodeShader.use();
                    erodeShader.setInt("texture1", 0);
                    //ourShader.setFloatv2("samplingRadius", 0, Radius);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                    //// 纹理还原
                    //glActiveTexture(GL_TEXTURE0);
                    //glBindTexture(GL_TEXTURE_2D, texture1);


                    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
                    // -------------------------------------------------------------------------------
                    glfwSwapBuffers(window);
                    glfwPollEvents();

                    std::this_thread::sleep_for(std::chrono::milliseconds(25));

                ///////////
                //SDL_UpdateYUVTexture(texture, NULL,
                //    pict->data[0], pict->linesize[0],
                //    pict->data[1], pict->linesize[1],
                //    pict->data[2], pict->linesize[2]);

                //rect.x = 0;
                //rect.y = 0;
                //rect.w = pCodecCtx->width;
                //rect.h = pCodecCtx->height;

                //SDL_RenderClear(renderer);
                //SDL_RenderCopy(renderer, texture, NULL, &rect);
                //SDL_RenderPresent(renderer);

                av_free_packet(&packet);
            }
        }
        else if (packet.stream_index == audioStream) { //for audio
            packet_queue_put(&audioq, &packet);
        }
        else {
            av_free_packet(&packet);
        }

        // Free the packet that was allocated by av_read_frame
        SDL_PollEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
            quit = 1;
            goto __QUIT;
            break;
        default:
            break;
        }

    }

__QUIT:
    ret = 0;

__FAIL:
    // Free the YUV frame
    if (pFrame) {
        av_frame_free(&pFrame);
    }

    // Close the codecs
    if (pCodecCtxOrig) {
        avcodec_close(pCodecCtxOrig);
    }

    if (pCodecCtx) {
        avcodec_close(pCodecCtx);
    }

    if (aCodecCtxOrig) {
        avcodec_close(aCodecCtxOrig);
    }

    if (aCodecCtx) {
        avcodec_close(aCodecCtx);
    }

    // Close the video file
    if (pFormatCtx) {
        avformat_close_input(&pFormatCtx);
    }

    if (pict) {
        avpicture_free(pict);
        free(pict);
    }

    //if (win) {
    //    SDL_DestroyWindow(win);
    //}

    //if (renderer) {
    //    SDL_DestroyRenderer(renderer);
    //}

    //if (texture) {
    //    SDL_DestroyTexture(texture);
    //}

    SDL_Quit();

    return ret;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
