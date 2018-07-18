#version 330 core

// Definitions and types
#define MAX_LIGHTS 20
#define ENABLE_ATTENUATION

struct Attenuation
{
    float constant;
    float linear;
    float quadratic;
};

struct PointLight
{
    // Standard
    vec3 position;
    vec3 color;

    float colorR;
    float colorG;
    float colorB;

    // Attenuation
    Attenuation attenuation;
    
    // Light factors
    vec3 ambientFactor;
    vec3 diffuseFactor;
#ifdef ENABLE_SPECULAR
    vec3 specularFactor;
#endif
};

struct Material
{
    sampler2D diffuseTexture;
#ifdef ENABLE_SPECULAR
    sampler2D specularTexture;
    float shininess;
#endif
};



// Fragment
in vec3 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentTexCoord;

// Color output
out vec4 finalColor;

// Texture data
uniform bool isObject = false;
uniform Material material;

// Lighting
uniform vec3 cameraPosition;
uniform vec3 basicAmbient = vec3(1.0, 1.0, 1.0);

uniform uint pointLightsLength = 0u;
uniform PointLight pointLights[MAX_LIGHTS];



// Declare functions 
vec3 calucaltePointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 cameraDirection);

// Main shader function
void main()
{
    if (isObject) {
        // Objects of the world are affected by lighting
        vec3 normal = normalize(vec3(0, 0, 0));
        vec3 cameraDirection = normalize(cameraPosition - fragmentPosition);

        vec4 pixel = texture2D(material.diffuseTexture, fragmentTexCoord);
        
        if (pixel.a == 0) {
            // Do nothing, when fully transparent
            discard;
        } else {
            // Basic ambient color
            vec3 color = pixel.rgb * basicAmbient;

            // Point lights
            for (uint i = 0u; i < pointLightsLength; ++i) {
                color += calucaltePointLight(pointLights[i], normal, fragmentPosition, cameraDirection);
            }

            // Apply transparency
            finalColor = vec4(color, pixel.a);
        }
    } else {
        // Make full color for non-objects
        finalColor = fragmentColor;
    }
}



// Lighting functions
vec3 calucaltePointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 cameraDirection)
{
    // Distance
#ifdef ENABLE_ATTENUATION
    float lightDistance = distance(light.position, fragmentPosition);
    float attenuation = 1.0 / (
        // x^0
        light.attenuation.constant + 
        lightDistance * (
            // x^1
            light.attenuation.linear + 
            lightDistance * (
                // x^2
                light.attenuation.quadratic
            )
        )
    );
#endif

    // Direction
    vec3 lightDirection = normalize(light.position - fragmentPosition);
    float diffusePositionFactor = max(dot(normal, lightDirection), 0.0);
    
#ifdef ENABLE_SPECULAR
    if (material.shininess != 0.0) {
        vec3 reflectDirection = reflect(-lightDirection, normal);
        float specularPositionFactor = pow(max(dot(cameraDirection, reflectDirection), 0.0), material.shininess);
    }
#endif

    // Combined factors
    vec3 result;

    result = (
        light.ambientFactor +
        light.diffuseFactor * diffusePositionFactor
    ) * texture2D(material.diffuseTexture, fragmentTexCoord).rgb;

#ifdef ENABLE_SPECULAR
    result += (
        light.specularFactor * specularPositionFactor
    ) * texture2D(material.specularTexture, fragmentTexCoord).rgb;
#endif

#ifdef ENABLE_ATTENUATION
    result *= attenuation;
#endif

    // Return result
    return result * light.color;
}


