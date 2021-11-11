#version 330 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexCoord;

#define UI_ID_NUMBER 20

//struct Texture
//{	
//	sampler2D ourtexture;
//	float ID;
//};
//uniform Texture UItexture[UI_ID_NUMBER];
uniform sampler2D ourtexture;
void main()

{    
	
	//texColor=mix(texColor,texture(UItexture[i].ourtexture,TexCoord),0.2);
	vec4 texColor=texture(ourtexture,TexCoord);
	if(texColor.a<0.1f)
	 	discard;
	FragColor =texColor;
}