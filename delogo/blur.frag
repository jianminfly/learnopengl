#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform vec2      samplingRadius;	// 采样在 xy 两方向的像素偏移半径.
// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

float _BlurSize = 0.01;

void main() {
	//calculate aspect ratio
	float invAspect = 720.0 / 1280.0;
	//init color variable
	float alpha = 0;
	
	//iterate over blur samples
	for (float x = 0; x < 10; x++) {
		//get uv coordinate of sample
		vec2 uv = TexCoord + vec2((x / 9 - 0.5) * _BlurSize * invAspect, 0);
		//add color at position to color
		alpha += texture2D(texture1, uv).a;
	}
	
	for (float y = 0; y < 10; y++) {
		//get uv coordinate of sample
		vec2 uv = TexCoord + vec2(0, (y / 9 - 0.5) * _BlurSize * invAspect);
		//add color at position to color
		alpha += texture2D(texture1, uv).a;
	}
	//divide the sum of values by the amount of samples
	alpha = alpha / 20;
	
	vec4 c;
	c.rgb = texture2D(texture1, TexCoord).rgb;
	c.a = alpha;
	
	
	FragColor = c;
}
