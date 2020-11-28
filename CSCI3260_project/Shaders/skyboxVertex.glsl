#version 430

in layout (location = 0) vec3 aPos;

uniform mat4 projection;
uniform mat4 view;

out vec3 TexCoords;

void main()
{
    TexCoords = aPos;
    gl_Position = (projection * view * vec4(aPos, 1.0)).xyww;
}