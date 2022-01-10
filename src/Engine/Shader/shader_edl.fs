#version 330 core

in vec2 tex_coordinate;
out vec4 color_fragment;

uniform sampler2D tex_color;
uniform sampler2D tex_depth;

//Ok le problème vient du faite que l'on utiliser 2x le meme texture de profondeur

uniform float A;
uniform float B;
uniform float EDL_STRENGTH;
uniform float EDL_DISTANCE;
uniform bool EDL_ON;



//FUNCTION 1 - Compute real depth
float compute_real_depth(float depth){
  return 0.5*(-A * depth + B) / depth + 0.5;
}

//FUNCTION 2 - Compute neighbor influence
vec2 neighborContribution(float depth_current, vec2 offset) {
  float dist = EDL_DISTANCE;

  vec2 NN_coord_0 = tex_coordinate + offset * floor(dist);
  vec2 NN_coord_1 = tex_coordinate + offset * ceil(dist);

  float depth_NN_0 = texture(tex_depth, NN_coord_0).r;
  float depth_NN_1 = texture(tex_depth, NN_coord_1).r;

  if (depth_NN_0 == 0.0 || depth_NN_1 == 0.0) {
    return vec2(0.0);
  }

  // interpolate the two adjacent depth values
  float depth_NN_mix = mix(depth_NN_0, depth_NN_1, fract(dist));
  float depth_NN_current = compute_real_depth(depth_NN_mix);
  float NN_contrib = max(0.0, log2(depth_current) - log2(depth_NN_current));

  return vec2(NN_contrib, 1.0);
}

//MAIN FUNCTION
void main()
{
  vec4 color = texture(tex_color, tex_coordinate);

  if(EDL_ON){

    // Build the Depth
    float depth_buffer = texture(tex_depth, tex_coordinate).r;
    float depth_real = compute_real_depth(depth_buffer);

    //Check neighborhood influence
    vec2 texel_size = 1.0 / vec2(804, 382);
    vec2 responseAndCount = vec2(0.0);
    responseAndCount += neighborContribution(depth_real, vec2(-texel_size.x, 0.0));
    responseAndCount += neighborContribution(depth_real, vec2(+texel_size.x, 0.0));
    responseAndCount += neighborContribution(depth_real, vec2(0.0, -texel_size.y));
    responseAndCount += neighborContribution(depth_real, vec2(0.0, +texel_size.y));

    // Build the Eye Dome Lighting effect PostProcessing
    float depth_response = responseAndCount.x / responseAndCount.y;
    float shade = exp(-depth_response * 300.0 * EDL_STRENGTH);
    if(responseAndCount.x == 0){
      shade = 10;
    }
    color.rgb *= shade;

  }

  color_fragment = vec4(color);
}
