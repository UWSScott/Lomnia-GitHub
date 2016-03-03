//Created by Scott Howie - AGP - 2015

#version 330

in vec3 in_Position;
uniform mat4 lightSpaceMatrix;
uniform mat4 modelview;

void main()
{
	gl_Position = lightSpaceMatrix * modelview * vec4(in_Position,1.0);
}

