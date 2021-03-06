

#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	FragColor = texture(texture1,  TexCoord);//gl_FragCoord.xy / vec2(1280, 720)
}
