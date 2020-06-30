#pragma once
#include<algorithm>
using namespace std;

// RGB模式转为HSV模式
// RGB：R(0-255)，G(0-255)，B(0-255)
// HSV：H-色相(0-360)，S-饱和度(0.0-1.0)，V-明度(0.0-1.0)
void _cie_rgb2hsv(const unsigned char
    RGB[3], double(&HSV)[3])
{
    double m_max = max(RGB[0], max(RGB[1], RGB[2]));
    double m_min = min(RGB[0], min(RGB[1], RGB[2]));
    double m_delta = m_max - m_min;

    //明度
    HSV[2] = m_max / 255.0;

    //判断RGB三个分量是否相等,包含了3个分量均为0的条件
    if (m_delta == 0)
    {
        HSV[0] = 0; //色度
        HSV[1] = 0; //饱和度
    }
    else {
        HSV[1] = m_delta / m_max;   //饱和度
        //计算H(色相)
        if (RGB[0] == m_max)          //R最大
            HSV[0] = (RGB[1] - RGB[2]) / m_delta;

        else if(RGB[1] == m_max)     //G最大
            HSV[0] = 2 + (RGB[2] - RGB[0]) / m_delta;

        else                   //B最大
            HSV[0] = 4 + (RGB[0] - RGB[1]) / m_delta;

        //转换
        HSV[0] = HSV[0] * 60;
        if (HSV[0] < 0)
            HSV[0] = HSV[0] + 360;
    }
}

// HSV模式转为RGB模式
// HSV：H-色相(0-360)，S-饱和度(0.0-1.0)，V-明度(0.0-1.0)
// HSV：H-色相(0-360)，S-饱和度(0.0-1.0)，V-明度(0.0-1.0)
// RGB：R(0-255)，G(0-255)，B(0-255)
void _cie_hsv2rgb(const double
    hsv[3], unsigned char(&rgb)[3])
{
    //RGB的0.0-1.0表示
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;

    //
    int h_i = (int)abs(hsv[0] / 60.0);
    double f = hsv[0] / 60.0 - h_i;
    double p = hsv[2] * (1 - hsv[1]);
    double q = hsv[2] * (1 - f * hsv[1]);
    double t = hsv[2] * (1 - (1 - f) * hsv[1]);

    switch (h_i) {
    case 0:
        r = hsv[2]; g = t; b = p;
        break;
    case 1:
        r = q; g = hsv[2]; b = p;
        break;
    case 2:
        r = p; g = hsv[2]; b = t;
        break;
    case 3:
        r = p; g = q; b = hsv[2];
        break;
    case 4:
        r = t; g = p; b = hsv[2];
        break;
    case 5:
        r = hsv[2]; g = p; b = q;
        break;
    }

    //转换成0-255
    rgb[0] = (int)(r * 255 + 0.5);
    rgb[1] = (int)(g * 255 + 0.5);
    rgb[2] = (int)(b * 255 + 0.5);
}



#define CLAMP0255_XY(x, y) (((x) < 0) ? 0 : ((x) > (y) ? (y) : (x)))
const float param_13 = 1.0f / 3.0f;
const float param_16116 = 16.0f / 116.0f;
const float Xn = 0.950456f;
const float Yn = 1.0f;
const float Zn = 1.088754f;

void XYZ2RGB(float X, float Y, float Z, float* R, float* G, float* B)
{
    float RR, GG, BB;
    RR = 3.240479f * X - 1.537150f * Y - 0.498535f * Z;
    GG = -0.969256f * X + 1.875992f * Y + 0.041556f * Z;
    BB = 0.055648f * X - 0.204043f * Y + 1.057311f * Z;

    *R = (float)CLAMP0255_XY(RR, 1.0f);
    *G = (float)CLAMP0255_XY(GG, 1.0f);
    *B = (float)CLAMP0255_XY(BB, 1.0f);
}

void Lab2XYZ(float L, float a, float b, float* X, float* Y, float* Z)
{
    float fX, fY, fZ;

    fY = (L + 16.0f) / 116.0f;
    if (fY > 0.206893f)
        *Y = fY * fY * fY;
    else
        *Y = (fY - param_16116) / 7.787f;

    fX = a / 500.0f + fY;
    if (fX > 0.206893f)
        *X = fX * fX * fX;
    else
        *X = (fX - param_16116) / 7.787f;

    fZ = fY - b / 200.0f;
    if (fZ > 0.206893f)
        *Z = fZ * fZ * fZ;
    else
        *Z = (fZ - param_16116) / 7.787f;

    (*X) *= Xn;
    (*Y) *= Yn;
    (*Z) *= Zn;
}

void Lab2RGB(float L, float a, float b, float* R, float* G, float* B)
{
    float X = 0.0f, Y = 0.0f, Z = 0.0f;
    Lab2XYZ(L, a, b, &X, &Y, &Z);
    XYZ2RGB(X, Y, Z, R, G, B);
}

void RGB2XYZ(float R, float G, float B, float* X, float* Y, float* Z)
{
    *X = 0.412453f * R + 0.357580f * G + 0.180423f * B;
    *Y = 0.212671f * R + 0.715160f * G + 0.072169f * B;
    *Z = 0.019334f * R + 0.119193f * G + 0.950227f * B;
}

void XYZ2Lab(float X, float Y, float Z, float* L, float* a, float* b)
{
    float fX, fY, fZ;

    X /= Xn;
    Y /= Yn;
    Z /= Zn;

    if (Y > 0.008856f)
        fY = pow(Y, param_13);
    else
        fY = 7.787f * Y + param_16116;

    *L = 116.0f * fY - 16.0f;
    *L = *L > 0.0f ? *L : 0.0f;

    if (X > 0.008856f)
        fX = pow(X, param_13);
    else
        fX = 7.787f * X + param_16116;

    if (Z > 0.008856)
        fZ = pow(Z, param_13);
    else
        fZ = 7.787f * Z + param_16116;

    *a = 500.0f * (fX - fY);
    *b = 200.0f * (fY - fZ);
}

void RGB2Lab(float R, float G, float B, float* L, float* a, float* b)
{
    float X = 0.0f, Y = 0.0f, Z = 0.0f;
    RGB2XYZ(R, G, B, &X, &Y, &Z);
    XYZ2Lab(X, Y, Z, L, a, b);
}