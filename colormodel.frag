vec3 phong(vec3 normal, vec3 view, vec3 color, float shininness)
{
    vec3 R = normalize(reflect(view, normal));
    return color * pow( max(dot(R, normal), 0.0), shininness );
}

vec3 lambert(vec3 normal, vec3 light, vec3 color)
{
    return color * max(dot(light, normal), 0.2);
}
