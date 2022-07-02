#version 330 core
out vec4 out_colour;

in vec2 v_tex;
in vec4 v_colour; // make this linear space
in vec2 v_sprite_pos;
in float v_tex_unit;

uniform sampler2D textures[1];

float
linear_to_srgb(float f)
{
  if (f <= 0.0031308f)
    return 12.92f * f;
  return 1.055f * pow(f, 1.0f / 2.4f) - 0.055f;
};

void
main()
{
  vec4 lin_tex = texture(textures[0], v_tex);

  out_colour = vec4(
    linear_to_srgb(lin_tex.r),
    linear_to_srgb(lin_tex.g),
    linear_to_srgb(lin_tex.b),
    lin_tex.a
  );
}
