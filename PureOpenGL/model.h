#pragma once
#include "camera.h"
#include "shader.h"
#include "function.h"

namespace PureOpenGL
{
	class Model
	{
	public:	// �麯��
		virtual void Draw(){};							// ���ƽӿڣ���д�����������
		virtual void bindShader(Shader * shader);
		virtual void bindCamera(Camera * camera);
		virtual void setKey(int k, bool f);
		virtual glm::vec3 getPixelPos(int x, int y);	// ������Ļλ�û����άλ��

	public:	// ���ؽӿڵĺ���
		
		glm::vec3 getBoxMin();
		glm::vec3 getBoxMax();
		Camera * getCamera();
		Shader * getShader();
		float getMaxEdge();
		void setBoxMax(glm::vec3 bm);
		void setBoxMin(glm::vec3 bm);
		glm::vec3 getBoxCenter();		
	public:
		Model();
		~Model();
		void saveImage();		// ����ͼƬ
		void setLastPos(glm::ivec2 p);			// ���õ����λ��
	protected:
		Camera * camera;
		Shader * shader;		
		bool key[256];			// �����¼�
		glm::vec2 lastPos;		// ���λ��
	protected:
		// ģ�ͱ�����Ϣ
		glm::vec3 box_min = glm::vec3(1e10);
		glm::vec3 box_max = glm::vec3(-1e10);
		float max_edge;
	};
}