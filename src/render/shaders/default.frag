#version 330 core

layout(location = 0) out vec4 colour;

in vec3 v_pos;
in vec3 v_norm;

uniform vec3 uLightPos;
uniform vec3 uLightColour;
uniform float uLightPower;

uniform vec3 uAmbientColour = vec3(0.1, 0.0, 0.0);
uniform vec3 uDiffuseColour = vec3(0.5, 0.0, 0.0);
uniform vec3 uSpecColour = vec3(1.0, 1.0, 1.0);
uniform float uShininess = 16.0;

const float screenGamma = 2.2;

void main()
{
  vec3 lightDir = uLightPos - v_pos;
  float dist = length(lightDir);
  dist = dist * dist;
  lightDir = normalize(lightDir);

  float lambertian = max(dot(lightDir, v_norm), 0.0);
  float specular = 0.0;

  if (lambertian > 0.0) {

    vec3 viewDir = normalize(-v_pos);
    vec3 halfDir = normalize(lightDir + viewDir);
    float specAngle = max(dot(halfDir, v_norm), 0.0);
    specular = pow(specAngle, uShininess);
  }
  vec3 colourLinear = uAmbientColour +
                     uDiffuseColour * lambertian * uLightColour * uLightPower / dist +
                     uSpecColour * specular * uLightColour * uLightPower / dist;

  vec3 colourGammaCorrected = pow(colourLinear, vec3(1.0 / screenGamma));

  gl_FragColor = vec4(colourGammaCorrected, 1.0);
}
