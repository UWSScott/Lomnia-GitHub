//Created by Scott Howie - AGP - 2015

#version 330

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

out vec2 ex_TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 modelview;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = projection * view * modelview * vec4(in_Position, 1.0f);
    vs_out.FragPos = vec3(modelview * vec4(in_Position, 1.0));
    vs_out.Normal = transpose(inverse(mat3(modelview))) * in_Normal;
    vs_out.TexCoords = in_TexCoord;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
	ex_TexCoords = in_TexCoord;
}