#version 120

#ifdef GL_ES 
precision mediump float;
#define GLSLIFY 1
#define GLSLIFY 1 
#endif

#define PI 3.1415926

uniform float     iDuration;             // shader playback total time (in seconds)
uniform float     iTime;                 // shader playback current time (in seconds)
uniform vec3      iResolution;
uniform sampler2D iChannel[8];           // input channel. XX = 2D/Cube
vec2 fTextureCoord = gl_FragCoord.xy / iResolution.xy;


vec3 _DarkColor = vec3(0.13, 0.59, 0.31);
vec3 _LightColor = vec3(0.13, 0.59, 0.31);
uniform float _Threshhold = 0;
uniform float _BlurSize;

float CalculateDistance(float c, float min, float max) {
	if (c < min) return min - c;
	if (c > max) return c - max;
	return 0;
}

float deltaE(vec3 c1, vec3 c2) {
	return sqrt( pow((c2.x - c1.x),2) + pow((c2.y - c1.y),2) + pow((c2.z - c1.z),2) );
}


float step1(float col) {

	if (col > 0.04045)
	{
		col = pow(((col + 0.055) / 1.055), 2.4);
	}
	else
	{
		col = col / 12.92;
	}

	return col;
}

float step2(float col) {
	if (col > 0.008856) {
		col = pow(col, (1.0 / 3.0));
	}
	else {
		col = (7.787 * col) + (16.0 / 116.0);
	}
	return col;
}

vec3 RGB2XYZ(vec3 color) {
	color.r = step1(color.r);
	color.g = step1(color.g);
	color.b = step1(color.b);

	color.r = color.r * 100;
	color.g = color.g * 100;
	color.b = color.b * 100;

	vec3 xyz;

	xyz.x = color.r * 0.4124 + color.g * 0.3576 + color.b * 0.1805;
	xyz.y = color.r * 0.2126 + color.g * 0.7152 + color.b * 0.0722;
	xyz.z = color.r * 0.0193 + color.g * 0.1192 + color.b * 0.9505;	

	return xyz;
}

vec3 XYZ2LAB(vec3 color) {
	vec3 temp;

	temp.x = color.x / 92.834;
	temp.y = color.y / 100.000;
	temp.z = color.z / 103.665;

	temp.x = step2(temp.x);
	temp.y = step2(temp.y);
	temp.z = step2(temp.z);

	vec3 lab;

	lab.x = (116 * temp.y) - 16;
	lab.y = 500 * (temp.x - temp.y);
	lab.z = 200 * (temp.y - temp.z);

	return lab;
}

vec4 getcolor(vec2 uv){
	vec4 c = texture2D(iChannel[0], uv);
	
	vec4 xyz;
	vec4 lab;
	xyz.a = 1;
	lab.a = 1;
	
	xyz.rgb = RGB2XYZ(c.rgb);
	lab.rgb = XYZ2LAB(xyz.rgb);
	
	vec3 dxyz = RGB2XYZ(_DarkColor);
	vec3 lxyz = RGB2XYZ(_LightColor);
	vec3 dlab = XYZ2LAB(dxyz);
	vec3 llab = XYZ2LAB(lxyz);
	
	float deltae1 = deltaE(lab.rgb, dlab) / 100;
	float deltae2 = deltaE(lab.rgb, llab) / 100;
	float ediff = deltae1 * deltae2;
	c.a = ediff * c.a;
	
	if (c.a < _Threshhold)
		c.a = 0;
	else
		c.a = 1;
	

	return c;
}

vec4 erode_color(vec2 tex){
	vec4 c = getcolor(tex);
	
	float alpha = c.a;
	int scale = 1;
	for (int i = -scale; i <= scale && alpha > 0; ++i) {
		for (int j = -scale; j <= scale && alpha > 0; j++) {
			alpha -= 1 - getcolor(tex + vec2(i / iResolution.x, j / iResolution.y)).a;
		}
	}
	c.a = alpha;
	return c;
}

void main() { 

// blur
	//calculate aspect ratio
	float invAspect = iResolution.y / iResolution.x;
	//init color variable
	float alpha = 0;
	
	//iterate over blur samples
	for (float x = 0; x < 10; x++) {
		//get uv coordinate of sample
		vec2 uv = fTextureCoord + vec2((x / 9 - 0.5) * _BlurSize * invAspect, 0);
		//add color at position to color
		alpha += erode_color(uv).a;
	}
	
	for (float y = 0; y < 10; y++) {
		//get uv coordinate of sample
		vec2 uv = fTextureCoord + vec2(0, (y / 9 - 0.5) * _BlurSize * invAspect);
		//add color at position to color
		alpha += erode_color(uv).a;
	}
	//divide the sum of values by the amount of samples
	alpha = alpha / 20;
	
	vec4 c;
	c.rgb = erode_color(fTextureCoord).rgb;
	c.a = alpha;
	
	float average = (c.r + c.b) / 2;
	if (c.g > average)
		c.g = average;
	
	gl_FragColor = c;
}






















