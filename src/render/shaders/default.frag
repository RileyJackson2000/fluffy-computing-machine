#version 330 core

#ifndef MAX_NUM_DIR_LIGHTS
#define MAX_NUM_DIR_LIGHTS 2
#endif

#ifndef MAX_NUM_POINT_LIGHTS
#define MAX_NUM_POINT_LIGHTS 10
#endif

layout(location = 0) out vec4 colour;

in vec3 v_pos;
in vec3 v_norm;
in vec2 v_uv;

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
uniform int numDirLights;

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
uniform int numPointLights;

vec3 calcPointLight(PointLight light, vec3 viewDir);  

// textures
uniform sampler2D uTexture;
vec3 texColour;

void main()
{
  vec3 viewDir = normalize(uCamPos - v_pos);

  vec3 outColour = vec3(0.0);
  texColour = texture(uTexture, v_uv).xyz; // ignore alpha for now

  for (int i = 0; i < numDirLights; ++i)
  {
    outColour += calcDirLight(uDirLights[i], viewDir);
  }
  
  for (int i = 0; i < numPointLights; ++i)
  {
    outColour += calcPointLight(uPointLights[i], viewDir);
  }

  vec3 colourGammaCorrected = pow(outColour, vec3(1.0 / screenGamma));

  gl_FragColor = vec4(colourGammaCorrected, 1.0);
}

vec3 calcDirLight(DirLight light, vec3 viewDir)
{
  vec3 lightDir = normalize(-light.dir);
  // diffuse shading
  float lambertian = max(dot(v_norm, lightDir), 0.0);
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
  return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 viewDir)
{
  vec3 lightDir = light.pos - v_pos;
  float dist = length(lightDir);
  dist = dist * dist;
  lightDir = normalize(lightDir);

  // diffuse shading
  float lambertian = max(dot(v_norm, lightDir), 0.0);

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
  float attenuation = 1.0 / 
        (light.constant + light.linear * dist + light.quadratic * (dist * dist));    
  // combine results
  vec3 ambient  = light.ambientColour * uAmbientColour * texColour;
  vec3 diffuse  = light.diffuseColour  * lambertian * uDiffuseColour * texColour;
  vec3 specular = light.specularColour * spec * uSpecularColour;
  ambient  *= attenuation;
  diffuse  *= attenuation;
  specular *= attenuation;
  return (ambient + diffuse + specular);
}

