#pragma once
#include <algorithm>
#include<iostream>

// 查询一串值的下一个连续的小于阈值的范围.
void find_next_continus_range(double* values, double low_threshold, int from_index, int to_index,
    int& ostart, int& oend) {

    int start = -1;
    int end = -1;

    for (int i = from_index; i < to_index; i++) {
        auto value = values[i];

        if (value <= low_threshold) {
            if (start == -1) {
                start = i;
                end = i;
            }
            else {
                end = i;
            }
        }
        else {
            if (start != -1)
                break;
        }
    }
    ostart = start;
    oend = end;
}
// 查询一串值的下一个连续的小于阈值的范围.
void find_max_length_continus_range(double* values, double low_threshold, int from_index, int to_index,
    int& ostart, int& oend) {

    int start = -1;
    int end = -1;

    while (true) {
        int nex_start = -1, next_end = -1;
        find_next_continus_range(values, low_threshold, from_index, to_index, nex_start, next_end);

        if (nex_start == -1)
            break;
        // 范围更大.
        if (next_end - nex_start > end - start) {
            start = nex_start;
            end = next_end;
        }
        from_index = next_end + 1;
    }
    ostart = start;
    oend = end;
}

double* convfft(__int32* a, double* b, int alen, int blen) {
    unsigned int sz = alen + blen - 1;
    double* cx = new double[sz];
    memset(cx, 0.00, sz * sizeof(double));

    int b_len = blen;
    int a_len = alen;
    for (int n = 0; n < sz; n++)
    {
        int kmin = (n >= b_len - 1) ? n - (b_len - 1) : 0;
        int kmax = (n < a_len - 1) ? n : a_len - 1;


        for (int k = kmin; k <= kmax; k++)
        {
            cx[n] += double(a[k]) * b[n - k];
        }
    }

    return cx;
}

// 进行卷积运算以平滑, 防止部分值变化剧烈而产生的影响, 然后再选择中间的 256 部分.
double* get_one_channel_diff_threshold(__int32* hist, int len, float& threshold, float& smooth_radius) {

    auto convolve_radius = 4;
    auto convolve_size = convolve_radius * 2 + 1;

    double* v = new double[convolve_size];
    for (int i = 0; i < convolve_size; i++) {
        v[i] = 1 / double(convolve_size);
    }

    int convlen = len + convolve_size - 1;
    double*  r = NULL;

    // 卷积，目的是平滑，防止突变产生过大影响
    r = convfft(hist, v, len, convolve_size);


    double* rhist = new double[convlen - 8];
    int rhistlen = convlen - 8;

    // 卷积结果去掉头和尾，刚好是和hist一样长度
    for (int i = 4, j = 0; i < convlen - 4; i++, j++)
        rhist[j] = r[i];

    int count_integral = 0;
    int sumrhist = 0;
    // 计算数量的累计积分.
    for (int i = 0; i < rhistlen; i++) {
        count_integral += i * rhist[i];
        sumrhist += rhist[i];
    }
    // 计算重心位置.
    float center = float(count_integral) / float(sumrhist);
    // 每个位置的均值
    double average_value = double(sumrhist) / 256.0;

    int start = -1;
    int end = -1;
    // 不断双倍提升阈值, 从 13% 到 760%.
    for (int t = 0; t < 11; t++) {
        auto low_threshold = average_value * pow( 1.5, (t - 5));
        int next_start = 0, next_end = 0;
        find_max_length_continus_range(rhist, low_threshold, center, 256, next_start, next_end);

        // 直到阈值增加 1.5 倍时, 增长小于 1.125 倍.
        if (next_end - next_start >= 16 && next_end - next_start < (end - start) * 1.125)
            break;
        else {
            start = next_start;
            end = next_end;
        }
    }
    // 获得谷底的近似中间值作为阈值.
    // 实测背景向前景略微移动效果会更好.
    threshold = (float(start) * 0.5 + float(end) * 0.5);
    smooth_radius = (float(end) - float(start)) / 2.0;

    delete[] v;
    delete[] r;

    return rhist;
}
// 分析图片的色差键的分隔阈值和平滑范围.
void analysis_channel_diff_threshold(unsigned char* image, int width, int height, 
    bool is_blue_screen, float& _threshold, float& _average_backgroundr, float& _average_backgroundg, float& _average_backgroundb) {

    __int32 hist[256] = { 0 };
    // RGB 通道的和.
    __int32 sum[3] = { 0 };
    // 抽样的背景色像素综合.
    int total_pixels = 0;
    int h = height, w = width;
    // 跨越多少行列进行采样.
    // 每隔 10 行以及列 (每 100 个抽取一个) 进行抽样.
    // 例如 1M 的图片, 会抽取 10000 个像素.
    int stride = 10;

    for (int y = 0; y < h; y += stride) {
        for (int i = 0; i < w * 4; i += stride * 4) {

            auto r = image[w * y * 4 + i];
            auto g = image[w * y * 4 + i + 1];
            auto b = image[w * y * 4 + i + 2];

            if (!is_blue_screen) {

                auto diff = 256 - (2 * (g - std::max(r, b)));

                if (0 <= diff && diff < 256) {
                    hist[diff] += 1;
                    sum[0] += r;
                    sum[1] += g;
                    sum[2] += b;
                    total_pixels += 1;
                }

            }
            else {
                auto diff = 256 - (2 * (g - std::max(r, g)));
                if (0 <= diff && diff < 256) {
                    hist[diff] += 1;
                    sum[0] += r;
                    sum[1] += g;
                    sum[2] += b;
                    total_pixels += 1;
                }
            }

        }
    }

    float average_backgroundr = float(sum[0]) / float(total_pixels);
    float average_backgroundg = float(sum[1]) / float(total_pixels);
    float average_backgroundb = float(sum[2]) / float(total_pixels);

    float threshold = 0, smooth_radius = 0;
    double* rhist = get_one_channel_diff_threshold(hist, 256, threshold, smooth_radius);

    threshold /= 255;
    smooth_radius /= 255;

    _threshold = threshold;
    _average_backgroundr = average_backgroundr / 255.0;
    _average_backgroundg = average_backgroundg / 255.0;
    _average_backgroundb = average_backgroundb / 255.0;

    delete[] rhist;
}