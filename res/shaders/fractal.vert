#version 330

layout (location=0) in vec3 aPos;

out vec3 v_Pos;

void main()
{
    gl_Position = vec4(aPos, 1.0f);

    v_Pos = aPos;
}