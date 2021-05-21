// 参考:
//   https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.37.7740&rep=rep1&type=pdf
//   http://graphics.stanford.edu/courses/cs148-09-fall/papers/smith-blinn.pdf
// 未采用经常被推荐使用 Chroma Key Matting, 因为它和亮度总体不相关, 因此在 CbCr / UV 的空间的过渡区域往往非常小, 生成的 Alpha 的梯度变化会大幅度增大, 从而会产生许多的噪点.
// 而色差键的特色就是生成的 Alpha 平滑, 在边界过渡的地方向外部延伸过程中, 这个差值在不断变大. 此变化关系刚好符合 alpha 的线性变化.

#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform vec2      samplingRadius;	// 采样在 xy 两方向的像素偏移半径.
// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

// 输入的纹理图像的尺寸.
vec2 iResolution = vec2(1280.0, 720.0);

// 绿色和红色通道的均值差达到此值时, 开始将透明度从 0 过渡到 1.
// 取值范围大多介于 0.5~0.8 之间. 值越大, 被抠除的内容越多.

// 如何自动估计此阈值:
// 	   首先统计 `1.0 - (g - r) * 2` 的颜色分布.
//     产生的统计图是一个左侧 (背景) 和右侧 (前景) 分别很大, 中间是一个沟的图像.
//     选择沟底的中间即可.
uniform float colorDiffThreshold = 0.8;

// 是否为蓝屏抠图.
bool isBlueScreen = false;

// 背景均色.
uniform vec3 averageBackground = vec3(0.0, 1.0, 0.0);


// 线性插值, 根据 x 的范围返回 0~1 的结果.
float linearStep(float x, float min, float max) {
	if (x <= min) {
		return 0.0;
	} 
	else if (x >= max) {
		return 1.0;
	}
	else {
		return (x - min) / (max - min);
	}
}

// 高斯采样的坐标偏移和权重.
vec3 guassianXYP[25] = vec3[](
	vec3(-2.0, -2.0, 0.0106),
	vec3(-2.0, -1.0, 0.0246),
	vec3(-2.0,  0.0, 0.0325),
	vec3(-2.0,  1.0, 0.0246),
	vec3(-2.0,  2.0, 0.0106),
	vec3(-1.0, -2.0, 0.0246),
	vec3(-1.0, -1.0, 0.0571),
	vec3(-1.0,  0.0, 0.0756),
	vec3(-1.0,  1.0, 0.0571),
	vec3(-1.0,  2.0, 0.0246),
	vec3( 0.0, -2.0, 0.0325),
	vec3( 0.0, -1.0, 0.0756),
	vec3( 0.0,  0.0, 0.0756),
	vec3( 0.0,  1.0, 0.0756),
	vec3( 0.0,  2.0, 0.0325),
	vec3( 1.0, -2.0, 0.0246),
	vec3( 1.0, -1.0, 0.0571),
	vec3( 1.0,  0.0, 0.0756),
	vec3( 1.0,  1.0, 0.0571),
	vec3( 1.0,  2.0, 0.0246),
	vec3( 2.0, -2.0, 0.0106),
	vec3( 2.0, -1.0, 0.0246),
	vec3( 2.0,  0.0, 0.0325),
	vec3( 2.0,  1.0, 0.0246),
	vec3( 2.0,  2.0, 0.0106)
);

// 进行 5x5 范围的高斯模糊采样.
// 它可以明显加强小的细节例如文字细节边缘的平滑度.
vec4 getGaussianBlurredColor() {
	vec4 blurredColor = vec4(0.0);
	vec2 d = 1.0 / iResolution;

	for (int i = 0; i < 25; i++) {
		vec3 xyp = guassianXYP[i];
		vec4 color = texture(texture1, TexCoord + vec2(xyp.x, xyp.y) * d);
		blurredColor += color * xyp.z;
	}

	return blurredColor;
}

// 获取当前颜色可以分解出的最大的背景色比重.
float getMaxBackgroundRate(vec3 color) {
	vec3 rates1 = color / (averageBackground + vec3(0.0001));
	vec3 rates2 = (1.0 - color) / (1.0 - averageBackground + vec3(0.0001));

	float rate = min(min(rates1.r, rates1.g), rates1.b);
	rate = min(min(min(rates2.r, rates2.g), rates2.b), rate);

	return rate;
}

void main() {

	// 获取当前颜色.
	vec4 color = texture(texture1, TexCoord);

	// 获取经过了高斯模糊的颜色.
	vec4 blurredColor = getGaussianBlurredColor();

	float r = blurredColor.r;
	float g = blurredColor.g;
	float b = blurredColor.b;

	// 获得绿色通道和另外两个通道均值的差.
	// 它的整体量化范围为 0~1, 并且和最后的透明度正相关.
	float colorDiff = isBlueScreen
		? 1.0 - 2 * (b - max(r, g))
		: 1.0 - 2 * (g - max(r, b));

	// 线性插值获得两个通道差下的 Alpha.
	float alpha = linearStep(colorDiff, colorDiffThreshold, 1.0);

	// 能够最大分解为多少比例的背景色.
	float maxBackgroundRate = getMaxBackgroundRate(blurredColor.rgb);

	// 将所有的绿色抹除, 包括边缘和反光产生的绿色.
	// 它利用了 rb 通道的平滑来使得 g 通道被修正的同时也会变得平滑.
	if (isBlueScreen) {
		if (color.b > (color.r + color.g) / 2.0) {
			float newB = (color.r + color.g) / 2.0;
			float maxRate = linearStep(maxBackgroundRate, 0.2, 0.5);
			color.b = mix(color.b, newB, maxRate);
		}
	}
	else {
		if (color.g > (color.r + color.b) / 2.0) {
			float newG = (color.r + color.b) / 2.0;
			float maxRate = linearStep(maxBackgroundRate, 0.2, 0.5);
			color.g = mix(color.g, newG, maxRate);
		}
	}

	// 注意这里输出的是预乘色, 如果希望输出非预乘色, 应当为 color.a *= ...
	color *= alpha;
	gl_FragColor = vec4(mix(texture2D(texture2, TexCoord).rgb, color.rgb, color.a), 1);
}