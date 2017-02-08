#version 410

in vec3 position;
in vec3 color;


out vec3 vcolor;

void main()
{
    gl_Position = vec4(position, 1.0);
    vcolor = color;
}
