#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform vec2      samplingRadius;	// 采样在 xy 两方向的像素偏移半径.
// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

lowp float greenPercent = 1.2;


void main()
{
	vec4 c = texture2D(texture1, TexCoord);
	FragColor = c;
	return;
	
	float alpha = c.a;
	int scale = 1;
	for (int i = -scale; i <= scale && alpha < 1; ++i) {
		for (int j = -scale; j <= scale && alpha < 1; j++) {
			alpha +=  texture2D(texture1, TexCoord + vec2(i / 1280.0, j / 720.0)).a;
		}
	}
	c.a = alpha;
	FragColor = c;
}