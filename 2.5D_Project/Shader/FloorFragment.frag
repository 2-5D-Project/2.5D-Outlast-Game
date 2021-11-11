#version 330 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexCoord;

uniform sampler2D ourtexture;
uniform sampler2D NoiseTexture;

void main()
{    
    
    //FragColor = mix(texture(ourtexture, TexCoord),texture(NoiseTexture, TexCoord),0.2f);
   vec4 texColor=texture(ourtexture,TexCoord);
	if(texColor.a<0.1f)
	 	discard;
	FragColor =texColor;
}