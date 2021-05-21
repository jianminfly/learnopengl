#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform vec2      samplingRadius;	// 采样在 xy 两方向的像素偏移半径.
// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

float strength = 0.4;

void main()
{
	vec4 color = texture2D(texture1, TexCoord);
	float r = color.r * 255.0;
	float g = color.g * 255.0;
	float b = color.b * 255.0;
	float alpha = 0.0;
	if (color.a < 0.1)
	{
		alpha = 0.0;
	}
	else
	{
		if (g > b && g > r)
		{
			alpha = g * 2.0 - (r + b);
			alpha = 255.0 - alpha;
		}
		else
		{
			alpha = 255.0;
		}
		if (alpha > 255.0)
		{
			alpha = 255.0;
		}
		if (alpha < 1.0)
		{
			alpha = 1.0;
		}
		float diffrb = (color.b + color.r) / 2.0;
		if (color.g > diffrb)
		{
			color.g = (color.r + color.b) / 2.0;
		}
	}
	alpha = alpha / 255.0;
	float alphaGain = 255.0 * pow(alpha, 1.25 / strength);
	float alphaContrast = 0.0;
	alphaContrast = alphaGain + (alphaGain - 200.0) * strength;
	float alphaBrightness = alphaContrast + 255.0 * pow(alphaContrast / 256.0, 1.0 / strength);
	alpha = alphaBrightness / 255.0;
	if (alpha > 0.9)
	{
		alpha = 1.0;
	}
	if (alpha < 0.05)
	{
		alpha = 0.0;
	}
	FragColor = vec4(color.r, color.g, color.b, alpha);
}