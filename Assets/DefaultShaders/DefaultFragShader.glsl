#version 440 core

#define PI 3.141592

in vec4 out_Color;

out vec4 FragColor;

uniform float Time;
uniform vec2 Resolution;

vec3 palette(float t) 
{
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263,0.416,0.557);

    return a + b*cos(3.1415926 * (c * t + d));
}

void main() 
{
    vec2 uv = (gl_FragCoord.xy * 2.0 - Resolution.xy) / Resolution.y;
    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);
    
    for (float i = 0.0; i < 4.0; i++) 
    {
        uv = fract(uv * 1.5) - 0.5;

        float d = length(uv) * exp(-length(uv0));

        vec3 col = palette(length(uv0) + i * .4 + Time * .4);

        d = sin(d * 8. + Time) / 8.;
        d = abs(d);

        d = pow(0.01 / d, 1.2);
        d = smoothstep(0.0, 2.0, d);

        finalColor += col * d;
    }

    FragColor = vec4(finalColor, 1.0);
}