#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexIndex;
layout (location = 4) in float aTilingFactor;

out vec4 v_color;
out vec2 texCoord;
out float v_tex_index;
out float v_tiling_factor;

uniform mat4 viewProj;

void main()
{
    gl_Position = viewProj * vec4(aPos, 1.0);
    v_color = aColor;
    texCoord = aTexCoord;
    v_tex_index = aTexIndex;
    v_tiling_factor = aTilingFactor;
}

#type fragment
#version 330 core
layout(location = 0) out vec4 color1;
layout(location = 1) out int color2;
layout(location = 2) out vec4 color3;
layout(location = 3) out vec4 color4;

in vec4 v_color;
in vec2 texCoord;
in float v_tex_index;
in float v_tiling_factor;

uniform sampler2D u_textures[32];

void main()
{
  color1 = texture(u_textures[int(v_tex_index)], texCoord * v_tiling_factor) * v_color;
  color2 = 50;
  color3 = vec4(0.2, 0.8, 0.3, 1.0);
  color4 = vec4(0.2, 0.3, 0.8, 1.0);
}