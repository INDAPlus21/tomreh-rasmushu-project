#version 440

layout (location=0) in vec3 aPos;

out vec3 position_passed;

void main()
{
    gl_Position = vec4(aPos, 1.0f);

    position_passed = aPos;
}