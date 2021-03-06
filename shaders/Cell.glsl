--Vertex

#version 430

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;
layout(location = 2) in vec2 vTex;

out vec3 norm;
out vec2 tex;

layout(std140) uniform cameraBlock
{
	mat4 worldToClip;
	vec4 cameraPos;
	vec4 cameraDir;
};

uniform mat4 modelToWorld;

void main()
{
	norm = mat3(modelToWorld) * vNorm; tex = vTex;
	gl_Position = worldToClip * modelToWorld * vec4(vPos, 1.0);
}

--Fragment

#version 430

in vec3 norm;
in vec2 tex;

out vec4 color;

layout(std140) uniform cameraBlock
{
	mat4 worldToClip;
	vec4 cameraPos;
	vec4 cameraDir;
};

uniform sampler2D surfaceTex;
uniform int levels;

void main()
{
	float camDotN = clamp(dot(-vec3(cameraDir), norm), 0, 1);
	int level = int(camDotN * levels);
	float brightness = float(level) / levels;
	color = brightness * texture(surfaceTex, tex); 
}
