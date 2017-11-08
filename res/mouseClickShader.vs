#version 400
uniform mat4 MVP;
attribute vec3 position;

void main()
{
	gl_Position = MVP * vec4(position, 1.0);
   	gl_ClipDistance[0] = 100;

}