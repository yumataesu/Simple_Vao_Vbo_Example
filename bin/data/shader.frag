#version 410

in vec3 vcolor;
out vec4 color;


void main()
{
    color = vec4(vcolor, 1.0);
}
