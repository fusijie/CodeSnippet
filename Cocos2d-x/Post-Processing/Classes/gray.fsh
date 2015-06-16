//optimize code
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform sampler2D lut;
		
void main()			
{
    vec4 originColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
    vec2 lutCoord;
    lutCoord.x = (originColor.r + floor(originColor.b * 15.0 + 0.5)) * 0.062255859 + 0.001953125;
    lutCoord.y = originColor.g * 0.9375 + 0.03125;
    gl_FragColor = texture2D(lut, lutCoord);
}

//origin code
//varying vec4 v_fragmentColor;
//varying vec2 v_texCoord;
//uniform sampler2D lut;
//const float pH = 256.0;
//const float pV = 16.0;
//
//void main()
//{
//    vec4 originColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
//    vec2 lutCoord;
//    lutCoord.x = ((originColor.r)/16.0 + floor(originColor.b*15.0 + 0.5)*16.0/256.0)*(pH-1.0)/pH +1.0/(pH*2.0);
//    lutCoord.y = originColor.g * (pV-1.0)/pV +1.0/(pV*2.0);
//    gl_FragColor = texture2D(lut, lutCoord);
//}
