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
    vec3 nb, ob, l, dist, c, cw, cr, cg, cb;
    
    // Trail length
    l = vec3(0.95, 0.97, 0.99);
    nb = vec3(texture(texture2,uv).rgb) * l;
    ob = vec3(texture(texture1,uv).rgb) * l;
    dist.r = distance(ob.r, nb.r);
    dist.g = distance(ob.g, nb.b);
    dist.b = distance(ob.b, nb.b);
    
    c = nb;
    cw = nb;
    cr = nb;
    cg = nb;
    cb = nb;
    
    //white trail
    cw.rgb += dist;
    
    //red trail
	cr.r += dist.r;
    cr.g = cr.r - dist.r;
    cr.b = cr.r - dist.r;
    
    //green trail
    cg.g += dist.g;
    cg.r = cg.g - dist.g;
    cg.b = cg.g - dist.g;
    
    //blue trail
    cb.b += dist.b;
    cb.r = cb.b - dist.b;
    cb.g = cb.b - dist.b;
    
    c = clamp(vec3(cr.rg, cb.b), vec3(0.), vec3(1.));
    FragColor = vec4(c, 1.);
}