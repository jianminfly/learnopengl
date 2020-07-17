#define STB_IMAGE_IMPLEMENTATION
#include<Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <shader_s.h>

#include <iostream>
#include "helper.hpp"
#include <vector>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int imageWidth = 0;
int imageHeight = 0;


/*
     * CLAHE
     * 自适应直方图均衡化
     */
void AHE(int** oldmat, int pblock, int** new_mat)
{
    int width = imageWidth;
    int height = imageHeight;
    int block = pblock;

    //将图像均匀分成等矩形大小，8行8列64个块是常用的选择  
    int width_block = width / block;
    int height_block = height / block;

    //存储各个直方图  
    int** tmp = new int*[block * block];
    for (int i = 0; i < block * block; i++) {
        tmp[i] = new int[256];
        memset(tmp[i], 0x00, sizeof(int) * 256);
    }

    //存储累积函数  
    float** C = new float*[block * block];
    for (int i = 0; i < block * block; i++) {
        C[i] = new float[256];
        memset(C[i], 0x00, sizeof(int) * 256);
    }


    //计算累积函数  
    for (int i = 0; i < block; i++)
    {
        for (int j = 0; j < block; j++)
        {
            int start_x = i * width_block;
            int end_x = start_x + width_block;
            int start_y = j * height_block;
            int end_y = start_y + height_block;
            int num = i + block * j;
            int total = width_block * height_block;
            for (int ii = start_x; ii < end_x; ii++)
            {
                for (int jj = start_y; jj < end_y; jj++)
                {
                    int index = oldmat[jj][ii];
                    tmp[num][index]++;
                }
            }

            //裁剪操作  
            int average = width_block * height_block / 255;
            int LIMIT = 4 * average;
            int steal = 0;
            for (int k = 0; k < 256; k++)
            {
                if (tmp[num][k] > LIMIT) {
                    steal += tmp[num][k] - LIMIT;
                    tmp[num][k] = LIMIT;
                }

            }

            int bonus = steal / 256;
            //hand out the steals averagely  
            for (int k = 0; k < 256; k++)
            {
                tmp[num][k] += bonus;
            }

            //计算累积分布直方图  
            for (int k = 0; k < 256; k++)
            {
                if (k == 0)
                    C[num][k] = 1.0f * tmp[num][k] / total;
                else
                    C[num][k] = C[num][k - 1] + 1.0f * tmp[num][k] / total;
            }

        }
    }

    //计算变换后的像素值  
    //根据像素点的位置，选择不同的计算方法  
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            //four coners  
            if (i <= width_block / 2 && j <= height_block / 2)
            {
                int num = 0;
                new_mat[j][i] = (int)(C[num][oldmat[j][i]] * 255);
            }
            else if (i <= width_block / 2 && j >= ((block - 1) * height_block + height_block / 2)) {
                int num = block * (block - 1);
                new_mat[j][i] = (int)(C[num][oldmat[j][i]] * 255);
            }
            else if (i >= ((block - 1) * width_block + width_block / 2) && j <= height_block / 2) {
                int num = block - 1;
                new_mat[j][i] = (int)(C[num][oldmat[j][i]] * 255);
            }
            else if (i >= ((block - 1) * width_block + width_block / 2) && j >= ((block - 1) * height_block + height_block / 2)) {
                int num = block * block - 1;
                new_mat[j][i] = (int)(C[num][oldmat[j][i]] * 255);
            }

            //four edges except coners  
            else if (i <= width_block / 2)
            {
                //线性插值  
                int num_i = 0;
                int num_j = (j - height_block / 2) / height_block;
                int num1 = num_j * block + num_i;
                int num2 = num1 + block;
                float p = (j - (num_j * height_block + height_block / 2)) / (1.0f * height_block);
                float q = 1 - p;
                new_mat[j][i] = (int)((q * C[num1][oldmat[j][i]] + p * C[num2][oldmat[j][i]]) * 255);
            }
            else if (i >= ((block - 1) * width_block + width_block / 2)) {
                //线性插值  
                int num_i = block - 1;
                int num_j = (j - height_block / 2) / height_block;
                int num1 = num_j * block + num_i;
                int num2 = num1 + block;
                float p = (j - (num_j * height_block + height_block / 2)) / (1.0f * height_block);
                float q = 1 - p;
                new_mat[j][i] = (int)((q * C[num1][oldmat[j][i]] + p * C[num2][oldmat[j][i]]) * 255);
            }
            else if (j <= height_block / 2) {
                //线性插值  
                int num_i = (i - width_block / 2) / width_block;
                int num_j = 0;
                int num1 = num_j * block + num_i;
                int num2 = num1 + 1;
                float p = (i - (num_i * width_block + width_block / 2)) / (1.0f * width_block);
                float q = 1 - p;
                new_mat[j][i] = (int)((q * C[num1][oldmat[j][i]] + p * C[num2][oldmat[j][i]]) * 255);
            }
            else if (j >= ((block - 1) * height_block + height_block / 2)) {
                //线性插值  
                int num_i = (i - width_block / 2) / width_block;
                int num_j = block - 1;
                int num1 = num_j * block + num_i;
                int num2 = num1 + 1;
                float p = (i - (num_i * width_block + width_block / 2)) / (1.0f * width_block);
                float q = 1 - p;
                new_mat[j][i] = (int)((q * C[num1][oldmat[j][i]] + p * C[num2][oldmat[j][i]]) * 255);
            }

            //inner area  
            else {
                int num_i = (i - width_block / 2) / width_block;
                int num_j = (j - height_block / 2) / height_block;
                int num1 = num_j * block + num_i;
                int num2 = num1 + 1;
                int num3 = num1 + block;
                int num4 = num2 + block;
                float u = (i - (num_i * width_block + width_block / 2)) / (1.0f * width_block);
                float v = (j - (num_j * height_block + height_block / 2)) / (1.0f * height_block);
                new_mat[j][i] = (int)((u * v * C[num4][oldmat[j][i]] +
                    (1 - v) * (1 - u) * C[num1][oldmat[j][i]] +
                    u * (1 - v) * C[num2][oldmat[j][i]] +
                    v * (1 - u) * C[num3][oldmat[j][i]]) * 255);

            }

            //new_mat[j][i] = new_mat[j][i] + (new_mat[j][i] << 8) + (new_mat[j][i] << 16);
        }
    }
    for (int i = 0; i < block * block; i++) {
        delete[] tmp[i];
    }
    for (int i = 0; i < block * block; i++) {
        delete[] C[i];
    }
    delete[] tmp;
    delete[] C;
}

