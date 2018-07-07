

/*
    - Gaussian blur
    - Bloom
    - Contrast
    - Brightness
    - Gamma
    - LightnessSaturation
    - LuminositySaturation
    - Fog with PerlinNoise
    - Vignetting
*/

#version 330 core

// Input from vertex shader
in vec3 position;
in vec4 color;
in vec2 texCoord;

// Output for OpenGL
out vec4 FragColor;

// Bound texture
uniform sampler2D boundTexture;
uniform bool isTextured;

// Defines
#define pi 3.14159265359
#define pi2 1.57079632679
#define pi4 0.78539816339

// Post processing
uniform vec2 resolution;
uniform float time;

uniform bool blurHV = true;
uniform float blur = 1.0;
uniform float bloom = 1.0;

uniform float contrast = 0.0;

uniform float brightness = 0.0;

uniform float gamma = 1.0;

uniform float saturation = 0.0;
uniform int saturationType = 0;

uniform vec3 fogSeed = vec3(12.9898, 78.233, 43758.5453);
uniform vec4 fogColor = vec4(0.0);
uniform float fogTurbulence = 2.0;
uniform float fogDirection = pi4;
uniform float fogSpeed = 16.0;
uniform bool fogReverted = false;

uniform float vignettingStrength = -1.0;

// Functions
float Min(vec3 xyz);

float Max(vec3 xyz);

vec2 Rotate(vec2 point, float angle);

vec2 Rotate(vec2 point, vec2 origin, float angle);

vec3 LightnessSaturation(vec3 rgb, float lvl);

vec3 LuminositySaturation(vec3 rgb, float lvl);

vec3 AverageSaturation(vec3 rgb, float lvl);

float Random(vec2 p, vec3 seed);

float Interpolate(float a, float b, float x);

float InterpolatedRandom(vec2 p, vec3 seed);

float PerlinNoise(vec2 p, float turbulence, vec3 seed, bool reverted);

void main()
{
    if (isTextured)
    {
        // Vars
        vec4 pixel = texture(boundTexture, texCoord);
        vec2 sampleSize = vec2(1.0) / resolution;

        //if (pixel.a == 0.0) discard;

        // Gaussian blur
        int blurSteps = int(min(blur * 0.7, sqrt(blur) * pi4));
        float blurFactor = blur / blurSteps;
        float bloomFactor = bloom / (blurSteps * 2 + 1);

        pixel.rgb *= bloomFactor;

        if(blurHV)
        {
            for (int i = 1; i <= blurSteps; ++i)
            {
                float v = (cos(i / (blurSteps + 1.0) / pi) + 1.0) * 0.5;
                pixel.rgb += texture(boundTexture, vec2(texCoord.x + i * sampleSize.x * blurFactor, texCoord.y)).rgb * v * bloomFactor;
                pixel.rgb += texture(boundTexture, vec2(texCoord.x - i * sampleSize.x * blurFactor, texCoord.y)).rgb * v * bloomFactor;
            }
        }
        else
        {
        	for (int i = 1; i <= blurSteps; ++i)
            {
        		float v = (cos(i / (blurSteps + 1.0) / pi) + 1.0) * 0.5;
        		pixel.rgb += texture(boundTexture, vec2(texCoord.x, texCoord.y + i * sampleSize.y * blurFactor)).rgb * v * bloomFactor;
        		pixel.rgb += texture(boundTexture, vec2(texCoord.x, texCoord.y - i * sampleSize.y * blurFactor)).rgb * v * bloomFactor;
        	}
        }

        // Contrast
        if (contrast != 0.0)
        {
            pixel.rgb = (pixel.rgb - 0.5) * tan((contrast + 1.0) * pi4) + 0.5;
        }

        // Brightness
        if (brightness < 0.0)
        {
            pixel.rgb *= 1.0 + brightness;
        }
        else if (brightness > 0.0)
        {
            pixel.rgb += (1.0 - pixel.rgb) * brightness;
        }

        // Gamma
        if (gamma != 1.0)
        {
            pixel.rgb = pow(pixel.rgb, vec3(1.0 / gamma));
        }

        // Fog
        if (fogColor.a != 0.0)
        {
            float noise = PerlinNoise(position.xy - vec2(time * cos(fogDirection) * fogSpeed, time * sin(fogDirection) * fogSpeed), fogTurbulence, fogSeed, fogReverted);
            pixel = (pixel * (1.0 - fogColor.a)) + vec4(vec3(noise) * fogColor.rgb, 1.0) * fogColor.a;
        }

        // Saturation
        if (saturation != 0.0)
        {
            if (saturationType == 0)
            {
                pixel.rgb = LightnessSaturation(pixel.rgb, saturation);
            }
            else if (saturationType == 1)
            {
                pixel.rgb = LuminositySaturation(pixel.rgb, saturation);
            }
            else
            {
                pixel.rgb = AverageSaturation(pixel.rgb, saturation);
            }
        }

        // Vignetting
        if (vignettingStrength >= 0.0)
        {
            pixel.rgb *= vignettingStrength - distance(vec2(0.5, 0.5), texCoord);
        }

        // Final color
        FragColor = pixel * color;
    }
    else
    {
        FragColor = color;
    }
}

