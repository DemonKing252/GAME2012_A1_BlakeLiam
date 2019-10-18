#version 430 core

out vec4 frag_color;
in vec3 myColor; // in the vertex shader
void main()
{
	frag_color = vec4(myColor, 1.0);
}