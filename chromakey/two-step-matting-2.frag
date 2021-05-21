// 用于对已估计出的图像进行边缘杂色移除.

#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

// 输入的纹理图像的尺寸.
vec2 iResolution = vec2(1280.0, 720.0);

// 进行高斯加权采样时的像素半径, 默认为 8, 只需要保证边缘足够顺滑即可.
float samplingRadius = 8.0;

// 进行高斯加权采样时的颜色和背景色的差值的次方.
float weightedColorDiffPower = 4.0;

// 背景均色.
uniform vec3 averageBackground = vec3(0.0, 1.0, 0.0);

// 是否为蓝屏抠图.
bool isBlueScreen = false;

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

// 获取线性系数.
float getLinearFactor(float value, float from, float to) {
	return (value - from) / (to - from);
}

// 根据线性系数进行插值.
float getValueByLinearFactor(float factor, float from, float to) {
	return (to - from) * factor + from;
}

// 猜测和重新调整颜色中的绿色分量.
vec3 adjustBackgroundChannel(vec3 currentColor, vec3 neighborColor) {

	// Spill 处理, 将所有的绿色抹除, 包括边缘和反光产生的绿色.
	// 它利用了 rb 通道的平滑来使得 g 通道被修正的同时也保持平滑.
	if (!isBlueScreen) {
		// 当绿色通道不是最小通道.
		if (currentColor.g > min(currentColor.r, currentColor.b)) {
			
			// 获取邻近色绿色通道在红蓝通道中的线性位置.
			// 注意这个参数非常容易溢出, 所以需要进行限制到 -1~1.
			// 它相当于某种最大最小值滤波.
			float neighborGreenFactor = getLinearFactor(neighborColor.g, neighborColor.r, neighborColor.b);
			neighborGreenFactor = clamp(neighborGreenFactor, -1.0, 1.0);

			// 将当前颜色的绿色通道插值, 获得是结果是色相和邻近色最接近的绿色.
			// 本质上其实是搬运 HSL 颜色空间中的色相.
			float newGreen = getValueByLinearFactor(neighborGreenFactor, currentColor.r, currentColor.b);
			newGreen = max(newGreen, min(currentColor.r, currentColor.b));
			newGreen = min(newGreen, max(currentColor.r, currentColor.b));
			
			// 最大的绿色分量被限制到红蓝的中心.
			float centralGreen = (currentColor.r + currentColor.b) / 2.0;

			// 根据最大可分解为背景色的大小, 即背景色的最大比例进行加权混合.
			// 会处理蓝绿色, 因为背景多包含蓝色, 即使绿色比重降低到和蓝色也差别不大.
			// 但是不会处理亮黄色, 要不然会有非常明显的偏色.
			if (currentColor.r >= currentColor.b) {
				float redBlueDiffRate = 1.0 - linearStep(currentColor.r - currentColor.b, 0.2, 0.5);
				centralGreen = mix(currentColor.g, centralGreen, redBlueDiffRate);
			}

			// 取以上二者的最小值.
			newGreen = min(newGreen, centralGreen);

			// 最大不能超过红蓝通道的最大值.
			currentColor.g = newGreen;
		}
	}

	// 如果仅使用绿色通道, 或者为了性能更加高效, 可以考虑改写为两个着色器,
	// 一个只处理绿色, 一个只处理蓝色, CPU 有分支预测, 但是 GPU 没有, 所有分支代码都会全部运行.
	else {
		if (currentColor.b > min(currentColor.r, currentColor.g)) {
			float neighborBlueFactor = getLinearFactor(neighborColor.b, neighborColor.r, neighborColor.g);
			neighborBlueFactor = clamp(neighborBlueFactor, -1.0, 1.0);
			float newBlue = getValueByLinearFactor(neighborBlueFactor, currentColor.r, currentColor.g);
			newBlue = min(newBlue, max(currentColor.r, currentColor.g));
			newBlue = max(newBlue, min(currentColor.r, currentColor.g));
			float centralBlue = (currentColor.r + currentColor.g) / 2.0;
			if (currentColor.r >= currentColor.g) {
				float redGreenDiffRate = 1.0 - linearStep(currentColor.r - currentColor.g, 0.2, 0.5);
				centralBlue = mix(currentColor.b, centralBlue, redGreenDiffRate);
			}
			newBlue = min(newBlue, centralBlue);
			currentColor.b = newBlue;
		}
	}
	
	return currentColor;
}

// 进行加权高斯前景色采样.
vec4 getWeightedFgColor() {
	// 获取当前颜色.
	vec4 currentColor = texture2D(texture1, TexCoord);

	vec3 totalColor = vec3(0.0);
	float totalColorWeight = 0.0;
	float neighborAlpha = 0.0;
	vec2 d = samplingRadius / iResolution / 2.0;

	for (int i = 0; i < 25; i++) {
		vec3 xyp = guassianXYP[i];

		// 注意此颜色为非预乘色.
		vec4 color = texture(texture1, TexCoord + vec2(xyp.x, xyp.y) * d);
		float alpha = color.a;

		// 临近颜色和背景色的远离程度决定了加权的比例.
		float colorWeight = pow(distance(color.rgb, averageBackground), weightedColorDiffPower) * alpha * xyp.z;

		// 高斯权重决定了透明度的权重.
		float alphaWeight = xyp.z;

		totalColor += color.rgb * colorWeight;
		totalColorWeight += colorWeight;
		neighborAlpha += alpha * alphaWeight;
	}

	// 查询附近比较临近的前景色.
	vec3 neighborColor = totalColor / totalColorWeight;

	// 调整绿色通道.
	currentColor.rgb = adjustBackgroundChannel(currentColor.rgb, neighborColor);
	//currentColor.rgb *= currentColor.a;

	return currentColor;
}

void main() {
	vec4 c = getWeightedFgColor();
	gl_FragColor = vec4(mix(texture2D(texture2, TexCoord).rgb, c.rgb, c.a), 1);
}









