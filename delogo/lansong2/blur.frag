#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform vec2      samplingRadius;	// 采样在 xy 两方向的像素偏移半径.
// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;


float widthRatio = 1.0 / (1280.0*2.0);
float heightRatio = 1.0 / (720.0*2.0);

void main()
{
	vec4 src = texture2D(texture1, TexCoord);
	vec4 sum = vec4(0.0);
	sum += texture2D(texture1, vec2(TexCoord.x - 4.0*widthRatio, TexCoord.y - 4.0*heightRatio)) * 0.0162162162;
	sum += texture2D(texture1, vec2(TexCoord.x - 3.0*widthRatio, TexCoord.y - 3.0*heightRatio)) * 0.0540540541;
	sum += texture2D(texture1, vec2(TexCoord.x - 2.0*widthRatio, TexCoord.y - 2.0*heightRatio)) * 0.1216216216;
	sum += texture2D(texture1, vec2(TexCoord.x - 1.0*widthRatio, TexCoord.y - 1.0*heightRatio)) * 0.1945945946;
	sum += texture2D(texture1, vec2(TexCoord.x, TexCoord.y)) * 0.2270270270;
	sum += texture2D(texture1, vec2(TexCoord.x + 1.0*widthRatio, TexCoord.y + 1.0*heightRatio)) * 0.1945945946;
	sum += texture2D(texture1, vec2(TexCoord.x + 2.0*widthRatio, TexCoord.y + 2.0*heightRatio)) * 0.1216216216;
	sum += texture2D(texture1, vec2(TexCoord.x + 3.0*widthRatio, TexCoord.y + 3.0*heightRatio)) * 0.0540540541;
	sum += texture2D(texture1, vec2(TexCoord.x + 4.0*widthRatio, TexCoord.y + 4.0*heightRatio)) * 0.0162162162;
	FragColor =  vec4(src.rgb, sum.a);
}