#version 330 core

out vec4 fragment_color;
in vec2 TexCoord;

uniform sampler2D colorTexture;
uniform sampler2D depthTexture;
uniform float A;
uniform float B;
uniform float EDL_STRENGTH;
uniform float EDL_DISTANCE;
uniform bool WITH_EDL;


vec4 color;
float buffer_depth;
float real_depth;

float buffer_depth_to_real_depth(float depth){
  return 0.5*(-A * depth + B) / depth + 0.5;
}
vec2 neighborContribution(float log2Depth, vec2 offset) {
  float dist = EDL_DISTANCE;
  vec2 texCoordOrig = TexCoord + offset * dist;
  vec2 texCoord0 = TexCoord + offset * floor(dist);
  vec2 texCoord1 = TexCoord + offset * ceil(dist);
  float depthOrLogDepth0 = texture2D(depthTexture, texCoord0).r;
  float depthOrLogDepth1 = texture2D(depthTexture, texCoord1).r;
  if (depthOrLogDepth0 == 0.0 || depthOrLogDepth1 == 0.0) {
    return vec2(0.0);
  }

  // interpolate the two adjacent depth values
  float depthMix = mix(depthOrLogDepth0, depthOrLogDepth1, fract(dist));
  float new_depth = buffer_depth_to_real_depth(depthMix);
  return vec2(max(0.0, log2Depth - log2(new_depth)), 1.0);
}


void main(){

  color = texture(colorTexture, TexCoord);

  if(WITH_EDL){

    // Build the Depth
    buffer_depth = texture(depthTexture, TexCoord).r;
    real_depth = buffer_depth_to_real_depth(buffer_depth);
    float log2Depth = log2(real_depth);
    if (log2Depth == 0){
      discard;
    }

    vec2 texelSize = 1.0 / vec2(720, 1080);
    vec2 responseAndCount = vec2(0.0);
    responseAndCount += neighborContribution(log2Depth, vec2(-texelSize.x, 0.0));
    responseAndCount += neighborContribution(log2Depth, vec2(+texelSize.x, 0.0));
    responseAndCount += neighborContribution(log2Depth, vec2(0.0, -texelSize.y));
    responseAndCount += neighborContribution(log2Depth, vec2(0.0, +texelSize.y));

    // Build the Eye Dome Lighting effect PostProcessing
    float response = responseAndCount.x / responseAndCount.y;
    float shade = exp(-response * 300.0 * EDL_STRENGTH);
    color.rgb *= shade;

  }

  fragment_color = vec4(color);
}
