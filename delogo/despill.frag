#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform vec2      samplingRadius;	// 采样在 xy 两方向的像素偏移半径.
// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
	vec4 c = texture2D(texture1, TexCoord);
	
	float average = (c.r + c.b) / 2;
	if (c.g > average)
		c.g = average;
	
	vec4 bg = texture2D(texture2, TexCoord);
	
	
	
	gl_FragColor = vec4(mix( bg.rgb, c.rgb, c.a ), 1);
}
