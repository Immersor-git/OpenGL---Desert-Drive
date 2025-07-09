#version 330
// A fragment shader for rendering fragments in the Phong reflection model.
layout (location=0) out vec4 FragColor;

// Inputs: the texture coordinates, world-space normal, and world-space position
// of this fragment, interpolated between its vertices.
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragWorldPos;
in vec4 FragPosLightSpace;
in vec3 Tangent;

uniform mat4 model;
vec3 AdjustedFragWorldPos;
vec4 AFragPosLightSpace;

// Uniforms: MUST BE PROVIDED BY THE APPLICATION.

// The mesh's base (diffuse) texture.
uniform sampler2D baseTexture;
uniform sampler2D specMap;
uniform sampler2D normalMap;
uniform sampler2D shadowMap;
uniform sampler2D heightMap;
uniform bool hasHeightmap;
uniform bool includeLighting;

// Material parameters for the whole mesh: k_a, k_d, k_s, shininess.
uniform vec4 material;

// Ambient light color.
uniform vec3 ambientColor;

// Direction and color of a single directional light.
uniform vec3 directionalLight; // this is the "I" vector, not the "L" vector.
uniform vec3 directionalColor;

// Location of the camera.
uniform mat4 view;

float cosine;
float lamber_factor;

vec3 ambientIntensity;
float diffuseIntensity;
vec3 specularIntensity;

vec3 spec_factor;
vec3 sampledNormal;
vec3 normal;

vec4 shininess;

uniform mat4 lights[10];
vec3 _pl_intensity;
float sceneBrightness = 0.5;

vec3 lPosition;
vec3 lColor;
float lType;
float lRange;
float lCutoffAngle;
vec3 lLookVec;

vec3 light_vector;
vec3 light_direction;
float distance;
float distanceMult;
float dotProduct;
float angleDist;

float spec;

vec3 view_vector;
vec3 reflect_vector;
vec3 light_intensity;

vec3 projCoords;
vec2 shadowMapUV;

float nshadowDepth;

float lineearDepth;
float ndc;

int width = 1200;
int height = 800;

vec3 shadowDepth;
float bias;
float closestDepth;
float currentDepth;
vec3 lightCoords;

int sampleRadius = 2;
float sampleAverageMult = 1.0/pow(sampleRadius * 2 + 1, 2);

vec4 parallaxTextureColor;

vec2 newTexCoord;

float calculateShadow() {
    lightCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    if (lightCoords.z <= 1.0f) {
        lightCoords = lightCoords * 0.5 + 0.5;
        //closestDepth = texture(shadowMap, lightCoords.xy).r;
        currentDepth = lightCoords.z;

        bias = max(0.005f * (lamber_factor), 0.0005f);
        //if (currentDepth > closestDepth + bias) {
        //    return 1.0f;
        //}
        float sampleDepth;
        float shadow = 0.0;
        vec2 pixelSize = 1.0/textureSize(shadowMap, 0);
        for (int x = -sampleRadius; x <= sampleRadius; x++) {
            for (int y = -sampleRadius; y <= sampleRadius; y++) {
                vec2 offset = vec2(x, y) * pixelSize;
                sampleDepth = texture(shadowMap, lightCoords.xy + offset).r;
                if (currentDepth > sampleDepth + bias) {
                    shadow += 1.0f;
                }
            }
        }
        return shadow * sampleAverageMult;
    }
    return 0.0f;//abs(currentDepth - closestDepth); //0.0f;
}

