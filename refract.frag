float fresnel(float VdotN, float eta)
{
    float sqr_eta = eta * eta;
    float etaCos = eta * VdotN;
    float sqr_etaCos = etaCos*etaCos;
    float one_minSqrEta = 1.0 - sqr_eta;
    float value = etaCos - sqrt(one_minSqrEta + sqr_etaCos);
    value *= value / one_minSqrEta;
    return min(1.0, value * value);
}

const float tracePrecision  = 0.005;
int trace(inout vec3 origin, in vec3 dir, int maxIterations);
vec3 normalAt(vec3 p);
vec3 colorAt(int object, COLORSPEC);
float distanceAt(vec3 p, out int object);

vec3 refract_color(COLORCALL, float indexOfRefraction, int reflectLimit, int refractLimit)
{
    vec3 cReflected, cRefracted;

    vec3 reflDir = normalize(reflect(view, normal));
    vec3 reflPos = point + reflDir * (tracePrecision * 4.0);
    int reflObj = trace(reflPos, reflDir, reflectLimit);

    vec3 reflNormal = normalAt(reflPos);
    vec3 reflShift = reflPos - point;
    vec3 reflView = reflShift - view;
    vec3 reflLight = reflShift - light;
    // FIXME: don't allow recursion
    cReflected = colorAt(reflObj, reflPos, reflNormal, reflView, reflLight);

    // Now trace refraction inside object
    vec3 refrDir = normalize(refract(view, normal, indexOfRefraction));
    vec3 refrPos = origin + refrDir * (tracePrecision * 2.0);
    int refrObj = object;

    int iteration = 0;
    while (++iteration < reflectLimit && refrObj == object) {
        float distance = abs(distanceAt(refrPos, refrObj));
        refrPos += refrDir * distance;

        if (distance < tracePrecision) {
            vec3 N = normalAt(refrPos);
            float index = indexOfRefraction;
            if (dot(N, refrDir) > 0.0) {
                N = -N;
                index = 1.0 / index;
            }
            if (length(N) > 0.1) {
                vec3 newDir = normalize(refract(refrDir, N, index));
                if (length(newDir) > 0.1)
                    refrDir = newDir;
            }

            refrPos += refrDir * (tracePrecision * 2.0);
        }
    }

    // Finally, trace it to another object
    refrObj = trace(refrPos, refrDir, refractLimit);
    vec3 refrNormal = normalAt(refrPos);
    vec3 refrShift = refrPos - point;
    vec3 refrView = refrShift - view;
    vec3 refrLight = refrShift - light;
    // FIXME: don't allow recursion
    cRefracted = colorAt(refrObj, refrPos, refrNormal, refrView, refrLight);

    float fFresnel = fresnel(dot(-view, normal), indexOfRefraction);
    return mix(cRefracted, cReflected, fFresnel);
}
