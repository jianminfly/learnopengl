#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform vec2      samplingRadius;	// 采样在 xy 两方向的像素偏移半径.
// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
	vec4 c = texture2D(texture1, TexCoord);
	
	float alpha = c.a;
	int scale = 1;
	for (int i = -scale; i <= scale && alpha > 0; ++i) {
		for (int j = -scale; j <= scale && alpha > 0; j++) {
			alpha -= 1 - texture2D(texture1, TexCoord + vec2(float(i) / 1280.0, float(j) / 720.0)).a;
		}
	}
	c.a = alpha;
	FragColor = c;
}
