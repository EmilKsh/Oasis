#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out float diffuseLighting;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
uniform float lightIntensity;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);

	diffuseLighting = clamp(dot(aNormal, lightPos - aPos), 0, 1) * lightIntensity / pow(length(lightPos - aPos),2);
	TexCoord = aTexCoord;
}