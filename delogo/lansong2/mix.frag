#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform vec2      samplingRadius;	// 采样在 xy 两方向的像素偏移半径.
// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
vec4 base = texture2D(texture1, TexCoord);
vec4 overlay = texture2D(texture2, TexCoord);
vec4 outputColor;
outputColor.r = base.a*base.r + overlay.r * (1.0 - base.a);
outputColor.g = base.a*base.g + overlay.g * (1.0 - base.a);
outputColor.b = base.a*base.b + overlay.b * (1.0 - base.a);
outputColor.a = base.a;
FragColor = outputColor;
}