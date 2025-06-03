#version 330 core
out vec4 fragColor;

in float diffuseLighting;
in vec2 TexCoord;

uniform sampler2D TextureParam;
uniform bool hasTexture = false;
uniform vec3 myColor;
uniform vec3 lightPosition;

float ambientLighting = 0.12;

void main()
{
	//fragColor = vec4(myColor, 1.0f);
	if (hasTexture)
	{
		fragColor = vec4(clamp(myColor * diffuseLighting + ambientLighting, 0.f, 1.f), 1.0f) * texture(TextureParam, TexCoord);
	}
	else
	{
		fragColor = vec4(clamp(myColor * diffuseLighting + ambientLighting, 0.f, 1.f), 1.0f);
	}
}
