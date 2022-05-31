#version 410 core
#define numPointLights 2
#define numSpotLights 3

vec3 getDirectionalLight(vec3 norm, vec3 viewDir, float shadow);
vec3 getPointLight(int i, vec3 norm, vec3 viewDir);
vec3 getSpotLight(int i, vec3 norm, vec3 viewDir);
float calcShadow(vec4 fragPosLightSpace);
float blinn(vec3 norm, vec3 viewDir, vec3 lightDir, float shine);
float phong(vec3 norm, vec3 viewDir, vec3 lightDir, float shine);

out vec4 FragColor;

in vec3 normal;
in vec3 posWS;
in vec2 uv;
in mat3 TBN;

struct PointLight {
    vec3 pos;
    vec3 aCol;
    vec3 dCol;
    vec3 sCol;
    float Kc;
    float Kl;
    float Ke;

    float shine;
    float specStr;
};

struct SpotLight {
    vec3 pos;
    vec3 dir;
    vec3 col;
    float Kc;
    float Kl;
    float Ke;

    float innerRad;
    float outerRad;

    float shine;
    float specStr;
};

struct Material {
    sampler2D diffuseTexture;
    sampler2D normalTexture;
    sampler2D specularTexture;
    float shine;
};

uniform vec3 viewPos;


uniform PointLight pLight[numPointLights];
uniform SpotLight sLight[numSpotLights];
uniform Material mat;

uniform vec3 dLightCol;
uniform vec3 dLightDir;
uniform float dLightShine;
uniform float dLightSpecStr;

uniform bool DL;
uniform bool PL;
uniform bool SL;
uniform bool torch;
uniform bool normalMap;

//shadow map uniforms
uniform sampler2D shadowMap;
uniform mat4 lightSpaceMatrix;
uniform int pcfLevel;

float ambientFactor = 0.5;
vec3 diffMapCol = texture(mat.diffuseTexture, uv).xyz;
float specMapCol = texture(mat.specularTexture, uv).x;

void main() {
    vec4 posLS = lightSpaceMatrix * vec4(posWS, 1.0);
    float shadow = calcShadow(posLS);

    vec3 norm = vec3(0.0);
    vec3 viewDir = normalize(viewPos - posWS);

    if (normalMap) {
        norm = texture(mat.normalTexture, uv).xyz;
        norm = norm * 2.0 - 1.0;
        norm = normalize(TBN*norm);
    } else {
        norm = normalize (normal);
    }

    vec3 result;

    if (DL) {
        result = result + getDirectionalLight(norm, viewDir, shadow);
    }
    if (PL) {
        for (int i = 0; i < numPointLights; i++) {
            result = result + getPointLight(i, norm, viewDir);
        }
    }
    if (torch) {
        result = result + getSpotLight(0, norm, viewDir);
    }
    if (SL) {
        for (int i = 1; i < numSpotLights; i++) {
            result = result + getSpotLight(i, norm, viewDir);
        }
    }

    FragColor = vec4(result, 1.0);
}

vec3 getDirectionalLight(vec3 norm, vec3 viewDir, float shadow) {
    //ambient
    vec3 ambientColour = dLightCol * diffMapCol * ambientFactor;

    //diffuse
    float diffuseFactor = max(dot(norm, -dLightDir), 0.0);
    vec3 diffuseColour = dLightCol * diffMapCol * diffuseFactor;

    //specular
    float specBlinn = blinn(norm, viewDir, dLightDir, dLightShine);
    //float specPhong = phong(norm, viewDir, dLightDir, dLightShine); //not used, legacy.
    vec3 specularColour = dLightCol * specBlinn * specMapCol;

    return ambientColour + (1.0 - shadow) * (diffuseColour + specularColour);
}

vec3 getPointLight(int i, vec3 norm, vec3 viewDir) {
    float dist = length(pLight[i].pos - posWS);
    float attn = 1.0/(pLight[i].Kc + (pLight[i].Kl*dist) + (pLight[i].Ke * (dist*dist)));
    vec3 pLightDir = normalize(pLight[i].pos - posWS);

    //ambient
    vec3 ambientColor = pLight[i].aCol * diffMapCol * ambientFactor *attn;

    //diffuse
    float diffuseFactor = max(dot(norm, pLightDir), 0.0);
    vec3 diffuseColor = pLight[i].dCol * diffMapCol * diffuseFactor * attn;

    //specular
    float phong = blinn(norm, viewDir, pLightDir, pLight[i].shine);
    vec3 specularColour = pLight[i].sCol * phong * specMapCol * attn;

    return ambientColor + diffuseColor + specularColour;
}

vec3 getSpotLight(int i, vec3 norm, vec3 viewDir) {
    float dist = length(sLight[i].pos - posWS);
    float attn = 1.0/(sLight[i].Kc + (sLight[i].Kl*dist) + (sLight[i].Ke * (dist*dist)));
    vec3 sLightDir = normalize(sLight[i].pos - posWS);

    //diffuse
    float diffuseFactor = max(dot(norm, sLightDir), 0.0);
    vec3 diffuseColour = sLight[i].col * diffMapCol * diffuseFactor * attn;

    //specular
    float specBlinn = blinn(norm, viewDir, sLightDir, sLight[i].shine);
    vec3 specularColour = sLight[i].col * specBlinn * specMapCol * attn;

    float theta = dot(-sLightDir, normalize(sLight[i].dir));
    float illumination = clamp((theta - sLight[i].outerRad) / (sLight[i].innerRad - sLight[i].outerRad), 0.0, 1.0);

    diffuseColour = diffuseColour * illumination;
    specularColour = specularColour * illumination;

    return diffuseColour + specularColour;
}

float calcShadow(vec4 fragPosLightSpace) {
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = 0.005;  
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    for(int x = -pcfLevel; x <= pcfLevel; ++x) {
        for(int y = -pcfLevel; y <= pcfLevel; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    return shadow;
}

float blinn(vec3 norm, vec3 viewDir, vec3 lightDir, float shine) {
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float specularFactor = dot(norm, halfwayDir);
    return pow(max(specularFactor, 0.0), (shine*2)/mat.shine);
}

float phong(vec3 norm, vec3 viewDir, vec3 lightDir, float shine) {
    vec3 reflectDir = reflect(-lightDir, norm);
    float specularFactor = dot(viewDir, reflectDir);
    return pow(max(specularFactor, 0.0), shine/mat.shine);
}