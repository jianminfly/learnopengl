#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace PureOpenGL
{
	std::ostream & operator<<(std::ostream & onfs, const glm::vec3 & v);
	std::ostream & operator<<(std::ostream & onfs, const glm::ivec3 & v);
	std::istream & operator>>(std::istream & infs, glm::vec3 & v);
	std::istream & operator>>(std::istream & infs, glm::ivec3 & v);

	// �����Դ·����ͷ
	std::string treatSourcePath(std::string path);

	// ���shader·����ͷ
	std::string treatShaderPath(std::string path);

	// ����vector<char>���������������ڶ����ݽ���һ���ԵĶ���
	std::ostream & operator<< (std::ostream & onfs, const std::vector<char> & v);	

	// ת�����ػ����ݵĸ�ʽ
	void transfer();

	void Error(std::string msg);

	// ��������
	int randomNumber(int range);

	// ��������ɫ
	glm::vec3 randomColor();

	// ����ͼƬ���ļ���
	std::string getImgName();

	std::pair<float, float> screen2NDC(int posx, int posy, int width, int height);
}