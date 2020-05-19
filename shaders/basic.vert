#version 410

layout(location = 0) in vec3 vertex_position;

out vec4 fPos;
void main()
{
    gl_Position = vec4(vertex_position, 1.0);
    fPos = vec4(vertex_position,1.0f);
}