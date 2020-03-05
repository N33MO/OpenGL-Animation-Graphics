#version 330 core

struct Dir_Light{
    bool status;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Point_Light{
    bool status;
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct Material{
    vec3 object_color;
    float shininess;
};

in vec3 pixelPos;
in vec2 texCoord;
smooth in vec3 pixelNormalS;
flat in vec3 pixelNormalF;

in mat3 TBN;

uniform bool FlatOn;
uniform bool DiffuseOn;
uniform bool NormalOn;

//uniform vec3 modelColor;
//uniform float shininess;
uniform Material m_object;

uniform vec3 camera_pos;

//uniform vec3 dDirection; // dir_light.direction
//uniform vec3 dLightAmbient; // dir_light.ambient
//uniform vec3 dLightDiffuse; // dir_light.diffuse
//uniform vec3 dLightSpecular; // dir_light.specular
uniform Dir_Light dir_light;

//uniform vec3 pLightloc; // point_light.position
//uniform vec3 pLightAmbient; // point_light.ambient
//uniform vec3 pLightDiffuse; // point_light.diffuse
//uniform vec3 pLightSpecular; // point_light.specular
uniform Point_Light point_light;

// Texture sampler:
uniform sampler2D TexDiffuse;
uniform sampler2D TexNormal;

out vec4 color; // Color output from the frag shader.

void main(){
    // Set the pixel normal as Flat/Smooth:
    vec3 pixelNormal;
    if(FlatOn == true){
        pixelNormal = pixelNormalS;
    }
    else{
        pixelNormal = pixelNormalF;
    }

    if(NormalOn){
        pixelNormal = texture(TexNormal, texCoord).rgb; // TexNormal
        pixelNormal = normalize(pixelNormal * 2.0 - 1.0);
        pixelNormal = normalize(TBN * pixelNormal);
    }

    // Determine vector toward each light
    //vec3 dDirection = vec3(0, 1, 1);
    //dDirection = normalize(dDirection);
    vec3 pDirection = normalize(point_light.position - pixelPos);

    // Ambient
    vec3 ambient = dir_light.ambient * 1.0f;
    vec3 pAmbient = point_light.ambient * 1.0f;
    ambient += pAmbient;

    // Diffuse
    vec3 diffuse = max(dot(pixelNormal, dir_light.direction), 0.0f) * dir_light.diffuse;
    vec3 pDiffuse = max(dot(pixelNormal, pDirection), 0.0f) * point_light.diffuse;
    diffuse += pDiffuse;

    // Specular Directional:
    vec3 viewVector = normalize(camera_pos - pixelPos);
    vec3 h = normalize(dir_light.direction + viewVector);
    vec3 reflectVector = reflect(-1*dir_light.direction, pixelNormal);
    float fSpecular = pow(dot(h, pixelNormal), m_object.shininess);
    vec3 specular = dir_light.specular * fSpecular;
    
    // Specular Point:
    vec3 pH = normalize(pDirection + viewVector);
    vec3 pReflectVector = reflect(-1*pDirection, pixelNormal);
    float pFSpecular = pow(dot(pH, pixelNormal), m_object.shininess);
    vec3 pSpecular = point_light.specular * pFSpecular;
    specular += pSpecular;

    if(DiffuseOn){
        color = vec4((ambient + diffuse + specular)*texture(TexDiffuse, texCoord).rgb, 1.0f);
    }
    else{
        // Blinn-Phong Model:
        color = vec4((ambient + diffuse + specular)*m_object.object_color, 1.0f);
    }
}