float Min(vec3 xyz)
{
    return min(min(xyz.x, xyz.y), xyz.z);
}

float Max(vec3 xyz)
{
    return max(max(xyz.x, xyz.y), xyz.z);
}

vec2 Rotate(vec2 point, float angle)
{
	return vec2(cos(angle) * point.x - sin(angle) * point.y, sin(angle) * point.x + cos(angle) * point.y);
}

vec2 Rotate(vec2 point, vec2 origin, float angle)
{
	return vec2(cos(angle) * (point.x - origin.x) - sin(angle) * (point.y - origin.y) + origin.x, sin(angle) * (point.x - origin.x) + cos(angle) * (point.y - origin.y) + origin.y);
}

vec3 LightnessSaturation(vec3 rgb, float lvl)
{
    return mix(rgb, 0.5 * vec3(Min(rgb) + Max(rgb)), lvl);
}

vec3 LuminositySaturation(vec3 rgb, float lvl)
{
    return mix(rgb, vec3(rgb.r * 0.21 + rgb.g * 0.72 + rgb.b * 0.07), lvl);
}

vec3 AverageSaturation(vec3 rgb, float lvl)
{
    return mix(rgb, vec3(rgb.r + rgb.g + rgb.b) / 3.0, lvl);
}

float Random(vec2 p, vec3 seed)
{
    return fract(sin(dot(p, seed.xy)) * seed.z);
}

float Interpolate(float a, float b, float x)
{
    float f = (1.0 - cos(x * pi)) * 0.5;
    return a * (1.0 - f) + b * f;
}

float InterpolatedRandom(vec2 p, vec3 seed)
{
    vec2 i = floor(p);
    vec2 f = fract(p);
    vec4 v = vec4(Random(vec2(i.x, i.y), seed), Random(vec2(i.x + 1.0, i.y), seed), Random(vec2(i.x, i.y + 1.0), seed), Random(vec2(i.x + 1.0, i.y + 1.0), seed));

    return Interpolate(Interpolate(v.x, v.y, f.x), Interpolate(v.z, v.w, f.x), f.y);
}

float PerlinNoise(vec2 p, float turbulence, vec3 seed, bool reverted)
{
    float t = reverted ? 1.0 : 0.0;

    for (int i = 0; i < 8; ++i)
    {
        float freq = pow(turbulence, float(i));
        float amp = pow(0.5, float(8 - i));
        t += InterpolatedRandom(p / freq, seed) * amp * (reverted ? -1.0 : 1.0);
    }

    return t;
}
