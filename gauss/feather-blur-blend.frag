#version 330 core

#ifdef GL_ES
precision mediump float;
#endif

in vec2 TexCoord;	// 采样坐标.
out vec4 fragColor;		// 输出的颜色.

vec2      iResolution = vec2(1280, 720);		// 输出图像的尺寸, 即画布尺寸.
uniform sampler2D texture1;		// 输入待调色的图片对应的纹理.
uniform sampler2D texture2;		// 输入待调色的图片对应的纹理.

vec4 getFragColor() {
    vec4 color = texture2D(texture1, TexCoord);
	float alpha = texture2D(texture2, TexCoord).a;

	// 边缘一般情况下获得 alpha 值为 0.5, 此时我们将 0.5-1 的范围拉长为 0~1 的范围.
    return color * (alpha * 2.0 - 1.0);
}

void main() {
	fragColor = getFragColor();
}