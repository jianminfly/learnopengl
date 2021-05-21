#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform vec2      samplingRadius;	// 采样在 xy 两方向的像素偏移半径.
// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

// 高斯模糊权重.
const vec2 guassBlurPriority[33] = vec2[](
	vec2(-16.0, 0.0138),
	vec2(-15.0, 0.0158),
	vec2(-14.0, 0.018),
	vec2(-13.0, 0.0203),
	vec2(-12.0, 0.0226),
	vec2(-11.0, 0.025),
	vec2(-10.0, 0.0274),
	vec2(-9.0, 0.0298),
	vec2(-8.0, 0.0321),
	vec2(-7.0, 0.0343),
	vec2(-6.0, 0.0364),
	vec2(-5.0, 0.0382),
	vec2(-4.0, 0.0397),
	vec2(-3.0, 0.0409),
	vec2(-2.0, 0.0418),
	vec2(-1.0, 0.0424),
	vec2(0.0, 0.0426),
	vec2(1.0, 0.0424),
	vec2(2.0, 0.0418),
	vec2(3.0, 0.0409),
	vec2(4.0, 0.0397),
	vec2(5.0, 0.0382),
	vec2(6.0, 0.0364),
	vec2(7.0, 0.0343),
	vec2(8.0, 0.0321),
	vec2(9.0, 0.0298),
	vec2(10.0, 0.0274),
	vec2(11.0, 0.025),
	vec2(12.0, 0.0226),
	vec2(13.0, 0.0203),
	vec2(14.0, 0.018),
	vec2(15.0, 0.0158),
	vec2(16.0, 0.0138)
);

/** 进行高斯模糊透明度采样. */
vec4 gaussBlurSampling() {
	vec4 color;
	float samplingCount = 32.0;
	vec2 d = samplingRadius / vec2(1280.0,720.0) / samplingCount;

	for (int i = 0; i < 33; i++) {
		vec2 priority = guassBlurPriority[i];
		color += texture2D(texture1, TexCoord - priority.x * d) * priority.y;
	}

    return color /= color.a;
}

vec4 getFragColor() {

	vec4 res_color = texture2D(texture1, TexCoord);
	
	vec4 color = texture2D(texture2, TexCoord);
	if(all(greaterThan(color.rgb, vec3(0.9,0.9,0.9)))){
		res_color = gaussBlurSampling();
	}
    return res_color;
}

void main() {
	FragColor = getFragColor();
}
















