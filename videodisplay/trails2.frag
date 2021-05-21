#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform vec2      samplingRadius;       // 采样在 xy 两方向的像素偏移半径.
// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

vec2 iResolution = vec2(720.0, 720.0);

void main()
{
FragColor = texture(texture1, TexCoord);
return;

vec2 m = vec2(1.0, 1.0)/iResolution.xy;
    vec2 uv = TexCoord;

    //Chromatic Abberation Distance;
    float a = .0;
    
    vec3 c = vec3(
        		texture(texture1,uv-(m*a)).r,
              	texture(texture1,uv).g,
              	texture(texture1,uv+(m*a)).b
    			);
 	//c *= vec3(1., 0.75, 0.86);
	FragColor = vec4(c,1.);
}