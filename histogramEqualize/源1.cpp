#include<cstdio>
#include<cassert>
#include<cstring>
#include<math.h>
#include<time.h>
#include<Windows.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
using namespace std;
unsigned char* imageA = NULL;
unsigned char* imageB = NULL;
BITMAPFILEHEADER* bmpFileHeader = NULL;
BITMAPINFOHEADER* bmpInfoHeader = NULL;
//对8位灰度图进行直方图均衡化
void histogramEqulize_8Gray(unsigned char* pimg, int biSizeImage) {
	unsigned char* pcur, * pend = pimg + biSizeImage;
	unsigned int hist[256], LUT[256];
	memset(hist, 0, sizeof(hist));
	for (pcur = pimg; pcur < pend; ++pcur) {
		hist[*pcur]++;
	}
	double sum = 0;
	for (int i = 0; i < 256; ++i) {
		sum += hist[i];
		LUT[i] = 255 * (sum / biSizeImage);
	}
	for (pcur = pimg; pcur < pend; ++pcur) {
		*(pcur) = LUT[*pcur];
	}
}
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

void gao() {
	FILE* fileA = NULL, * fileB = NULL;
	//////////////////////////////////////////////////////////////////////////
	//以下读取A.bmp的文件信息
	fileA = fopen("A.bmp", "rb");
	assert(fileA != NULL);
	//读取BMP文件头
	bmpFileHeader = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
	assert(bmpFileHeader != NULL);
	fread(bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fileA);
	//判断是否BMP格式图片
	if (bmpFileHeader->bfType != 0x4d42) {
		fclose(fileA);
		return;
	}
	//读取位图信息头
	bmpInfoHeader = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));
	fread(bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fileA);
	//只支持8位灰度图或24位真彩色图
	assert(bmpInfoHeader->biBitCount == 24);
	fseek(fileA, bmpFileHeader->bfOffBits, SEEK_SET);
	//分配内存给位图数据
	imageA = (unsigned char*)malloc(bmpInfoHeader->biSizeImage);
	assert(imageA != NULL);
	imageB = (unsigned char*)malloc(bmpInfoHeader->biSizeImage);
	assert(imageB != NULL);
	//读取位图数据
	fread(imageA, 1, bmpInfoHeader->biSizeImage, fileA);
	assert(imageA != NULL);
	memcpy(imageB, imageA, bmpInfoHeader->biSizeImage);
	///////////////////////////////////////////////////////////////////////////
	//直方图处理
	histogramEqualize_24RGB(imageB, bmpInfoHeader->biSizeImage);
	/////////////////////////////////////////////////////////////////////////
	//处理后输出为B.bmp
	fileB = fopen("B.bmp", "wb");
	assert(fileB != NULL);
	unsigned char* bmpHeader = (unsigned char*)malloc(bmpFileHeader->bfOffBits);
	assert(bmpHeader != NULL);
	fseek(fileA, SEEK_SET, 0);
	fread(bmpHeader, bmpFileHeader->bfOffBits, 1, fileA);
	fwrite(bmpHeader, bmpFileHeader->bfOffBits, 1, fileB);
	fwrite(imageB, bmpInfoHeader->biSizeImage, 1, fileB);
	////////////////////////////////////////////////////////////////////////
	free(bmpHeader);
	bmpHeader = NULL;
	fclose(fileA);
	fileA = NULL;
	fclose(fileB);
	fileB = NULL;
}
void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 800.0, 0.0, 400.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gao();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	if (bmpInfoHeader->biWidth >= bmpInfoHeader->biHeight) {
		glPixelZoom(400.0 / bmpInfoHeader->biWidth, 400.0 / bmpInfoHeader->biWidth);
	}
	else {
		glPixelZoom(400.0 / bmpInfoHeader->biHeight, 400.0 / bmpInfoHeader->biHeight);
	}
	glRasterPos2d(0, 0);
	glDrawPixels(bmpInfoHeader->biWidth, bmpInfoHeader->biHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, imageA);
	glRasterPos2d(400, 0);
	glDrawPixels(bmpInfoHeader->biWidth, bmpInfoHeader->biHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, imageB);
	glFlush();
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 400);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("直方图均衡化处理");
	init();
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}