//对8位灰度图进行直方图均衡化
void histogramEqulize_8Gray(unsigned char* pimg, int biSizeImage) {
    int** imageMat = new int*[imageHeight];
    for (int i = 0; i < imageHeight; i++)
        imageMat[i] = new int[imageWidth];

    for (int i = 0; i < imageHeight; i++)
        for (int j = 0; j < imageWidth; j++)
            imageMat[i][j] = pimg[j + imageWidth * i];

    int** outMat = new int* [imageHeight];
    for (int i = 0; i < imageHeight; i++)
        outMat[i] = new int[imageWidth];
    AHE(imageMat, 8, outMat);

    for (int i = 0; i < imageHeight; i++)
        for (int j = 0; j < imageWidth; j++)
            pimg[j + imageWidth * i] = outMat[i][j];

    for (int i = 0; i < imageHeight; i++)
        delete[] imageMat[i];
    for (int i = 0; i < imageHeight; i++)
        delete outMat[i];
    delete[] imageMat;
    delete[] outMat;
}



////对8位灰度图进行直方图均衡化
//void histogramEqulize_8Gray(unsigned char* pimg, int biSizeImage) {
//    unsigned char* pcur, * pend = pimg + biSizeImage;
//    unsigned int hist[256], LUT[256];
//    memset(hist, 0, sizeof(hist));
//    for (pcur = pimg; pcur < pend; ++pcur) {
//        hist[*pcur]++;
//    }
//    double sum = 0;
//    for (int i = 0; i < 256; ++i) {
//        sum += hist[i];
//        LUT[i] = 255 * (sum / biSizeImage);
//    }
//    for (pcur = pimg; pcur < pend; ++pcur) {
//        *(pcur) = LUT[*pcur];
//    }
//}
//对24位彩色图，对RGB分别均衡化
void histogramEqualize_24RGB(unsigned char* pimg, int biSizeImage) {
    unsigned char* img_R, * img_G, * img_B;
    unsigned char* pcur, * pend = pimg + biSizeImage;
    img_R = (unsigned char*)malloc(biSizeImage / 3);
    assert(img_R != NULL);
    img_G = (unsigned char*)malloc(biSizeImage / 3);
    assert(img_G != NULL);
    img_B = (unsigned char*)malloc(biSizeImage / 3);
    assert(img_B != NULL);
    //位图颜色格式是BGR
    unsigned char* pb = img_B, * pg = img_G, * pr = img_R;
    for (pcur = pimg; pcur < pend;) {
        *(pb++) = *(pcur++);
        *(pg++) = *(pcur++);
        *(pr++) = *(pcur++);
    }
    histogramEqulize_8Gray(img_B, biSizeImage / 3);
    histogramEqulize_8Gray(img_G, biSizeImage / 3);
    histogramEqulize_8Gray(img_R, biSizeImage / 3);

    pb = img_B, pg = img_G, pr = img_R;
    for (pcur = pimg; pcur < pend;) {
        *(pcur++) = *(pb++);
        *(pcur++) = *(pg++);
        *(pcur++) = *(pr++);
    }
}

