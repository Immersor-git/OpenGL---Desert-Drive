#version 330
in vec3 VertexColor;
in vec3 FragPos;
layout (location=0) out vec4 FragColor;


uniform mat4 viewMatrix;
uniform mat4 projection;
uniform vec3 sunVector;
uniform vec3 cameraVector;

float width = 1000;
float height = 800;

float angleY;
float anglePerc;

vec2 uv;
vec2 sunUV;
vec2 ndcSunPos;
vec4 cameraSunPos;
vec4 clipSpaceSunPos;
vec4 cameraSpaceSunPos;
void main() {
    vec3 normalizedFragPos = normalize(FragPos);
    vec3 referenceDirection = -vec3(0,0,-1);//vec3(viewMatrix[0][2],viewMatrix[1][2],viewMatrix[2][2]);//vec3(0, 0, -1);  // Change this to your desired reference direction

    // Calculate the cosine of the angle between the fragment position and the reference direction
    float cosTheta = dot(normalizedFragPos, referenceDirection);

    // Map the cosine value from [-1, 1] to [0, 1] to use as a gradient
    float gradient = cosTheta * 0.5 + 0.5;

    gradient = clamp(1-(FragPos.y+1)/2.0,0.0,1.0);

    // Use the gradient to color the fragment
    FragColor = vec4(0.5+gradient,0.5+gradient,1.0,1.0);//vec4(vec3(gradient), 1.0);
    //FragColor = vec4(1,1,1,1);

    /*
    // Transform the sun's position to camera space
    vec4 cameraSpaceSunPos = viewMatrix * vec4(sunVector,1.0); //vec4(-sunVector, 1.0);

    // Transform the camera space position to clip space
    vec4 clipSpaceSunPos = projection * cameraSpaceSunPos;

    // Perform the perspective divide
    vec2 ndcSunPos = clipSpaceSunPos.xy / clipSpaceSunPos.w;

    vec2 uv = gl_FragCoord.xy/vec2(width,height);
    uv.y = 1-uv.y;
    // Map from [-1, 1] to [0, 1] to get the UV coordinates
    vec2 sunUV = (ndcSunPos + 1.0) / 2.0;
    vec2 dist = sunUV-uv;

    float sunColor = min(length(dist)/2.0f,1.0);

    FragColor = vec4(sunUV.x,sunUV.y,1.0,1);
    // Now you can use sunUV in your calculations
    // ...
    */
}

/*
#version 330
in vec3 FragPos;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 sunVector;

layout (location=0) out vec4 FragColor;

void main() {
    // Transform the sun's position to camera space
    vec4 cameraSpaceSunPos = view * vec4(sunVector, 1.0);

    // Transform the camera space position to clip space
    vec4 clipSpaceSunPos = projection * cameraSpaceSunPos;

    // Perform the perspective divide
    vec2 ndcSunPos = clipSpaceSunPos.xy / clipSpaceSunPos.w;

    // Map from [-1, 1] to [0, 1] to get the UV coordinates
    vec2 sunUV = (ndcSunPos + 1.0) / 2.0;

    // Now you can use sunUV in your calculations
    // ...
}
*/