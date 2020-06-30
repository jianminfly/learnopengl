#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform float iProgress;
uniform float aspect0;

const float iStartAngle = 0.0;
const float iEndAngle = 360.0;
float iSpeed = abs(iEndAngle - iStartAngle);

mat4 initmat = mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

mat4 scale( mat4 m_mat,float x, float y) {
		return mat4(
			 cos(x), -sin(x), 0, 0,
		     sin(x), cos(x), 0, 0,
			 0, 0, 1, 0,
			 0, 0, 0, 1
		)*m_mat;
	}

void main()
{
	vec2 uv = aPos;
	uv.x *= aspect0;
	uv /= aspect0;
	uv = vec4(scale(initmat, radians(iStartAngle + iProgress * iSpeed), radians(iStartAngle + iProgress * iSpeed)) * vec4(uv, 0, 1)).xy;
	uv.y *= aspect0;
	
	gl_Position = vec4(uv, 0, 1);
	
	TexCoord =  aTexCoord;
}