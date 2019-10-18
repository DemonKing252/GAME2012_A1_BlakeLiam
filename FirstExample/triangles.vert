#version 430 core
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour;

out vec3 myColor;

uniform mat4 MVP;

void main()
{
	myColor = vertex_colour;
	gl_Position = MVP * vec4(vertex_position,1.0f);

}
