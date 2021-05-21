#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform vec2      samplingRadius;	// 采样在 xy 两方向的像素偏移半径.
// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

vec2 iResolution = vec2(1280.0, 720.0);

void main()
{

    vec2 uv = TexCoord;
   
    vec3 c = texture(texture1,uv).rgb;
	FragColor = vec4(c,1.);
}