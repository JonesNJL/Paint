#version 430 core

layout (location = 0) in ivec3 aPos;
layout (location = 1) in ivec4 aUV;
layout (location = 2) in int aTexture;

out vec3 bPos;
out vec2 bUV;
out int bTexture;

void main()
{
	gl_Position = vec4(((aPos.x / 1920.0) * 2.0) - 1.0, ((aPos.y / 640.0) * 2.0) - 1.0, aPos.z, 1.0);

	bPos = aPos;
	bUV = vec2((aUV.x / float(aUV.y)), (aUV.z / float(aUV.w)));
	bTexture = aTexture;
}