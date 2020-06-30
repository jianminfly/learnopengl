#pragma once
#include <stdio.h>
#include <iostream>
#include <Windows.h>

typedef DWORD _DWORD;
typedef BYTE _BYTE;
#define _ceil(a) std::ceil(a)
#define _malloc(a) malloc(a) 
#define _free(a) free(a)


int  ClipHistGram(int a1, int a2)
{
    int v2; // ebx
    int v3; // esi
    _DWORD* v4; // ecx
    signed int v5; // edx
    int v6; // eax
    int v7; // eax
    int v8; // eax
    int result; // eax
    signed int v10; // edx
    int v11; // ecx
    signed int v12; // ecx
    int v13; // edx
    int v14; // [esp+8h] [ebp-8h]
    int v15; // [esp+Ch] [ebp-4h]
    signed int v16; // [esp+Ch] [ebp-4h]

    v2 = a2;
    v3 = 0;
    v4 = (_DWORD*)(a2 + 8);
    v5 = 64;
    do
    {
        v6 = *(v4 - 2);
        if (v6 > a1)
            v3 += v6 - a1;
        v7 = *(v4 - 1);
        if (v7 > a1)
            v3 += v7 - a1;
        if (*v4 > a1)
            v3 += *v4 - a1;
        v8 = v4[1];
        if (v8 > a1)
            v3 += v8 - a1;
        v4 += 4;
        --v5;
    } while (v5);
    result = v3 / 256;
    v15 = a1 - v3 / 256;
    v10 = 0;
    do
    {
        v11 = *(_DWORD*)(v2 + 4 * v10);
        if (v11 <= a1)
        {
            v2 = a2;
            if (v11 <= v15)
            {
                v3 -= result;
                *(_DWORD*)(a2 + 4 * v10) = result + v11;
            }
            else
            {
                v3 += v11 - a1;
                *(_DWORD*)(a2 + 4 * v10) = a1;
            }
        }
        else
        {
            *(_DWORD*)(v2 + 4 * v10) = a1;
        }
        ++v10;
    } while (v10 < 256);
    do
    {
        v12 = 0;
        v16 = 0;
        v14 = v3;
        if (!v3)
            break;
        while (v12 < 256)
        {
            result = 256 / v3;
            if (256 / v3 < 1)
                result = 1;
            do
            {
                if (!v3)
                    break;
                v13 = *(_DWORD*)(v2 + 4 * v12);
                if (v13 < a1)
                {
                    *(_DWORD*)(v2 + 4 * v12) = v13 + 1;
                    --v3;
                }
                v12 += result;
            } while (v12 < 256);
            ++v16;
            if (!v3)
                return result;
            v12 = v16;
        }
    } while (v3 && v3 < v14);
    return result;
}

