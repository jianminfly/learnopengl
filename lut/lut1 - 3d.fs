#version 330 core

uniform sampler3D texture2;
uniform sampler2D texture1;
float intensity = 0.5;

in vec2 TexCoord;	// 采样坐标.
out vec4 fragColor;		// 输出的颜色.

int celllen = 17;


void main() {

	vec4 srcColor = texture2D(texture1, TexCoord);
	
	vec4 newColor1 = texture3D(texture2, vec3(TexCoord,1));//srcColor.rgb);
		
	fragColor = newColor1;//texture2D(texture2, vec2(0,1));
}