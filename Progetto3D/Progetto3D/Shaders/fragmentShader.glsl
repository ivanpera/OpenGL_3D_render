#version 400 core
in vec4 ourColor;
in vec3 pos;
in vec3 N;
in vec2 uvs;
out vec4 FragColor;  

uniform vec3 cameraPos;
uniform int t;
uniform int numLights;
uniform float dt;

uniform vec3 blendingFactors;
uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D alphaTexture;

uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialShininess;
uniform float materialAlpha;

#define MAX_LIGHTS 20

uniform struct Light {
    vec3 position;
    vec3 colorAmbient;
    vec3 colorDiffuse;
    vec3 colorSpecular;
    float constant;
    float linear;
    float quadratic;
} lights[MAX_LIGHTS];

vec3 applyLight(Light light, vec3 pos, vec3 cameraPos, vec3 N) {
    vec3 L = normalize(light.position.xyz - pos);
    vec3 V = normalize(cameraPos - pos);
    float distanceToLight = length(light.position.xyz - pos);
    float attenuation = 1.0 / (light.constant + light.linear * distanceToLight + light.quadratic * pow(distanceToLight,2));

    vec3 R = -normalize(reflect(L,N));

    vec3 ambient  = light.colorAmbient  * materialAmbient;
    vec3 diffuse  = light.colorDiffuse  * materialDiffuse  * max(dot(L,N), 0.0);
    diffuse *= mix(vec4(1), texture(diffuseTexture, uvs), blendingFactors.y).xyz;
    vec3 specular = light.colorSpecular * materialSpecular * pow(max(dot(R,V), 0.0), materialShininess);

    return ambient + attenuation * (diffuse + specular);
} 

void main()
{
    vec3 lightingColor = vec3(0.0);
    for (int i = 0; i < numLights; i++) {
        lightingColor += applyLight(lights[i], pos, cameraPos, N);
    }
    FragColor = vec4(lightingColor, materialAlpha);
}