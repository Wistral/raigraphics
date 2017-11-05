#version 400

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 colorCoord;
attribute vec3 normal;

varying vec2 texCoord0;
varying vec3 position0;
varying vec3 colorCoord0;
varying vec3 camPos;

uniform mat4 MVP;
uniform mat4 Normal;
uniform vec3 cameraPos;
uniform vec3 colorMono;

void main()
{
	gl_Position = MVP * vec4(position, 1.0);
	position0 = position;
	texCoord0 = texCoord;
	colorCoord0 = colorMono;
	camPos = cameraPos;
}