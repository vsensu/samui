#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

uniform mat4 viewProj;
uniform mat4 transform;

void main()
{
    gl_Position = viewProj * transform * vec4(aPos, 1.0);
    texCoord = aTexCoord;
}

#type fragment
#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D u_texture;
uniform vec4 u_color;

void main()
{
  FragColor = texture(u_texture, texCoord) * u_color;
}