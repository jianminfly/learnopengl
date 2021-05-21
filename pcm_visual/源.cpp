//
//  main.cpp
//  Spectrum5.0
//
//  Created by boone on 2018/8/14.
//  Copyright © 2018年 boone. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fftw3.h>

#include <shader_s.h>

using namespace std;

struct wav_struct
{
    unsigned long file_size;        //文件大小
    unsigned short channel;            //通道数
    unsigned long frequency;        //采样频率
    unsigned long Bps;                //Byte率
    unsigned short sample_num_bit;    //一个样本的位数
    unsigned long data_size;        //数据大小
    unsigned char* data;            //音频数据
};

vector<float> vertices;    //用于存储pcm文件解析出的数据
int istart = 0;              //记录频谱每次绘制的起点、控制频谱的绘制
int wstart = 0;
int pstart = 10000;
int n;       //记录pcm文件中数据个数

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void drawLine();
void drawPoint();
void drawWave();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

//PCM文件数据解码保存到数组中  ssh-keygen -t rsa -C "363058661@qq.com" -b 4096
void fileOutput()
{
    short pcm_In = 0;
    int size = 0;
    FILE* fp = fopen("dayu.pcm", "rb+");     //为读写打开一个二进制文件 即pcm文件

    int i = 0;
    while (!feof(fp))
    {
        size = fread(&pcm_In, 1, 1, fp);     //pcm中每个数据大小为2字节，每次读取1个数据
        if (size > 0)
        {
            vertices.push_back(pcm_In / (double)255);
            //  cout<<pcm_In<<"     ";
        }
        i++;
    }

    n = i;

    fclose(fp);
}

int main()
{
    fileOutput();

    // glfw: 初始化并配置
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // ！！！for OS X   没有此行报错为 not support version 330
#endif

    // glfw 创建窗口
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Music Circular Spectrum", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: 家在所有OpenGL函数指针
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // 构建并编译着色器程序
    // ------------------------------------
    Shader ourShader("spectrum.vs", "spectrum.fs");

    //FFTW
    fftw_complex* in, * out;
    fftw_plan p;
    in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * n);
    out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * n);
    if (in == NULL || out == NULL) {
        cout << "ERROR: Fail to memory allocation" << endl;
    }
    else {
        int  i = 0;
        for (int i = 0; i < vertices.size(); i++) {
            in[i][0] = vertices[i];
            in[i][1] = 0;
            i++;
        }
    }
    p = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);
    fftw_destroy_plan(p);
    fftw_cleanup();

    //顶点数组
    float* arr = new float[3 * n];
    float* arr1 = new float[3 * n];
    float* arr2 = new float[3 * n / 2];

    float xstart = -1.0;
    int j = 0;
    //直线型频谱图数据存储
    for (int i = 0; i < 3 * n; )    //用迭代器的方式输出容器对象的值
    {
        float temp = sqrt(out[j][0] * out[j][0] + out[j][1] * out[j][1]) / 10000;
        j++;

        arr[i++] = xstart;
        arr[i++] = 0.0f;
        arr[i++] = 0.0f;

        arr[i++] = xstart;
        arr[i++] = temp;
        arr[i++] = 0.0f;

        xstart = xstart + 0.002;
        if (xstart > 1.0) {
            xstart = -1.0;
        }
    }

    if (in != NULL) {
        fftw_free(in);
    }
    if (out != NULL) {
        fftw_free(out);
    }
    //直线型
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, 12 * n, arr, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    // 循环渲染
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // 渲染
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        glBindVertexArray(VAO); // 激活VAO表示的顶点缓存
        if (istart < 3 * n) {
            drawLine();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: 每当窗口大小改变时，调用该回调函数
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
#include<thread>
#include<chrono>
using namespace std;
//绘制频谱
void drawLine()
{
    this_thread::sleep_for(chrono::milliseconds(100));
  //颜色随机设置 
    float redValue = 0.0f;
    float blueValue = 1.0f;

    for (int i = istart; i < 2000 + istart; i += 2) {

        glUniform4f(0, redValue, 1.0f, blueValue, 1.0f);

        if (i <= 1000 + istart) {
            redValue = redValue + 0.002;
            blueValue = blueValue - 0.002;
        }
        else {
            redValue = redValue - 0.002;
            blueValue = blueValue + 0.002;
        }
        glLineWidth(5);
        glDrawArrays(GL_LINES, i, 2);
    }

    istart += 2000;
}

//绘制离散型频谱
void drawPoint()
{

    for (int i = pstart; i < 800 + pstart; i++) {

        glUniform4f(0, 0.2f, 0.7f, 1.0f, 1.0f);

        glPointSize(3);
        glDrawArrays(GL_POINTS, i, 1);
    }

    pstart += 800;
}
//绘制波形频谱
void drawWave()
{
    //颜色随机设置
    float redValue = 0.0f;
    float blueValue = 1.0f;

    for (int i = wstart; i < 2000 + wstart; i++) {

        glUniform4f(0, redValue, blueValue, 1.0f, 1.0f);

        if (i <= 500 + wstart) {
            redValue = redValue + 0.002;
            blueValue = blueValue - 0.002;
        }
        else {
            redValue = redValue - 0.002;
            blueValue = blueValue + 0.002;
        }

        //glLineWidth(8);
        glPointSize(4);
        glDrawArrays(GL_POINTS, i, 1);
    }

    wstart += 2000;
}
