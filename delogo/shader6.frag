#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform vec2      samplingRadius;	// 采样在 xy 两方向的像素偏移半径.
// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;
vec3 cfour = vec3(0.13, 0.59, 0.31);;


float bbbhold = 0.16;
float sssso = 0.05;

void main()
{
    vec4 textureColor = texture2D(texture1, TexCoord);
    
    float maskY = 0.2989 * cfour.r + 0.5866 * cfour.g + 0.1145 * cfour.b;
    float maskCr = 0.7132 * (cfour.r - maskY);
    float maskCb = 0.5647 * (cfour.b - maskY);
    
    float Y = 0.2989 * textureColor.r + 0.5866 * textureColor.g + 0.1145 * textureColor.b;
    float Cr = 0.7132 * (textureColor.r - Y);
    float Cb = 0.5647 * (textureColor.b - Y);
    
    float blendValue = smoothstep(bbbhold, bbbhold + sssso, distance(vec2(Cr, Cb), vec2(maskCr, maskCb)));
    vec4 textureColor2 = vec4(textureColor.rgb* blendValue, textureColor.a * blendValue);

	gl_FragColor=textureColor2;
}