#version 330 core
precision mediump float;

uniform sampler2D texture2;
uniform sampler2D texture1;
float intensity = 1;

in vec2 TexCoord;	// 采样坐标.
out vec4 fragColor;		// 输出的颜色.

void main() {
    // 取出当前像素的纹素
    highp vec4 textureColor = texture2D(texture1, TexCoord);
    
    highp float blueColor = textureColor.b * 16.0;
    
	/*
	if(blueColor > 15.9)
	{gl_FragColor = vec4(1,0,0,1); return;}
	*///成立
	
    // 计算B通道，看使用哪个像素色块（这里分别对计算结果向上，向下取整，然后再对两者进行线性计算，减小误差）
    highp vec2 quad1;
    quad1.y = ceil(blueColor) / 1.0;
    quad1.x = 0;
    
    highp vec2 quad2;
    quad2.y = floor(ceil(blueColor) / 1.0);
    quad2.x = ceil(blueColor) - (quad2.y * 1.0);
    
    // 计算R、G通道
    highp vec2 texPos1;
	// (quad1.x * 0.125): 0.125表示8个格子均分0-1的坐标 quad1.x是向下取整的格子数
	// ((0.125 - 1.0/512.0) * textureColor.r):0.125*textureColor.r表示把坐标缩小到小格子中，- 1.0/512.0*textureColor.r表示减去不满一小格的部分
	// 0.5/512.0:为修正项，减少整体误差
    texPos1.x = (quad1.x * 1)  +  textureColor.r;
    texPos1.y = (quad1.y * 1.0/17.0) + ((1.0/17.0) * textureColor.g);
     
    highp vec2 texPos2;
    texPos2.x = (quad2.x * 1) + 0.5/17.0 + ((1 - 1.0/17.0) * textureColor.r);
    texPos2.y = (quad2.y * 1.0/17.0) + 0.5/17.0 + ((1 - 1.0/17.0) * textureColor.g);
    
    // 根据转换后的纹理坐标，在基准图上取色
    lowp vec4 newColor1 = texture2D(texture2, texPos1);
    /*
	if(texPos1.x>1)
		newColor1 = vec4(1,0,0,1);
	else if(texPos1.x<0)
		newColor1 = vec4(0,1,0,1);
	else if(texPos1.y>1)
		newColor1 = vec4(0,0,1,1);
	else if(texPos1.y<0)
		newColor1 = vec4(1,1,0,1);
	*/
	
    gl_FragColor = newColor1;
}