void calculatePointLight(mat4 light) {
    lPosition = light[0].xyz;
    lColor = light[1].xyz;
    lType = light[2].x - 1;
    lRange = light[2].y;
    lCutoffAngle = light[2].z;
    lLookVec = light[3].xyz;

    if (lType < 0.01) { // Direcitonal light becomes a direction rather than a position
        light_vector = -normalize(lLookVec);
        light_direction = -normalize(lLookVec);
        distance = 0.0f;
        lRange = 10.0f;
    } else {
        light_vector = lPosition - AdjustedFragWorldPos;
        light_direction = normalize(light_vector);
        distance = length(light_vector);
    }
    distanceMult = 1.0f - pow( min(distance/lRange, 1.0f), 2.0f);
    if (distanceMult == 0.0f) return;
    //Diffuse intensity
    cosine = dot(normal, light_direction);
    angleDist = acos(cosine);
    lamber_factor = 1-pow(clamp(angleDist/radians(90.0f),0,1),2);//min(max(cosine, 0),1);
    //lamber_factor = min(max(cosine, 0),1);
    diffuseIntensity = lamber_factor;//vec3(lamber_factor);

    //Specular intensity
    view_vector = vec3(-transpose(mat3(view)) * view[3].xyz) - AdjustedFragWorldPos;
    reflect_vector = reflect(-light_direction, normal);
    cosine = dot(reflect_vector, normalize(view_vector));
    //angleDist = acos(cosine);
    spec = pow(clamp(cosine,0,1), 15) * 4 * shininess.x; // * distanceMult * shininess.x;
    //if (cosine < 0) {
    //    spec = 0.0;
    //}


    //light_intensity;
    if (lType < 0.01) {
        //light_intensity = vec3(1);
        light_intensity = vec3(1-calculateShadow());
    } else if (lType < 1.01) {
        light_intensity = vec3(distanceMult);
    } else if (lType < 2.01) {
        dotProduct = dot(-light_direction, lLookVec);
        angleDist = acos(dotProduct);
        if (angleDist > radians(lCutoffAngle)) {
            light_intensity = vec3(0);
            spec = 0.0f;
        } else {
            //diffuseIntensity = diffuseIntensity * (1-distanceMult);
            diffuseIntensity = 1.0f;
            cosine = pow(1-clamp(angleDist/radians(lCutoffAngle),0,1),1);
            spec = spec * cosine;
            light_intensity = vec3(cosine * distanceMult);//vec3(clamp(pow(cosAngle - cosCutoff, 2),0,1)) *
            // If the fragment is outside the light's cone, return 0
        }
    }

    //light_intensity = vec3(1.0f);

    _pl_intensity = _pl_intensity + light_intensity * (diffuseIntensity + spec) * lColor; //* lColor;// + spec_factor);// + specularIntensity); //* color;
}
int numSteps;
vec2 S;
vec2 deltaUVs;
void getHeightMap()
{
    if (!hasHeightmap) return;
    vec3 viewVector2 =  -normalize(vec3(-transpose(mat3(view)) * view[3].xyz) - AdjustedFragWorldPos) ;//(CameraPos) //-transpose(mat3(view))[2];
    //float dist = length(vec3(-transpose(mat3(view)) * view[3].xyz) - FragWorldPos);
    float height = 1-(texture(heightMap, TexCoord).r); // Get the height at the current fragment
    newTexCoord = TexCoord; // Start with the original texture coordinates
    const int numSteps = 5; // The number of steps to take in the ray march
    float currentHeight = 0.0;
    //vec2 S = viewVector2.xy / viewVector2.z * 0.15;
    vec2 S = vec2(0.05);
    //const float minLayers = 9.0f;
   // condst float maxLayers = 64.0f;
    //float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0f, 0.0f, 1.0f), viewDirection)));
    vec2 deltaUVs = S/numSteps;
    int layer = 0;
    for (int i = 0; i < numSteps; ++i)
    {
        layer = i;
        currentHeight = 1-(texture(heightMap, newTexCoord).r); // Get the height at the current texture coordinates
        if (currentHeight > height)
        {
            // If the current height is greater than the original height, we've hit the heightmap
            break;
        }

        // Move the texture coordinates along the view vector
        newTexCoord -= deltaUVs*viewVector2.xz; //(height - currentHeight) * viewVector2.xy;
        //AdjustedFragWorldPos += (height - currentHeight) * viewVector;
    }

    //vec2 prevTexCoord = newTexCoord + deltaUVs;
    //vec2 prevDepth = 1.0-(texture(heightMap, prevTexCoord).r);
    //newTexCoord = (prevTexCoord + newTexCoord) / 2.0;

    //if (newTexCoord.y < 0.0 || newTexCoord.y > 1.0 || newTexCoord.x < 0.0 || newTexCoord.x > 1.0) {
    //    discard;
    //}

    AdjustedFragWorldPos = FragWorldPos + vec3(0, currentHeight, 0);
    AFragPosLightSpace = FragPosLightSpace + vec4(0, currentHeight, 0, 0);
    //AdjustedFragWorldPos = FragWorldPos + vec3(0,currentHeight,0);
    //AFragPosLightSpace = FragPosLightSpace + vec4(0,currentHeight,0,0);
    // Sample the base texture at the new texture coordinates
    parallaxTextureColor = texture(baseTexture, newTexCoord); //* (1-abs(height-currentHeight)*15.0);

    //FragColor = baseColor;
}
/*
vec3 T;
vec3 N;
vec3 B;
mat3 TBN;
vec3 worldNormal;


void adjustNormal() {
    // Sample the normal from the normal map
    vec3 sampledNormal = texture(normalMap, newTexCoord).rgb;

    // Convert the sampled normal from [0,1] to [-1,1]
    sampledNormal = normalize(sampledNormal * 2.0 - 1.0);

    // Calculate the TBN matrix
    T = normalize(vec3(model * vec4(Tangent, 0.0)));
    N = normalize(vec3(model * vec4(Normal, 0.0)));
    B = cross(N, T);
    TBN = mat3(T, B, N);

    // Convert the sampled normal from tangent space to world space
    worldNormal = normalize(TBN * sampledNormal);

    // Use the world normal in your lighting calculations
    normal = worldNormal;

    // ... rest of your shader code ...
}
*/
void main() {
    // Use the converted color as your normal.
    //newNormal = Normal;//normalize(sampledNormal * 0.5 + Normal * 0.5);
    normal = normalize(Normal);
    AdjustedFragWorldPos = FragWorldPos;
    AFragPosLightSpace = FragPosLightSpace;
    parallaxTextureColor = texture(baseTexture, TexCoord);
    newTexCoord = TexCoord;
    //adjustNormal;
        getHeightMap();
        //adjustNormal();
        //return;
    if (!includeLighting) return;
    _pl_intensity = vec3(0.0);//Total brightness / color of the fragment

    //Light Test//
        //normal = normalize(Normal);
        shininess = texture(specMap, newTexCoord);
        for (int i = 0; i < 10; i++) {
            if (lights[i][2][0] == 0) {
                break;
            }
            calculatePointLight(lights[i]);
        }
        FragColor = (vec4(_pl_intensity * sceneBrightness, 1) + vec4(0.45,0.45,0.4,0.0)) * parallaxTextureColor  ;//* (texture(baseTexture, TexCoord));

        //shadowMapUV = (gl_FragCoord.xy) / vec2(width, height);
        //FragColor = texture(shadowMap,shadowMapUV)/4.0;
    //FragColor = vec4(1,1,1,1);
}