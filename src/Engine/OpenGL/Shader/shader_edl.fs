#version 330 core

in vec2 frag_tex_coord;
out vec4 frag_color;

uniform sampler2D tex_color;
uniform sampler2D tex_depth;

uniform float A;
uniform float B;
uniform float EDL_STRENGTH;
uniform float EDL_DISTANCE;
uniform float EDL_RADIUS;
uniform bool EDL_ON;
uniform int GL_WIDTH;
uniform int GL_HEIGHT;



//FUNCTION 1 - Compute normalized depth
float compute_depth_normalized(float depth){
  // depth: Linear depth, in world units
  float depth_norm = 0.5 * (-A * depth + B) / depth + 0.5;
  // depth_norm: normalized depth between [0, 1]
  return depth_norm;
}

//FUNCTION 2 - Compute neighbor influence
vec2 neighbor_contribution(float depth_norm, vec2 offset) {
  // get depth at texture specified coordinate
  vec2 NN_coord = frag_tex_coord + offset;
  float depth_NN = texture(tex_depth, NN_coord).r;

  if (depth_NN == 0.0){
    return vec2(0.0);
  }

  // interpolate the two adjacent depth values
  float depth_NN_norm = compute_depth_normalized(depth_NN);
  float NN_contrib = max(0.0, log2(depth_norm) - log2(depth_NN_norm));

  return vec2(NN_contrib, 1.0);
}

//MAIN FUNCTION
void main()
{
  vec4 color = texture(tex_color, frag_tex_coord);

  if(EDL_ON){

    // Build the Depth
    float depth_buffer = texture(tex_depth, frag_tex_coord).r;
    float depth_norm = compute_depth_normalized(depth_buffer);

    //Check neighborhood influence
    vec2 texel_size = EDL_RADIUS / vec2(GL_WIDTH, GL_HEIGHT);
    vec2 NN_response = vec2(0.0);
    NN_response += neighbor_contribution(depth_norm, vec2(-texel_size.x, 0.0));
    NN_response += neighbor_contribution(depth_norm, vec2(+texel_size.x, 0.0));
    NN_response += neighbor_contribution(depth_norm, vec2(0.0, -texel_size.y));
    NN_response += neighbor_contribution(depth_norm, vec2(0.0, +texel_size.y));

    // Build the Eye Dome Lighting effect PostProcessing
    float depth_response = NN_response.x / NN_response.y;
    float shade = exp(-depth_response * 300.0 * EDL_STRENGTH);

    color.rgb *= shade;

  }

  frag_color = vec4(color);

}