void AdaptHistEqualize(void* pData, int width, int height, int stride, int xValue, int yValue, double limit, int bSeparate)
{
#pragma region declare
    double v8; // st7
    signed int v9; // eax
    int cellWidth; // ecx
    int cellHeight; // ebx
    int v12; // esi
    signed int v13; // eax
    char* v14; // edi
    int v15; // eax
    int v16; // ecx
    int v17; // edi
    unsigned __int8* v18; // eax
    int v19; // ebx
    int v20; // esi
    signed int v21; // edx
    int v22; // ecx
    int v23; // edi
    int v24; // edx
    _DWORD* v25; // eax
    signed int v26; // eax
    float v27; // xmm1_4
    signed int v28; // ecx
    int v29; // ecx
    int v30; // edx
    int v31; // edi
    _DWORD* v32; // eax
    int v33; // edi
    int v34; // edx
    signed int v35; // eax
    float v36; // xmm1_4
    signed int v37; // ecx
    int v38; // ecx
    int v39; // esi
    _DWORD* v40; // eax
    signed int v41; // eax
    float v42; // xmm1_4
    signed int v43; // ecx
    bool v44; // zf
    int v45; // ecx
    size_t v46; // eax
    int v47; // esi
    signed int v48; // edx
    char* v49; // eax
    int v50; // edi
    int v51; // ecx
    int v52; // edi
    int v53; // edx
    _DWORD* v54; // eax
    signed int v55; // eax
    float v56; // xmm0_4
    signed int v57; // ecx
    int v58; // ecx
    int v59; // esi
    int v60; // ebx
    int v61; // eax
    int v62; // edx
    int v63; // ebx
    int v64; // eax
    int v65; // ecx
    int v66; // edi
    int v67; // ecx
    int v68; // ebx
    int v69; // edx
    int v70; // edi
    int v71; // ebx
    int v72; // ecx
    int v73; // edx
    int v74; // edi
    int v75; // ebx
    unsigned __int8* v76; // ecx
    int v77; // esi
    int v78; // ST34_4
    unsigned __int8 v79; // al
    int v80; // edx
    unsigned __int8 v81; // al
    int v82; // esi
    unsigned __int8 v83; // al
    int v84; // edx
    unsigned __int8 v85; // al
    int v86; // edx
    unsigned __int8 v87; // al
    int v88; // esi
    int v89; // ebx
    int v90; // edi
    char* v91; // edx
    int v92; // eax
    _BYTE* v93; // ecx
    char v94; // dl
    char* v95; // eax
    char* v96; // ecx
    int v97; // eax
    _BYTE* v98; // ecx
    char* v99; // eax
    char cellWidth0; // dl
    _BYTE* cellWidth1; // eax
    _BYTE* cellWidth2; // ecx
    char* cellWidth3; // edi
    char* cellWidth4; // ebx
    int cellWidth5; // [esp+18h] [ebp-48h]
    int cellWidth6; // [esp+24h] [ebp-3Ch]
    int cellWidth7; // [esp+24h] [ebp-3Ch]
    int cellWidth8; // [esp+24h] [ebp-3Ch]
    int cellWidth9; // [esp+24h] [ebp-3Ch]
    int cellHeight0; // [esp+24h] [ebp-3Ch]
    int cellHeight1; // [esp+24h] [ebp-3Ch]
    char* Dst; // [esp+28h] [ebp-38h]
    char* Dsta; // [esp+28h] [ebp-38h]
    char* Dstb; // [esp+28h] [ebp-38h]
    char* Dstc; // [esp+28h] [ebp-38h]
    char* Dstd; // [esp+28h] [ebp-38h]
    void* Dste; // [esp+28h] [ebp-38h]
    char* Dstf; // [esp+28h] [ebp-38h]
    char* Dstg; // [esp+28h] [ebp-38h]
    char* Dsth; // [esp+28h] [ebp-38h]
    _DWORD* Src; // [esp+2Ch] [ebp-34h]
    char* Srca; // [esp+2Ch] [ebp-34h]
    int Srcb; // [esp+2Ch] [ebp-34h]
    int Srcc; // [esp+2Ch] [ebp-34h]
    char* Srcd; // [esp+2Ch] [ebp-34h]
    char* Srce; // [esp+2Ch] [ebp-34h]
    int v127; // [esp+30h] [ebp-30h]
    int v128; // [esp+30h] [ebp-30h]
    int v129; // [esp+30h] [ebp-30h]
    int v130; // [esp+30h] [ebp-30h]
    int v131; // [esp+30h] [ebp-30h]
    int v132; // [esp+30h] [ebp-30h]
    int v133; // [esp+34h] [ebp-2Ch]
    unsigned __int8* v134; // [esp+34h] [ebp-2Ch]
    char* v135; // [esp+34h] [ebp-2Ch]
    int v136; // [esp+34h] [ebp-2Ch]
    int v137; // [esp+38h] [ebp-28h]
    int v138; // [esp+38h] [ebp-28h]
    int v139; // [esp+38h] [ebp-28h]
    char* Memory; // [esp+3Ch] [ebp-24h]
    int Memorya; // [esp+3Ch] [ebp-24h]
    unsigned __int8* v142; // [esp+40h] [ebp-20h]
    int v143; // [esp+40h] [ebp-20h]
    char* v144; // [esp+40h] [ebp-20h]
    int v145; // [esp+40h] [ebp-20h]
    int v146; // [esp+44h] [ebp-1Ch]
    char* v147; // [esp+44h] [ebp-1Ch]
    int v148; // [esp+44h] [ebp-1Ch]
    int v149; // [esp+48h] [ebp-18h]
    int v150; // [esp+48h] [ebp-18h]
    int v151; // [esp+48h] [ebp-18h]
    int v152; // [esp+4Ch] [ebp-14h]
    int v153; // [esp+50h] [ebp-10h]
    int v154; // [esp+50h] [ebp-10h]
    int v155; // [esp+54h] [ebp-Ch]
    _DWORD* v156; // [esp+58h] [ebp-8h]
    _BYTE* v157; // [esp+58h] [ebp-8h]
    signed int v158; // [esp+5Ch] [ebp-4h]
    int v159; // [esp+68h] [ebp+8h]
    int v160; // [esp+6Ch] [ebp+Ch]
    int v161; // [esp+6Ch] [ebp+Ch]
    int v162; // [esp+6Ch] [ebp+Ch]
    int v163; // [esp+70h] [ebp+10h]
    int v164; // [esp+70h] [ebp+10h]
    int v165; // [esp+78h] [ebp+18h]
    int v166; // [esp+84h] [ebp+24h]
    int v167; // [esp+84h] [ebp+24h]
    int v168; // [esp+84h] [ebp+24h]
#pragma endregion

    v158 = (signed int)_ceil((double)width / (double)xValue);
    v8 = _ceil((double)height / (double)yValue);
    cellWidth = v158;
    cellHeight = (signed int)v8;
    v9 = (signed int)v8;
    v152 = (signed int)v8;
    if (v158 & 1)
        cellWidth = v158++ + 1;
    if (v9 & 1)
    {
        cellHeight = v9 + 1;
        v152 = v9 + 1;
    }
    v127 = xValue * cellWidth;
    v133 = yValue * cellHeight;
    if (xValue * cellWidth != width || yValue * cellHeight != height)
    {
        v88 = 4 * ((24 * v127 + 31) / 32);
        v89 = (v133 - height) / 2;
        v90 = (v127 - width) / 2;
        v157 = (BYTE*)_malloc(v133 * 4 * ((24 * v127 + 31) / 32));
        if (height > 0)
        {
            Srcd = (char*)pData;
            Dstf = (char*)&v157[3 * v90 + v88 * v89];
            cellWidth7 = height;
            do
            {
                memcpy(Dstf, Srcd, stride);
                Dstf += v88;
                Srcd += stride;
                --cellWidth7;
            } while (cellWidth7);
        }
        if (v90 && height > 0)
        {
            v91 = 0;
            v92 = v88 * v89;
            Srce = 0;
            Memorya = v88 * v89;
            v145 = height;
            do
            {
                if (v90 > 0)
                {
                    v93 = &v157[v92];
                    Dstg = &v91[2 * v90 - 3 + v90 + (_DWORD)pData];
                    cellWidth8 = (v127 - width) / 2;
                    do
                    {
                        v94 = *Dstg;
                        Dstg -= 3;
                        *v93 = v94;
                        v93 += 3;
                        --cellWidth8;
                    } while (cellWidth8);
                    v91 = Srce;
                }
                if (v127 - v90 - width > 0)
                {
                    v95 = (char*)&v157[2 * (v90 + width) + v90 + width + v92];
                    v96 = &v91[2 * width - 3 + width + (_DWORD)pData];
                    cellWidth9 = v127 - v90 - width;
                    do
                    {
                        *v95 = *v96;
                        v95 += 3;
                        v96 -= 3;
                        --cellWidth9;
                    } while (cellWidth9);
                    v91 = Srce;
                    v92 = Memorya;
                }
                v91 += stride;
                v92 += v88;
                v44 = v145-- == 1;
                Memorya = v92;
                Srce = v91;
            } while (!v44);
        }
        if (v89 && v127 > 0)
        {
            v97 = v133 - v89 - height;
            Dsth = (char*)v127;
            v162 = v133 - v89 - height;
            do
            {
                if (v89 > 0)
                {
                    v98 = v157;
                    v99 = (char*)&v157[v88 * (2 * v89 - 1)];
                    cellHeight0 = v89;
                    do
                    {
                        cellWidth0 = *v99;
                        v99 -= v88;
                        *v98 = cellWidth0;
                        v98 += v88;
                        --cellHeight0;
                    } while (cellHeight0);
                    v97 = v162;
                }
                if (v97 > 0)
                {
                    cellWidth1 = &v157[v88 * (v89 + height)];
                    cellWidth2 = &v157[v88 * (v89 + height - 1)];
                    cellHeight1 = v162;
                    do
                    {
                        *cellWidth1 = *cellWidth2;
                        cellWidth1 += v88;
                        cellWidth2 -= v88;
                        --cellHeight1;
                    } while (cellHeight1);
                    v89 = (v133 - height) / 2;
                    v97 = v162;
                }
                --Dsth;
            } while (Dsth);
        }
        AdaptHistEqualize(v157, v127, v133, v88, xValue, yValue, limit, bSeparate);
        if (height > 0)
        {
            cellWidth3 = (char*)&v157[3 * v90 + v88 * v89];
            cellWidth4 = (char*)pData;
            v165 = height;
            do
            {
                memcpy(cellWidth4, cellWidth3, stride);
                cellWidth4 += stride;
                cellWidth3 += v88;
                --v165;
            } while (v165);
        }
        _free(v157);
    }
    else
    {
        v12 = 2 * ((_BYTE)bSeparate != 1) + 1;
        v13 = (signed int)_ceil((float)((float)((float)((float)cellHeight * (float)cellWidth) * (float)v12) * 0.00390625));
        v163 = v13 + (signed int)((double)(v158 * cellHeight * v12 - v13) * *(double*)&limit);
        v14 = (char*)_malloc(3072 * yValue * xValue);
        Memory = v14;
        memset(v14, 0, 3072 * yValue * xValue);
        if ((_BYTE)bSeparate == 1)
        {
            if (yValue > 0)
            {
                v15 = 3072 * xValue;
                v16 = stride * cellHeight;
                v134 = (unsigned __int8*)pData;
                v17 = (int)(v14 + 1024);
                v137 = stride * cellHeight;
                v149 = v17;
                v153 = yValue;
                do
                {
                    if (xValue > 0)
                    {
                        v166 = 3 * v158;
                        v18 = v134;
                        v142 = v134;
                        v19 = v17;
                        Src = (_DWORD*)v17;
                        v146 = xValue;
                        do
                        {
                            v20 = v19 - 1024;
                            if (v152 > 0)
                            {
                                Dst = (char*)v152;
                                do
                                {
                                    v21 = v158;
                                    if (v158 > 0)
                                    {
                                        do
                                        {
                                            ++* (_DWORD*)(v20 + 4 * *v18);
                                            ++* (_DWORD*)(v19 + 4 * v18[1]);
                                            ++* (_DWORD*)(v19 + 4 * v18[2] + 1024);
                                            v18 += 3;
                                            --v21;
                                        } while (v21);
                                    }
                                    v18 += stride - v166;
                                    --Dst;
                                } while (Dst);
                            }
                            ClipHistGram(v163, v19 - 1024);
                            ClipHistGram(v163, v19);
                            ClipHistGram(v163, v19 + 1024);
                            v22 = 0;
                            v23 = 0;
                            v128 = 0;
                            v24 = 0;
                            v25 = (_DWORD*)(v20 + 8);
                            Dsta = (char*)64;
                            do
                            {
                                v22 += *(v25 - 2);
                                v24 += *(v25 - 1);
                                v23 += *v25;
                                v128 += v25[1];
                                v25 += 4;
                                --Dsta;
                            } while (Dsta);
                            v26 = 0;
                            v27 = (float)(v128 + v23 + v24 + v22);
                            v28 = 0;
                            do
                            {
                                v28 += *(_DWORD*)(v20 + 4 * v26);
                                *(_DWORD*)(v20 + 4 * v26++) = (signed int)(float)((float)((float)v28 * 255.0) / v27);
                            } while (v26 < 256);
                            v29 = 0;
                            v30 = 0;
                            v129 = 0;
                            v31 = 0;
                            v32 = Src + 2;
                            Dstb = (char*)64;
                            do
                            {
                                v29 += *(v32 - 2);
                                v31 += *(v32 - 1);
                                v30 += *v32;
                                v129 += v32[1];
                                v32 += 4;
                                --Dstb;
                            } while (Dstb);
                            v33 = v129 + v30 + v31;
                            v34 = 0;
                            v35 = 0;
                            v36 = (float)(v33 + v29);
                            v37 = 0;
                            do
                            {
                                v37 += Src[v35];
                                Src[v35++] = (signed int)(float)((float)((float)v37 * 255.0) / v36);
                            } while (v35 < 256);
                            v38 = 0;
                            v130 = 0;
                            v39 = 0;
                            v40 = Src + 258;
                            Dstc = (char*)64;
                            do
                            {
                                v38 += *(v40 - 2);
                                v34 += *(v40 - 1);
                                v39 += *v40;
                                v130 += v40[1];
                                v40 += 4;
                                --Dstc;
                            } while (Dstc);
                            v41 = 0;
                            v42 = (float)(v130 + v39 + v34 + v38);
                            v43 = 0;
                            do
                            {
                                v43 += Src[v41 + 256];
                                Src[v41++ + 256] = (signed int)(float)((float)((float)v43 * 255.0) / v42);
                            } while (v41 < 256);
                            v18 = &v142[v166];
                            v19 = (int)(Src + 768);
                            v44 = v146-- == 1;
                            Src += 768;
                            v142 += v166;
                        } while (!v44);
                        v15 = 3072 * xValue;
                        v16 = v137;
                        cellHeight = v152;
                    }
                    v134 += v16;
                    v17 = v15 + v149;
                    v44 = v153-- == 1;
                    v149 += v15;
                } while (!v44);
            }
        }
        else if (yValue > 0)
        {
            v45 = xValue << 10;
            v46 = stride * cellHeight;
            v135 = (char*)pData;
            v47 = (int)v14;
            v138 = stride * cellHeight;
            v147 = v14;
            v150 = yValue;
            do
            {
                if (xValue > 0)
                {
                    v48 = v158;
                    v49 = v135;
                    v50 = 3 * v158;
                    v167 = 3 * v158;
                    Srca = v135;
                    v143 = xValue;
                    while (1)
                    {
                        if (cellHeight > 0)
                        {
                            do
                            {
                                if (v48 > 0)
                                {
                                    do
                                    {
                                        ++* (_DWORD*)(v47 + 4 * (unsigned __int8)*v49);
                                        ++* (_DWORD*)(v47 + 4 * (unsigned __int8)v49[1]);
                                        ++* (_DWORD*)(v47 + 4 * (unsigned __int8)v49[2]);
                                        v49 += 3;
                                        --v48;
                                    } while (v48);
                                    v48 = v158;
                                }
                                v49 += stride - v50;
                                --cellHeight;
                            } while (cellHeight);
                        }
                        ClipHistGram(v163, v47);
                        v51 = 0;
                        v52 = 0;
                        v131 = 0;
                        v53 = 0;
                        v54 = (_DWORD*)(v47 + 8);
                        Dstd = (char*)64;
                        do
                        {
                            v51 += *(v54 - 2);
                            v53 += *(v54 - 1);
                            v52 += *v54;
                            v131 += v54[1];
                            v54 += 4;
                            --Dstd;
                        } while (Dstd);
                        v55 = 0;
                        v56 = (float)(v131 + v52 + v53 + v51);
                        v57 = 0;
                        do
                        {
                            v57 += *(_DWORD*)(v47 + 4 * v55);
                            *(_DWORD*)(v47 + 4 * v55++) = (signed int)(float)((float)((float)v57 * 255.0) / v56);
                        } while (v55 < 256);
                        v50 = 3 * v158;
                        cellHeight = v152;
                        v49 = &Srca[v167];
                        v47 += 1024;
                        v44 = v143-- == 1;
                        Srca += v167;
                        if (v44)
                            break;
                        v48 = v158;
                    }
                    v45 = xValue << 10;
                    v46 = v138;
                }
                v135 += v46;
                v47 = (int)&v147[v45];
                v44 = v150-- == 1;
                v147 += v45;
            } while (!v44);
        }
        v58 = 0;
        v144 = (char*)pData;
        cellWidth6 = 0;
        if (yValue >= 0)
        {
            cellWidth5 = 3 * width;
            while (1)
            {
                if (v58)
                {
                    v59 = cellHeight;
                    if (v58 == yValue)
                    {
                        v60 = yValue - 1;
                        v59 >>= 1;
                        v61 = yValue - 1;
                    }
                    else
                    {
                        v60 = v58 - 1;
                        v61 = v58;
                    }
                }
                else
                {
                    v59 = (cellHeight + 1) >> 1;
                    v60 = 0;
                    v61 = 0;
                }
                v62 = 0;
                v148 = v59;
                v132 = 0;
                if (xValue >= 0)
                {
                    v63 = xValue * v60;
                    Dste = (void*)v63;
                    v139 = xValue * v61;
                    do
                    {
                        v64 = v158;
                        if (v62)
                        {
                            if (v62 == xValue)
                            {
                                v64 = v158 >> 1;
                                v65 = xValue - 1;
                                v62 = xValue - 1;
                            }
                            else
                            {
                                v65 = v62 - 1;
                            }
                        }
                        else
                        {
                            v64 = (v158 + 1) >> 1;
                            v65 = 0;
                        }
                        v66 = v63 + v65;
                        v67 = v139 + v65;
                        v68 = v62 + v63;
                        v69 = v139 + v62;
                        v136 = v64;
                        if ((_BYTE)bSeparate)
                        {
                            v70 = 3072 * v66;
                            v71 = 3072 * v68;
                            v72 = 3072 * v67;
                            v73 = 3072 * v69;
                        }
                        else
                        {
                            v70 = v66 << 10;
                            v71 = v68 << 10;
                            v72 = v67 << 10;
                            v73 = v69 << 10;
                        }
                        v74 = (int)&Memory[v70];
                        v75 = (int)&Memory[v71];
                        v59 = v148;
                        v154 = (int)&Memory[v72];
                        v151 = v148 * v64;
                        v76 = (unsigned __int8*)v144;
                        v155 = (int)&Memory[v73];
                        v168 = v75;
                        v156 = (_DWORD*)v74;
                        v164 = 0;
                        v159 = v148;
                        if ((_BYTE)bSeparate == 1)
                        {
                            if (v148 > 0)
                            {
                                Srcb = 3 * v64;
                                do
                                {
                                    v77 = 0;
                                    v160 = v64;
                                    if (v64 > 0)
                                    {
                                        do
                                        {
                                            v78 = v76[2] + 512;
                                            v79 = (v164 * (v160 * *(_DWORD*)(v154 + 4 * *v76) + v77 * *(_DWORD*)(v155 + 4 * *v76))
                                                + v159 * (v160 * *(_DWORD*)(v74 + 4 * *v76) + v77 * *(_DWORD*)(v75 + 4 * *v76)))
                                                / v151;
                                            v80 = v76[1] + 256;
                                            v76 += 3;
                                            *(v76 - 3) = v79;
                                            *(v76 - 2) = (v164 * (v160 * *(_DWORD*)(v154 + 4 * v80) + v77 * *(_DWORD*)(v155 + 4 * v80))
                                                + v159 * (v160 * v156[v80] + v77 * *(_DWORD*)(v168 + 4 * v80)))
                                                / v151;
                                            v81 = (v164 * (v160 * *(_DWORD*)(v154 + 4 * v78) + v77 * *(_DWORD*)(v155 + 4 * v78))
                                                + v159 * (v160 * v156[v78] + v77 * *(_DWORD*)(v168 + 4 * v78)))
                                                / v151;
                                            v75 = v168;
                                            ++v77;
                                            --v160;
                                            v74 = (int)v156;
                                            *(v76 - 1) = v81;
                                            v64 = v136;
                                        } while (v77 < v136);
                                    }
                                    v59 = v148;
                                    ++v164;
                                    --v159;
                                    v76 += stride - Srcb;
                                } while (v164 < v148);
                            }
                        }
                        else if (v148 > 0)
                        {
                            Srcc = 3 * v64;
                            do
                            {
                                v82 = 0;
                                v161 = v64;
                                if (v64 > 0)
                                {
                                    do
                                    {
                                        v83 = (v164 * (v161 * *(_DWORD*)(v154 + 4 * *v76) + v82 * *(_DWORD*)(v155 + 4 * *v76))
                                            + v159 * (v161 * *(_DWORD*)(v74 + 4 * *v76) + v82 * *(_DWORD*)(v75 + 4 * *v76)))
                                            / v151;
                                        v84 = v76[1];
                                        v76 += 3;
                                        *(v76 - 3) = v83;
                                        v85 = (v164 * (v161 * *(_DWORD*)(v154 + 4 * v84) + v82 * *(_DWORD*)(v155 + 4 * v84))
                                            + v159 * (v161 * v156[v84] + v82 * *(_DWORD*)(v168 + 4 * v84)))
                                            / v151;
                                        v86 = *(v76 - 1);
                                        *(v76 - 2) = v85;
                                        v87 = (v164 * (v161 * *(_DWORD*)(v154 + 4 * v86) + v82 * *(_DWORD*)(v155 + 4 * v86))
                                            + v159 * (v161 * v156[v86] + v82 * *(_DWORD*)(v168 + 4 * v86)))
                                            / v151;
                                        v75 = v168;
                                        ++v82;
                                        --v161;
                                        v74 = (int)v156;
                                        *(v76 - 1) = v87;
                                        v64 = v136;
                                    } while (v82 < v136);
                                }
                                v59 = v148;
                                ++v164;
                                --v159;
                                v76 += stride - Srcc;
                            } while (v164 < v148);
                        }
                        v63 = (int)Dste;
                        v62 = v132 + 1;
                        v144 += 2 * v64 + v64;
                        v132 = v62;
                    } while (v62 <= xValue);
                    v58 = cellWidth6;
                }
                ++v58;
                v144 += stride * v59 - cellWidth5;
                cellWidth6 = v58;
                if (v58 > yValue)
                    break;
                cellHeight = v152;
            }
        }
        _free(Memory);
    }
}