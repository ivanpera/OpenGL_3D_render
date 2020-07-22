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

void main()
{
    vec3 diffuse = materialDiffuse * mix(vec4(1), texture(diffuseTexture, uvs), blendingFactors.y).xyz;
    FragColor = vec4(diffuse, materialAlpha);
}