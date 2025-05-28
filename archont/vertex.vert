#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 outColor;

uniform mat4 view;
uniform mat4 transform;
uniform mat4 projection;

uniform float time;

const float amplitude = 0.125;
const float frequency = 4;
const float PI = 3.14159;

void main()
{
	float distance = length(aPos);
	float y = amplitude * sin(-PI * distance * frequency + time);
	gl_Position = projection * view * transform * vec4(aPos.x, y, aPos.z, 1.0f);
	outColor = aColor;
}