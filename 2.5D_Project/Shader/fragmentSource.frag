#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D ourtexture;
uniform sampler2D texture_diffuse;

void main()
{    
	// vec4 texColor=texture(ourtexture,TexCoords);
	// if(texColor.a<0.1f)
	//	discard;
	// FragColor =texColor;
	//
   FragColor = texture(texture_diffuse, TexCoords);
}