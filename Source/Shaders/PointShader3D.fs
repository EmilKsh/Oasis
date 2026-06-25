#version 330 core
out vec4 FragColor;

in vec3 pointColor;

void main()
{
	vec2 p = gl_PointCoord * 2.0f - 1.0f;
	if (dot(p,p) > 1)
		discard;

	FragColor = vec4(pointColor, 1.0f);
}
