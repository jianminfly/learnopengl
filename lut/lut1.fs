#version 330 core

uniform sampler2D texture2;
uniform sampler2D texture1;
float intensity = 0.5;

in vec2 TexCoord;	// 采样坐标.
out vec4 fragColor;		// 输出的颜色.

float celllen = 17;


void main() {

	vec4 textureColor = texture2D(texture1, TexCoord);
    vec4 conColor = textureColor * (celllen - 1.);
    	
	float fy = floor(conColor.b) * 1./17. + conColor.g * 1./(17.*17.);
	float fx = conColor.r * 1./17.;
	
	//计算8个点和对应权重
	vec2 coords[8];
	vec3 weight[8];
	coords[0].x = floor(conColor.r) * 1./17.;
	coords[0].y = ceil(conColor.b) * 1./17. + ceil(conColor.g) * 1./(17.*17.);
	weight[0].r = 1. - fract(conColor.r);
	weight[0].g = fract(conColor.g);
	weight[0].b = fract(conColor.b);
	
	coords[1].x = ceil(conColor.r) * 1./17.;
	coords[1].y = ceil(conColor.b) * 1./17. + ceil(conColor.g) * 1./(17.*17.);
	weight[1].r = fract(conColor.r);
	weight[1].g = fract(conColor.g);
	weight[1].b = fract(conColor.b);

	coords[2].x = floor(conColor.r) * 1./17.;
	coords[2].y = ceil(conColor.b) * 1./17. + floor(conColor.g) * 1./(17.*17.);
	weight[2].r = 1. - fract(conColor.r);
	weight[2].g = 1. - fract(conColor.g);
	weight[2].b = fract(conColor.b);

	coords[3].x = ceil(conColor.r) * 1./17.;
	coords[3].y = ceil(conColor.b) * 1./17. + floor(conColor.g) * 1./(17.*17.);
	weight[3].r = fract(conColor.r);
	weight[3].g = 1. - fract(conColor.g);
	weight[3].b = fract(conColor.b);

	coords[4].x = floor(conColor.r) * 1./17.;
	coords[4].y = floor(conColor.b) * 1./17. + ceil(conColor.g) * 1./(17.*17.);
	weight[4].r = 1. - fract(conColor.r);
	weight[4].g = fract(conColor.g);
	weight[4].b = 1.- fract(conColor.b);
	
	coords[5].x = ceil(conColor.r) * 1./17.;
	coords[5].y = floor(conColor.b) * 1./17. + ceil(conColor.g) * 1./(17.*17.);
	weight[5].r = fract(conColor.r);
	weight[5].g = fract(conColor.g);
	weight[5].b = 1.- fract(conColor.b);

	coords[6].x = floor(conColor.r) * 1./17.;
	coords[6].y = floor(conColor.b) * 1./17. + floor(conColor.g) * 1./(17.*17.);
	weight[6].r = 1. - fract(conColor.r);
	weight[6].g = 1. - fract(conColor.g);
	weight[6].b = 1.- fract(conColor.b);

	coords[7].x = ceil(conColor.r) * 1./17.;
	coords[7].y = floor(conColor.b) * 1./17. + floor(conColor.g) * 1./(17.*17.);
	weight[7].r = fract(conColor.r);
	weight[7].g = 1. - fract(conColor.g);
	weight[7].b = 1.- fract(conColor.b);


	vec4 colors[8];
	for(int i = 0; i< 8; i++)
		colors[i] = texture2D(texture2, coords[i]);
		
	vec4 newColor1 = mix(mix(mix(colors[0], colors[1], weight[1].r), mix(colors[2], colors[3], weight[3].r), weight[3].g), mix(mix(colors[4], colors[5], weight[5].r), mix(colors[6], colors[7], weight[7].r), weight[7].g),weight[7].b);
	

	fragColor = newColor1;
}
