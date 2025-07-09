#version 330
// A vertex shader for rendering vertices with normal vectors and texture coordinates,
// which creates outputs needed for a Phong reflection fragment shader.
layout (location=0) in vec3 vPosition;
layout (location=1) in vec3 vNormal;
layout (location=2) in vec2 vTexCoord;
//layout (location=3) in vec3 vTangent;

uniform mat4 projection;
uniform mat4 shadowProjection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 LightSpaceMatrix;
uniform bool includeLighting;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragWorldPos;
out vec4 FragPosLightSpace;
//out vec3 Tangent;

void main() {
    // Transform the position to clip space.
    gl_Position = (includeLighting ? projection : shadowProjection) * view * model * vec4(vPosition, 1.0);
    TexCoord = vTexCoord;
    Normal = mat3(transpose(inverse(model))) * vNormal;
    FragWorldPos = vec3(model * vec4(vPosition, 1.0));
    FragPosLightSpace =  LightSpaceMatrix * vec4(FragWorldPos,1.0);
    //Tangent = vTangent;
}