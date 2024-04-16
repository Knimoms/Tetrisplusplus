#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in float brightness;

out vec4 v_Color;

uniform mat4 u_MVP;
uniform float u_AnimLengthSeconds;
uniform float u_AnimLengthProgressSeconds;

void main()
{
	vec4 alteredPosition = position;
	alteredPosition.x = alteredPosition.x * (u_AnimLengthProgressSeconds / u_AnimLengthSeconds);
	gl_Position = u_MVP * alteredPosition;
	v_Color = color * brightness;
};