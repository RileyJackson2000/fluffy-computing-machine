#version 330 core

#ifndef MAX_NUM_DIR_LIGHTS
#define MAX_NUM_DIR_LIGHTS 2
#endif

#ifndef MAX_NUM_POINT_LIGHTS
#define MAX_NUM_POINT_LIGHTS 2
#endif

// be careful if you change this value
// you likely will have to change all the code involving uShadowMap${i} below
#ifndef MAX_NUM_SHADOWS
#define MAX_NUM_SHADOWS 4
#endif

// customize shadow acne bias
// max acne bias is used when view is perpendicular to light ray
// min acne bias is used when view is parallel to light ray
// and it is interpolated via the dot product
#ifndef MIN_ACNE_BIAS
#define MIN_ACNE_BIAS 0.0005
#endif

#ifndef MAX_ACNE_BIAS
#define MAX_ACNE_BIAS 0.01
#endif

layout(location = 0) out vec4 colour;

in vec3 v_pos;
in vec3 v_norm;
in vec2 v_uv;

uniform mat4 uLightVP[MAX_NUM_SHADOWS];
vec4 lightSpacePos[MAX_NUM_SHADOWS];

uniform vec3 uCamPos;

// material properties
uniform vec3 uAmbientColour = vec3(0.1, 0.1, 0.1);
uniform vec3 uDiffuseColour = vec3(0.5, 0.5, 0.5);
uniform vec3 uSpecularColour = vec3(1.0, 1.0, 1.0);
uniform float uShininess = 16.0;

const float screenGamma = 2.2;

// lights
struct DirLight {
  vec3 dir; // direction pointing away from the light source

  vec3 ambientColour;
  vec3 diffuseColour;
  vec3 specularColour;
};

uniform DirLight uDirLights[MAX_NUM_DIR_LIGHTS];

vec3 calcDirLight(DirLight light, vec3 viewDir);

struct PointLight {
  vec3 pos;

  // attenuation
  float constant;
  float linear;
  float quadratic;

  vec3 ambientColour;
  vec3 diffuseColour;
  vec3 specularColour;
};

uniform PointLight uPointLights[MAX_NUM_POINT_LIGHTS];

vec3 calcPointLight(PointLight light, vec3 viewDir);

// shadows
// glsl doesn't allow for array of samplers with dynamic indexing
// to get around this, we just list them out manually and use a switch

// in the future we can explore combining all shadows into one sampler,
// which will also allow us to have arbitrarily many of them
uniform sampler2D uShadowMap0;
uniform sampler2D uShadowMap1;
uniform sampler2D uShadowMap2;
uniform sampler2D uShadowMap3;
int curShadowMap;
float calcShadow(vec4 lightSpacePos, float lambertian);

// textures
uniform sampler2D uTexture;
vec3 texColour;

void main()
{
  vec3 viewDir = normalize(uCamPos - v_pos);

  vec3 outColour = vec3(0.0);
  texColour = texture(uTexture, v_uv).xyz; // ignore alpha for now

  curShadowMap = 0;
  for (int i = 0; i < MAX_NUM_SHADOWS; ++i)
  {
      lightSpacePos[i] = uLightVP[i] * vec4(v_pos, 1.f);
  }

  for (int i = 0; i < MAX_NUM_DIR_LIGHTS; ++i)
  {
    outColour += calcDirLight(uDirLights[i], viewDir);
  }
  
  for (int i = 0; i < MAX_NUM_POINT_LIGHTS; ++i)
  {
    outColour += calcPointLight(uPointLights[i], viewDir);
  }

  vec3 colourGammaCorrected = pow(outColour, vec3(1.0 / screenGamma));

  gl_FragColor = vec4(colourGammaCorrected, 1.0);
}

vec3 calcDirLight(DirLight light, vec3 viewDir)
{
  vec3 lightDir = normalize(-light.dir);
  float lambertian = max(dot(v_norm, lightDir), 0.0);
  // shadow
  // dynamic array access could be slow
  float shadow = calcShadow(lightSpacePos[curShadowMap], lambertian);

  // specular shading
  float spec = 0.0;
#ifdef PHONG
  vec3 halfDir = normalize(lightDir + viewDir);
  float specAngle = max(dot(halfDir, v_norm), 0.0);
#else
  vec3 halfDir = normalize(lightDir + viewDir);
  float specAngle = max(dot(halfDir, v_norm), 0.0);
#endif
  spec = pow(specAngle, uShininess);
  // combine results
  vec3 ambient  = light.ambientColour * uAmbientColour * texColour;
  vec3 diffuse  = light.diffuseColour  * lambertian * uDiffuseColour * texColour;
  vec3 specular = light.specularColour * spec * uSpecularColour;
  return (ambient + (1.f - shadow) * (diffuse + specular));
}

vec3 calcPointLight(PointLight light, vec3 viewDir)
{
  vec3 lightDir = light.pos - v_pos;
  float dist = length(lightDir);
  dist = dist * dist;
  lightDir = normalize(lightDir);

  float lambertian = max(dot(v_norm, lightDir), 0.0);

  // shadow
  // dynamic array access could be slow
  float shadow = calcShadow(lightSpacePos[curShadowMap], lambertian);

  float spec = 0.0;
  // specular shading
#ifdef PHONG
  vec3 reflectDir = reflect(-lightDir, v_norm);
  float specAngle = max(dot(viewDir, reflectDir), 0.0);
#else
  vec3 halfDir = normalize(lightDir + viewDir);
  float specAngle = max(dot(halfDir, v_norm), 0.0);
#endif
  spec = pow(specAngle, uShininess);
  // attenuation
  float attenuation = 1.0 / (1.f + light.constant
                            + light.linear * dist
                            + light.quadratic * (dist * dist));
  // combine results
  vec3 ambient  = light.ambientColour * uAmbientColour * texColour;
  vec3 diffuse  = light.diffuseColour * lambertian * uDiffuseColour * texColour;
  vec3 specular = light.specularColour * spec * uSpecularColour;
  diffuse  *= attenuation;
  specular *= attenuation;
  return (ambient + (1 - shadow) * (diffuse + specular));
}

float calcShadow(vec4 lightSpacePos, float lambertian)
{
  vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
  projCoords = 0.5f * projCoords + 0.5f;
  
  float closestDepth;
  float closestDepth0 = texture(uShadowMap0, projCoords.xy).r;
  float closestDepth1 = texture(uShadowMap1, projCoords.xy).r;
  float closestDepth2 = texture(uShadowMap2, projCoords.xy).r;
  float closestDepth3 = texture(uShadowMap3, projCoords.xy).r;
  // apparently faster than using a switch/branches
  closestDepth = float(curShadowMap == 0) * closestDepth0 +
                 float(curShadowMap == 1) * closestDepth1 +
                 float(curShadowMap == 2) * closestDepth2 +
                 float(curShadowMap == 3) * closestDepth3;
  ++curShadowMap;
  float actualDepth = projCoords.z;
  
  if (projCoords.z > 1) {
    // don't render far shadows
    return 0.f;
  }

  // bias to fight shadow acne
  float bias = max(MAX_ACNE_BIAS * (1 - lambertian), MIN_ACNE_BIAS);
  return actualDepth - bias > closestDepth? 1.f : 0.f;
}

