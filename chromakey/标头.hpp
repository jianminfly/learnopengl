#pragma once
#include <algorithm>
#include<iostream>

// ��ѯһ��ֵ����һ��������С����ֵ�ķ�Χ.
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
// ��ѯһ��ֵ����һ��������С����ֵ�ķ�Χ.
void find_max_length_continus_range(double* values, double low_threshold, int from_index, int to_index,
    int& ostart, int& oend) {

    int start = -1;
    int end = -1;

    while (true) {
        int nex_start = -1, next_end = -1;
        find_next_continus_range(values, low_threshold, from_index, to_index, nex_start, next_end);

        if (nex_start == -1)
            break;
        // ��Χ����.
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

// ���о��������ƽ��, ��ֹ����ֵ�仯���Ҷ�������Ӱ��, Ȼ����ѡ���м�� 256 ����.
double* get_one_channel_diff_threshold(__int32* hist, int len, float& threshold, float& smooth_radius) {

    auto convolve_radius = 4;
    auto convolve_size = convolve_radius * 2 + 1;

    double* v = new double[convolve_size];
    for (int i = 0; i < convolve_size; i++) {
        v[i] = 1 / double(convolve_size);
    }

    int convlen = len + convolve_size - 1;
    double*  r = NULL;

    // �����Ŀ����ƽ������ֹͻ���������Ӱ��
    r = convfft(hist, v, len, convolve_size);


    double* rhist = new double[convlen - 8];
    int rhistlen = convlen - 8;

    // ������ȥ��ͷ��β���պ��Ǻ�histһ������
    for (int i = 4, j = 0; i < convlen - 4; i++, j++)
        rhist[j] = r[i];

    int count_integral = 0;
    int sumrhist = 0;
    // �����������ۼƻ���.
    for (int i = 0; i < rhistlen; i++) {
        count_integral += i * rhist[i];
        sumrhist += rhist[i];
    }
    // ��������λ��.
    float center = float(count_integral) / float(sumrhist);
    // ÿ��λ�õľ�ֵ
    double average_value = double(sumrhist) / 256.0;

    int start = -1;
    int end = -1;
    // ����˫��������ֵ, �� 13% �� 760%.
    for (int t = 0; t < 11; t++) {
        auto low_threshold = average_value * pow( 1.5, (t - 5));
        int next_start = 0, next_end = 0;
        find_max_length_continus_range(rhist, low_threshold, center, 256, next_start, next_end);

        // ֱ����ֵ���� 1.5 ��ʱ, ����С�� 1.125 ��.
        if (next_end - next_start >= 16 && next_end - next_start < (end - start) * 1.125)
            break;
        else {
            start = next_start;
            end = next_end;
        }
    }
    // ��ùȵ׵Ľ����м�ֵ��Ϊ��ֵ.
    // ʵ�ⱳ����ǰ����΢�ƶ�Ч�������.
    threshold = (float(start) * 0.5 + float(end) * 0.5);
    smooth_radius = (float(end) - float(start)) / 2.0;

    delete[] v;
    delete[] r;

    return rhist;
}
// ����ͼƬ��ɫ����ķָ���ֵ��ƽ����Χ.
void analysis_channel_diff_threshold(unsigned char* image, int width, int height, 
    bool is_blue_screen, float& _threshold, float& _average_backgroundr, float& _average_backgroundg, float& _average_backgroundb) {

    __int32 hist[256] = { 0 };
    // RGB ͨ���ĺ�.
    __int32 sum[3] = { 0 };
    // �����ı���ɫ�����ۺ�.
    int total_pixels = 0;
    int h = height, w = width;
    // ��Խ�������н��в���.
    // ÿ�� 10 ���Լ��� (ÿ 100 ����ȡһ��) ���г���.
    // ���� 1M ��ͼƬ, ���ȡ 10000 ������.
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