#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in uint aTexIndex;
layout (location = 4) in float aTilingFactor;
layout (location = 5) in uint aEntityID;

out vec4 v_color;
out vec2 texCoord;
flat out float v_tex_index;
out float v_tiling_factor;
flat out float v_entity_id;

uniform mat4 viewProj;

void main()
{
    gl_Position = viewProj * vec4(aPos, 1.0);
    v_color = aColor;
    texCoord = aTexCoord;
    v_tex_index = aTexIndex;
    v_tiling_factor = aTilingFactor;
    v_entity_id = aEntityID;
}

#type fragment
#version 330 core
layout(location = 0) out vec4 color1;
layout(location = 1) out uint color2;
layout(location = 2) out vec4 color3;
layout(location = 3) out vec4 color4;

in vec4 v_color;
in vec2 texCoord;
flat in float v_tex_index;
in float v_tiling_factor;
flat in float v_entity_id;

uniform sampler2D u_textures[32];

void main()
{
  color1 = texture(u_textures[int(v_tex_index)], texCoord * v_tiling_factor) * v_color;
  color2 = uint(v_entity_id);
  color3 = vec4(0.2, 0.8, 0.3, 1.0);
  color4 = vec4(0.2, 0.3, 0.8, 1.0);
}