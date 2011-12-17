const float density = %1;
const vec3 fogColor = %2;

vec3 applyFog(vec3 color, float distance)
{
    const float LOG2 = 1.442695;
    float fogFactor = exp2( -density * density * distance * distance * LOG2 );
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    return mix(fogColor, color, fogFactor);
}
