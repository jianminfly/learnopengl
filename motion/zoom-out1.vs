#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform float iProgress;

mat4 initmat = mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

mat4 scale( mat4 m_mat,float x, float y) {
		return mat4(
			 x, 0, 0, 0,
		     0, y, 0, 0,
			 0, 0, 1, 0,
			 0, 0, 0, 1
		)*m_mat;
	}

void main()
{
	gl_Position = scale(initmat, 1-iProgress, 1-iProgress) * vec4(aPos, 0, 1);
	TexCoord =  aTexCoord;
}