#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform vec2      samplingRadius;	// 采样在 xy 两方向的像素偏移半径.
// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;


void main()
{
	vec4 c = texture2D(texture1, TexCoord);
	FragColor = c;
	return;
    vec4 sample[9];
    vec4 minValue = vec4(1.0);
		
	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; j++) {
			sample[i] = texture2D(texture1, TexCoord + vec2(i / 1280.0, j / 720.0));
			
			minValue = min(sample[i], minValue);
		}
	}
	FragColor = minValue;
}