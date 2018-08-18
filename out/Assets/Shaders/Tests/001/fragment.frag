#version 330 core

// Input from vertex shader
in vec3 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentTexCoord;

// Output for OpenGL
out vec4 finalColor;

// Defines
#define pi 3.14159265359
#define pi2 1.57079632679
#define pi4 0.78539816339

// Uniforms
uniform sampler2D texture;

uniform float time;
uniform vec2 resolution;

uniform vec3 colorMod = vec3(1.0);

uniform float contrast = 0.0;

uniform float brightness = 0.0;

uniform struct Weather
{
    bool isVisible;
    float intensify;
    float opacity;
    float speed;
    vec2 scale;
    vec3 color;

} weather = Weather(false, 0.35, 1.0, 1.0, vec2(1.0), vec3(1.0));

uniform struct Fog
{
    bool isVisible;
    float turbulence;
    float direction;
    float opacity;
    float speed;
    vec3 color;

} fog = Fog(false, 2.0, pi4, 0.5, 16.0, vec3(0.0));

uniform float gamma = 1.0;

uniform float saturation = 0.0;
uniform int saturationType = 0;

uniform struct Vignette
{
    bool isVisible;
    bool isCircular;
    float radius;
    float softness;
    float opacity;
    vec2 offset;

} vignette = Vignette(false, true, 0.75, 0.45, 1.0, vec2(0.0));

// Functions
float Min(vec3 xyz);

float Max(vec3 xyz);

vec2 Rotate(vec2 point, float angle);

vec2 Rotate(vec2 point, vec2 origin, float angle);

vec3 LightnessSaturation(vec3 rgb, float lvl);

vec3 LuminositySaturation(vec3 rgb, float lvl);

vec3 AverageSaturation(vec3 rgb, float lvl);

float WeatherApplicator(vec2 uv, float scale, float speed);

vec3 AddWeather(vec2 fragCoord);

float Random(vec2 p);

float Interpolate(float a, float b, float x);

float InterpolatedRandom(vec2 p);

float PerlinNoise(vec2 p, float turbulence);

/* vec3 ApplyFog(vec3 rgb)
{

} */

vec3 ApplyVignette(vec3 rgb);

void main()
{
    vec4 pixel = texture2D(texture, fragmentTexCoord) * fragmentColor;

    vec2 fragCoord = fragmentPosition.xy * resolution.xy;

    // Color modificator
    pixel.rgb *= colorMod;

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

    // Weather
    if (weather.isVisible)
    {
        pixel.rgb += AddWeather(fragCoord);
    }

    // Fog
    if (fog.isVisible)
    {
        // TODO camera pos
        float noise = PerlinNoise(fragCoord - vec2(time * cos(fog.direction) * fog.speed, time * sin(fog.direction) * fog.speed), fog.turbulence);

        vec4 fogPixel = (pixel * (1.0 - fog.opacity)) + vec4(vec3(noise) * fog.color.rgb, 1.0) * fog.opacity;

        pixel = fogPixel;
    }

    // Gamma
    if (gamma != 1.0)
    {
        pixel.rgb = pow(pixel.rgb, vec3(1.0 / gamma));
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

    // Vignette
    if (vignette.isVisible)
    {
        pixel.rgb = ApplyVignette(pixel.rgb);
    }

    finalColor = pixel;
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

float WeatherApplicator(vec2 uv, float scale, float speed)
{
	float w = smoothstep(1.0, 0.0, -uv.y * (scale / 10.0));

    float timeModified = time * speed;

	uv += timeModified / scale;
    uv.y += timeModified * 2.0 / scale;
    uv.x += sin(uv.y + timeModified * 0.5) / scale;
	uv *= scale;

    vec2 f = fract(uv);
	vec2 p = 0.5 + weather.intensify * sin(11.0 * fract(sin((floor(uv) + scale) * mat2(7.0, 3.0, 6.0, 5.0)) * 5.0)) - f;
    float d = length(p);
	float k = smoothstep(0.0, min(d, 3.0), sin(f.x + f.y) * 0.01);

    return k * w;
}

vec3 AddWeather(vec2 fragCoord)
{
    // TODO camera pos

    vec2 uv = fragCoord / min(resolution.x, resolution.y) * weather.scale;

    float mask = 0.0;

    mask += WeatherApplicator(uv, 30.0, weather.speed) * 0.3;
    mask += WeatherApplicator(uv, 20.0, weather.speed) * 0.5;
    mask += WeatherApplicator(uv, 15.0, weather.speed) * 0.8;
    mask += WeatherApplicator(uv, 10.0, weather.speed);
    mask += WeatherApplicator(uv, 8.0, weather.speed);
    mask += WeatherApplicator(uv, 6.0, weather.speed);
    mask += WeatherApplicator(uv, 5.0, weather.speed);

    return vec3(mask) * weather.color * weather.opacity;
}

float Random(vec2 p)
{
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453);
}

float Interpolate(float a, float b, float x)
{
    float f = (1.0 - cos(x * pi)) * 0.5;
    return a * (1.0 - f) + b * f;
}

float InterpolatedRandom(vec2 p)
{
    vec2 i = floor(p);
    vec2 f = fract(p);
    vec4 v = vec4(Random(vec2(i.x, i.y)), Random(vec2(i.x + 1.0, i.y)), Random(vec2(i.x, i.y + 1.0)), Random(vec2(i.x + 1.0, i.y + 1.0)));

    return Interpolate(Interpolate(v.x, v.y, f.x), Interpolate(v.z, v.w, f.x), f.y);
}

float PerlinNoise(vec2 p, float turbulence)
{
    float t = 0.0;

    for (int i = 0; i < 8; ++i)
    {
        float freq = pow(turbulence, float(i));
        float amp = pow(0.5, float(8 - i));
        t += InterpolatedRandom(p / freq) * amp;
    }

    return t;
}

vec3 ApplyVignette(vec3 rgb)
{
	vec2 position = fragmentPosition.xy + (vignette.offset.xy / resolution.xy);

    if (vignette.isCircular)
    {
        position.x *= resolution.x / resolution.y;
    }

	float factor = smoothstep(vignette.radius, vignette.radius - vignette.softness, length(position));

	return mix(rgb, rgb * factor, vignette.opacity);
}
