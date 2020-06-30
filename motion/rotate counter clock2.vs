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
			 cos(x), sin(x), 0, 0,
		     -sin(x), cos(x), 0, 0,
			 0, 0, 1, 0,
			 0, 0, 0, 1
		)*m_mat;
	}

void main()
{
	vec2 uv = aPos/2;
	uv.y /= aspect0;// 旋转矩阵是针对实际坐标的，所以当宽高不等时，因为会标准化到相同的0-1区间，再和旋转矩阵进行计算就会出现误差，导致变形。
					//因此需要把x,y的坐标根据宽高比做拓展，运算完成后再归一化到0-1
	uv = vec4(scale(initmat, radians(iStartAngle + iProgress * iSpeed), radians(iStartAngle + iProgress * iSpeed)) * vec4(uv, 0, 1)).xy;
	uv.y *= aspect0;
	
	gl_Position = vec4(uv, 0, 1);
	
	TexCoord =  aTexCoord;
}