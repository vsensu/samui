#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 viewProj;
uniform mat4 transform;

void main()
{
    gl_Position = viewProj * transform * vec4(aPos, 1.0);
}

#type fragment
#version 330 core
out vec4 FragColor;

uniform vec4 u_color;

void main()
{
  FragColor = u_color;
}