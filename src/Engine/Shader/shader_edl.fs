#version 330 core

in vec2 tex_coordinate;
out vec4 color_fragment;

uniform sampler2D tex_color;
uniform sampler2D tex_depth;

uniform float A;
uniform float B;
uniform float EDL_STRENGTH;
uniform float EDL_DISTANCE;
uniform bool EDL_ON;

vec4 color;
float depth_buffer;
float depth_real;


//FUNCTION 1 - Compute real depth
float depth_buffer_to_depth_real(float depth){
  return 0.5*(-A * depth + B) / depth + 0.5;
}

//FUNCTION 2 - Compute neighbor influence
vec2 neighborContribution(float log2Depth, vec2 offset) {
  float dist = EDL_DISTANCE;

  vec2 texCoordOrig = tex_coordinate + offset * dist;
  vec2 texCoord0 = tex_coordinate + offset * floor(dist);
  vec2 texCoord1 = tex_coordinate + offset * ceil(dist);

  float depthOrLogDepth0 = texture2D(tex_depth, texCoord0).r;
  float depthOrLogDepth1 = texture2D(tex_depth, texCoord1).r;

  if (depthOrLogDepth0 == 0.0 || depthOrLogDepth1 == 0.0) {
    return vec2(0.0);
  }

  // interpolate the two adjacent depth values
  float depthMix = mix(depthOrLogDepth0, depthOrLogDepth1, fract(dist));
  float new_depth = depth_buffer_to_depth_real(depthMix);

  return vec2(max(0.0, log2Depth - log2(new_depth)), 1.0);
}

//MAIN FUNCTION
void main()
{
  color = texture(tex_color, tex_coordinate);

  if(EDL_ON){

    // Build the Depth
    depth_buffer = texture(tex_depth, tex_coordinate).r;
    depth_real = depth_buffer_to_depth_real(depth_buffer);
    float log2Depth = log2(depth_real);
    if (log2Depth == 0){
      discard;
    }

    vec2 texel_size = 1.0 / vec2(720, 1080);
    vec2 responseAndCount = vec2(0.0);

    responseAndCount += neighborContribution(log2Depth, vec2(-texel_size.x, 0.0));
    responseAndCount += neighborContribution(log2Depth, vec2(+texel_size.x, 0.0));
    responseAndCount += neighborContribution(log2Depth, vec2(0.0, -texel_size.y));
    responseAndCount += neighborContribution(log2Depth, vec2(0.0, +texel_size.y));

    // Build the Eye Dome Lighting effect PostProcessing
    float depth_response = responseAndCount.x / responseAndCount.y;
    float shade = exp(-depth_response * 300.0 * EDL_STRENGTH);
    color.rgb *= shade;

  }

  color_fragment = vec4(color);
}
