#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 view;
uniform mat4 projection;
uniform bool mapToViewport;
uniform vec3 worldMin;
uniform vec3 worldMax;
uniform vec3 viewportMin;
uniform vec3 viewportMax;

out vec3 pointColor;

void main()
{
	if (mapToViewport)
	{
		vec3 worldSize = worldMax - worldMin;
		vec3 normalizedPosition = clamp((aPos - worldMin) / worldSize, vec3(0.0), vec3(1.0));
		vec2 screenPosition = mix(viewportMin.xy, viewportMax.xy, normalizedPosition.xy);
		gl_Position = vec4(screenPosition, normalizedPosition.z * 2.0 - 1.0, 1.0);
	}
	else
	{
		gl_Position = projection * view * vec4(aPos, 1.0);
	}

	pointColor = aColor;
}
