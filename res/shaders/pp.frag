#version 330
out vec4 FragColor;

in vec2 v_TexCoord;
uniform sampler2D u_Color;

void main()
{
    FragColor = texture(u_Color, v_TexCoord);
} 