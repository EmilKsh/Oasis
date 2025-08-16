#version 330 core
out vec4 fragColor;

uniform vec3 myColor;

void main()
{
	fragColor = vec4(clamp(myColor, 0.f, 1.f), 1.0f);
}