void histogramEqualize_advance(unsigned char* prgbimg) {
    int count[256*3] = { 0 };
    
    for (unsigned char* i = prgbimg; i < prgbimg + imageHeight * imageWidth * 3; i += 3) {
        int rgbsum = (i[0] + i[1] + i[2]);
        count[rgbsum]++;
    }
    double ln_count[256] = { 0 };
    double ln_size = 0;//log(imageHeight * imageWidth);
    for (int i = 0; i < 256*3; i++) {
        //ln_count[i] = log(count[i] + 1);
        ln_size += log(count[i] + 1);
    }
    // 计算累计分布直方图
    double sum = 0;
    double C[256*3] = { 0 };
    for (int i = 0; i < 256*3; ++i) {
        sum += log(count[i]+1);
        C[i] = sum / ln_size;
        //sum += count[i];
        //C[i] = sum / ((double)imageHeight * (double)imageWidth);
    }

    // 逐像素计算输出
    for (unsigned char* i = prgbimg; i < prgbimg + imageHeight * imageWidth * 3; i += 3) {

        int rgbsum = (i[0] + i[1] + i[2]);
        double wi = static_cast<double>(rgbsum) / 3.00;
        double wo = 255 * C[rgbsum];
        double a = (double)wo / (double)wi;

        if (a * i[0] > 255 || a * i[1] > 255 || a * i[2] > 255) {
        //if (a > 1) {
            a = (255.00 - (double)wo) / (255.00 -  (double)wi);

            i[0] = 255.0 - a * (255.0 - i[0]);
            i[1] = 255.0 - a * (255.0 - i[1]);
            i[2] = 255.0 - a * (255.0 - i[2]);

        }
        else {

            i[0] = a * i[0];
            i[1] = a * i[1];
            i[2] = a * i[2];
        }
        if (rgbsum > 600 && i[0] == 0 && i[1] == 0 && i[2] == 0)
            std::cout << "";
    }
}
#define RGBmix
void gao( unsigned char* imageA , DWORD size) {

    ///////////////////////////////////////////////////////////////////////////
    //直方图处理
    //histogramEqualize_24RGB(imageA, size);
#ifdef HSV
    // hsv start
    double* hsvdata = new double[imageHeight * imageWidth * 3];
    double* j = hsvdata;
    for (unsigned char* i = imageA; i < imageA + imageHeight * imageWidth * 3; i += 3) {
        double hsv[3] = { 0 };
        _cie_rgb2hsv(i, hsv);
        
        *(j) = hsv[0];
        j++;
        *(j) = hsv[1];
        j++;
        *(j) = hsv[2];
        j++;
    }
    unsigned char* vdata = new unsigned char[imageHeight * imageWidth];
    auto q = vdata;
    for (auto p = hsvdata + 2; p < hsvdata + imageHeight * imageWidth * 3; p += 3) {
        *q = (*p) * 255;
        q++;
    }
    histogramEqulize_8Gray(vdata, imageHeight * imageWidth);
    auto q1 = vdata;
    for (auto p = hsvdata + 2; p < hsvdata + imageHeight * imageWidth * 3; p += 3) {
        *p = (double)(*q1) / 255.00;
        q1++;
    }
    unsigned char* i = imageA;
    for (auto j = hsvdata; j < hsvdata + imageHeight * imageWidth * 3; j += 3) {
        unsigned char rgb[3] = { 0 };
        _cie_hsv2rgb(&*j, rgb);
        *i = rgb[0];
        i++;
        *(i) = rgb[1];
        i++;
        *(i) = rgb[2];
        i++;
    }
    delete[] hsvdata;
    delete[] vdata;
    //hsv end
#endif

#ifdef LAB
    //lab start
    float* labdata = new float[imageHeight * imageWidth * 3];
    float* j = labdata;
    for (unsigned char* i = imageA; i < imageA + imageHeight * imageWidth * 3; i += 3) {

        float lab[3] = { 0 };
        RGB2Lab(i[2]/255.00, i[1] / 255.00, i[0] / 255.00, &lab[0], &lab[1], &lab[2]);
        
        *(j) = lab[0];
        j++;
        *(j) = lab[1];
        j++;
        *(j) = lab[2];
        j++;
    }
    unsigned char* ldata = new unsigned char[imageHeight * imageWidth];
    unsigned char* q = ldata;
    for (float* p = labdata; p < labdata + imageHeight * imageWidth * 3; p += 3) {
        *q = (*p)*2.55;
        q++;
    }
    histogramEqulize_8Gray(ldata, imageHeight * imageWidth);
    unsigned char* q1 = ldata;
    for (float* p = labdata; p < labdata + imageHeight * imageWidth * 3; p += 3) {
        *p = (double)(*q1) / 2.55;
        q1++;
    }
    unsigned char* i = imageA;
    for (float* j = labdata; j < labdata + imageHeight * imageWidth * 3; j += 3) {

        float rgb[3] = { 0 };
        Lab2RGB(j[0], j[1], j[2], &rgb[2], &rgb[1], &rgb[0]);

        *i = rgb[0]*255.00;
        i++;
        *(i) = rgb[1] * 255.00;
        i++;
        *(i) = rgb[2] * 255.00;
        i++;
    }

    //lab end
#endif

#ifdef  ADVANCE

    histogramEqualize_advance(imageA);
    
#endif //  ADVANCE

#ifdef ADVANCEEX

    unsigned char* lumin = new unsigned char[imageHeight * imageWidth];
    int j = 0;
    for (unsigned char* i = imageA; i < imageA + imageHeight * imageWidth * 3; i += 3, j++) {
        lumin[j] = (i[0] + i[1] + i[2]) / 3;
    }

    unsigned char* lumout = new unsigned char[imageHeight * imageWidth];
    memcpy(lumout, lumin, imageHeight * imageWidth);
    histogramEqulize_8Gray(lumout, imageHeight * imageWidth);

    auto cursor = imageA;
    for (int i = 0; i < imageHeight * imageWidth; i++) {
        auto r = cursor[0];
        auto g = cursor[1];
        auto b = cursor[2];

        auto a = (double)lumout[i] / (double)lumin[i];

        if (a * cursor[0] > 255 || a * cursor[1] > 255 || a * cursor[2] > 255) {
            //if (a > 1) {
            a = (255.00 - (double)lumout[i]) / (255.00 - (double)lumin[i]);

            cursor[0] = 255.0 - a * (255.0 - cursor[0]);
            cursor[1] = 255.0 - a * (255.0 - cursor[1]);
            cursor[2] = 255.0 - a * (255.0 - cursor[2]);

        }
        else {

            cursor[0] = a * cursor[0];
            cursor[1] = a * cursor[1];
            cursor[2] = a * cursor[2];
        }


        cursor += 3;
    }

#endif // ADVANCEEX

    // 目前来说效果最好的方式，取自https://www.cnblogs.com/Imageshop/archive/2013/04/07/3006334.html中提供的AdaptHistEqualize.dll反编译后梳理的结果
    // 主要特色为在计算直方图的时候，将rgb通道的级数统计到一个直方图，再用分别用原图像的三个通道的像素值和直方图进行计算
#ifdef RGBmix

    UINT hist[256] = { 0 };
    for (BYTE* p = imageA; p <= imageA + size; p += 3)
    {
        ++* (hist + *p);
        ++* (hist + p[1]);
        ++* (hist + p[2]);
    }
    int average = size / 255;
    int LIMIT = 7 * average;
    int steal = 0;
    for (int k = 0; k < 256; k++)
    {
        if (hist[k] > LIMIT) {
            steal += hist[k] - LIMIT;
            hist[k] = LIMIT;
        }
    }
    int bonus = steal / 256;
    //hand out the steals averagely  
    for (int k = 0; k < 256; k++)
    {
        hist[k] += bonus;
    }

    int sum = 0;
    for (int i = 0; i < 256; i++)
    {
        sum += hist[i];
        hist[i] = ((float)sum * 255.0) / (size);
    }
    for (BYTE* p = imageA; p <= imageA + size; p += 3) {
        *p = hist[*p];
        p[1] = hist[p[1]];
        p[2] = hist[p[2]];
    }
#endif // RGBmix

}


int main()
{
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

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("base.vs", "base.frag");


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
         1.f,  1.f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         1.f, -1.f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.f,  1.f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // top left 

         1.f, -1.f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.f, -1.f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -1.f,  1.f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);



    // load and create a texture 
    // -------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* data = stbi_load("C:\\Users\\EDZ\\Desktop\\shader editor\\捕获.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {

        imageWidth = width;
        imageHeight = height;
        //gao(data, width* height*3);


        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    //stbi_image_free(data);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        ourShader.use();
        ourShader.setFloat("iResolutionx", SCR_WIDTH);
        ourShader.setFloat("iResolutiony", SCR_HEIGHT);
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 1);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
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

