#version 330 core

#ifdef GL_ES
precision mediump float;
#endif

in vec2 TexCoord;	// 采样坐标.
out vec4 fragColor;		// 输出的颜色.

vec2      iResolution = vec2(1280, 720);		// 输出图像的尺寸, 即画布尺寸.
uniform sampler2D texture1;		// 输入待调色的图片对应的纹理.
uniform vec2      samplingRadius;	// 采样在 xy 两方向的像素偏移半径.

/** 进行高斯模糊透明度采样. */
float gaussBlurAlphaSampling() {
    float alpha = 0.0;
	float samplingCount = 32.0;
	vec2 d = samplingRadius / iResolution / samplingCount;

	// 进行 32 + 1 采样.
	alpha += texture2D(texture1, TexCoord - 16.0 * d).a * 0.0138;
	alpha += texture2D(texture1, TexCoord - 15.0 * d).a * 0.0158;
	alpha += texture2D(texture1, TexCoord - 14.0 * d).a * 0.018;
	alpha += texture2D(texture1, TexCoord - 13.0 * d).a * 0.0203;
	alpha += texture2D(texture1, TexCoord - 12.0 * d).a * 0.0226;
	alpha += texture2D(texture1, TexCoord - 11.0 * d).a * 0.025;
	alpha += texture2D(texture1, TexCoord - 10.0 * d).a * 0.0274;
	alpha += texture2D(texture1, TexCoord - 9.0 * d).a * 0.0298;
	alpha += texture2D(texture1, TexCoord - 8.0 * d).a * 0.0321;
	alpha += texture2D(texture1, TexCoord - 7.0 * d).a * 0.0343;
	alpha += texture2D(texture1, TexCoord - 6.0 * d).a * 0.0364;
	alpha += texture2D(texture1, TexCoord - 5.0 * d).a * 0.0382;
	alpha += texture2D(texture1, TexCoord - 4.0 * d).a * 0.0397;
	alpha += texture2D(texture1, TexCoord - 3.0 * d).a * 0.0409;
	alpha += texture2D(texture1, TexCoord - 2.0 * d).a * 0.0418;
	alpha += texture2D(texture1, TexCoord - 1.0 * d).a * 0.0424;
	alpha += texture2D(texture1, TexCoord + 0.0 * d).a * 0.0426;
	alpha += texture2D(texture1, TexCoord + 1.0 * d).a * 0.0424;
	alpha += texture2D(texture1, TexCoord + 2.0 * d).a * 0.0418;
	alpha += texture2D(texture1, TexCoord + 3.0 * d).a * 0.0409;
	alpha += texture2D(texture1, TexCoord + 4.0 * d).a * 0.0397;
	alpha += texture2D(texture1, TexCoord + 5.0 * d).a * 0.0382;
	alpha += texture2D(texture1, TexCoord + 6.0 * d).a * 0.0364;
	alpha += texture2D(texture1, TexCoord + 7.0 * d).a * 0.0343;
	alpha += texture2D(texture1, TexCoord + 8.0 * d).a * 0.0321;
	alpha += texture2D(texture1, TexCoord + 9.0 * d).a * 0.0298;
	alpha += texture2D(texture1, TexCoord + 10.0 * d).a * 0.0274;
	alpha += texture2D(texture1, TexCoord + 11.0 * d).a * 0.025;
	alpha += texture2D(texture1, TexCoord + 12.0 * d).a * 0.0226;
	alpha += texture2D(texture1, TexCoord + 13.0 * d).a * 0.0203;
	alpha += texture2D(texture1, TexCoord + 14.0 * d).a * 0.018;
	alpha += texture2D(texture1, TexCoord + 15.0 * d).a * 0.0158;
	alpha += texture2D(texture1, TexCoord + 16.0 * d).a * 0.0138;

    return alpha;
}

vec4 getFragColor() {
    vec4 color = texture2D(texture1, TexCoord);
    float alpha = gaussBlurAlphaSampling();

	// 实际上只保留 alpha 通道来作为下一次的输入.
    return vec4(color.rgb*alpha, alpha);
}

void main() {
	fragColor = getFragColor();
}