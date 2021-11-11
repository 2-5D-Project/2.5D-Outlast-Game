#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec4 boneIds;
layout (location = 4) in vec4 boneWeights;

out vec2 TexCoords;

uniform mat4 bone_transforms[50];
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
		mat4 boneTransform  =  mat4(0.0);
		boneTransform  +=    bone_transforms[int(boneIds.x)] * boneWeights.x;
		boneTransform  +=    bone_transforms[int(boneIds.y)] * boneWeights.y;
		boneTransform  +=    bone_transforms[int(boneIds.z)] * boneWeights.z;
		boneTransform  +=    bone_transforms[int(boneIds.w)] * boneWeights.w;
	vec4 pos =boneTransform * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
    //gl_Position = projMat * viewMat * modelMat * vec4(aPos,1.0);
	gl_Position = projMat * viewMat * modelMat * pos;
}