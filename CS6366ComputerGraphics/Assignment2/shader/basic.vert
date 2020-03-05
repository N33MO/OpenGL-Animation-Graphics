#version 330 core

// Vertex Inputs
layout(location = 0) in vec3 vPosition_modelspace;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoordIn;
layout (location = 3) in vec3 tangentIn;
layout (location = 4) in vec3 biTangentIn;

out vec3 pixelPos;
out vec2 texCoord;
out mat3 TBN;
smooth out vec3 pixelNormalS;
flat out vec3 pixelNormalF;


//uniform mat4 MVP;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    mat4 mMatrix = mat4(1.0f);

    pixelPos = vec3(mMatrix * vec4(vPosition_modelspace, 1.0));

    pixelNormalS = mat3(transpose(inverse(mMatrix))) * normal;
    pixelNormalF = mat3(transpose(inverse(mMatrix))) * normal;

    // Set vertex position:
    mat4 MVP = projection * view * model;
    gl_Position = MVP * vec4(vPosition_modelspace, 1);

//    texCoord = vec2( texCoordIn.x, 1.0 - texCoordIn.y );
    texCoord = texCoordIn;

   vec3 T = normalize(vec3(vec4(tangentIn, 0.0))); // Here model* should be present
   vec3 B = normalize(vec3(vec4(biTangentIn, 0.0)));
   vec3 N = normalize(vec3(vec4(normal, 0.0)));
   TBN = mat3(T, B, N);
}
