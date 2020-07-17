#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform float iProgress;

int num = 2;
vec4 rect[2];
rect[0] = vec4(0, 0, 0.5, 0.5);
rect[1] = vec4(0, 0, 0.5, 0.5);

void main()
{
	gl_Position = vec4(aPos, 0, 1);
	
	float subProgress = iProgress - float(num) / float(iCount);
	
	if(aTexCoord == vec2(1,1))
	TexCoord =  mix(aTexCoord , vec2(rect.xy + rect.zw), iProgress);
	else if(aTexCoord == vec2(1,0))
	TexCoord =  mix(aTexCoord , vec2(rect.x+rect.z, rect.y), iProgress);
	else if(aTexCoord == vec2(0,1))
	TexCoord =  mix(aTexCoord , vec2(rect.x, rect.y+rect.w), iProgress);
	else if(aTexCoord == vec2(0,0))
	TexCoord =  mix(aTexCoord , vec2(rect.xy), iProgress);
}