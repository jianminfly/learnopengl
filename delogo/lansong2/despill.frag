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
    vec4 color = texture2D(texture1, TexCoord);

    FragColor = vec4(color.r*1.2,color.g *greenPercent,color.b *1.2,color.a*1.0);
}