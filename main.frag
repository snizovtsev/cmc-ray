#version 120
//
// Parameters
//

varying vec2 pixel;
uniform float aspectRatio;
uniform mat4 matrix;
uniform sampler2D floorTex;

const int maxIterations = 150;
const float tracePrecision = 0.005;

const vec3  environment = vec3(0.0);
const int   skyObject   = -1;
const float skyDistance = 500.0;
const int   noneObject  = -2;

int iteration = 0; // Current resource usage

//
// Imports
//

float distanceAt(vec3 p, out int object);
vec3 normalAt(vec3 p);
float shadowAt(vec3 p, vec3 n, vec3 light);

int trace(inout vec3 origin, in vec3 dir)
{
    float totalDistance = 0.0;

    while (++iteration < maxIterations) {
        int nearObject;
        float distance = distanceAt(origin, nearObject);

        origin += dir * distance;
        totalDistance += distance;

        if (totalDistance >= skyDistance)
            return skyObject;

        if (distance < tracePrecision)
            return nearObject;
    }

    // Iteration limit exceeded
    return noneObject;
}

//
// Materials
//

struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininness;
    float refraction;
    float indexOfRefraction;
};

const Material material[] = Material[7] (
    Material(vec3(.5), vec3(.1), 0.07 * 128, 0.0, 0.0), // type1 floor
    Material(vec3(1.0), vec3(.1), 0.07 * 128, 0.0, 0.0), // type2 floor
    Material(vec3(.507554), vec3(.508273), 0.4 * 128, 0.0, 0.0), // Cube
    Material(vec3(0.0, .5, 0.0), vec3(0.0, .4, 0.0), 0.7 * 128.0, 0.8, 0.7), // Glass
    Material(vec3(.8, .8, .3), vec3(0.2, .4, 0.3), 0.8 * 128.0, 0.5, 0.8),// Mug
    Material(vec3(.1, .1, .8), vec3(0.3, .3, 0.7), 0.4 * 128.0, 0.0, 0.0), // Julia
    Material(vec3(.6, .1, .6), vec3(0.6, .3, 0.7), 0.1 * 128.0, 0.0, 0.0) // Torus
);

float phongSpecular(in vec3 view, in vec3 normal, float shininness)
{
    vec3 R = normalize(reflect(view, normal));
    return pow( max(dot(R, normal), 0.0), shininness );
}

vec3 diffuseAtPoint(int object, in vec3 point, in vec3 normal)
{
    if (object == 0) {
        return texture2D(floorTex, point.xz / 10).rgb;
    } if (object == 1) {
        return texture2D(floorTex, point.xy / 10).rgb;
    } if (object >= 2) {
        vec3 light = normalize(gl_LightSource[0].position.xyz - point);
        return material[object].diffuse * max(dot(light, normal), 0.2);
    } else if (object == skyObject) {
        return environment;
    } else {
        // Useful for debugging
        return vec3(0.0, 0.0, 0.0);
    }
}

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

vec3 staticLighting(in vec3 origin, in vec3 view, int object)
{
    if (object < 0)
        return environment;

    vec3 normal = normalAt(origin);

    vec3 color = diffuseAtPoint(object, origin, normal);
    if (material[object].refraction != 0.0) {
        vec3 cReflected, cRefracted;

        vec3 reflDir = normalize(reflect(view, normal));
        vec3 reflPos = origin + reflDir * (tracePrecision * 4);
        int reflObj = trace(reflPos, reflDir);

        vec3 reflNormal = normalAt(reflPos);
        cReflected = diffuseAtPoint(reflObj, reflPos, reflNormal);
        //cReflected = material[reflObj].diffuse * shadowAt(reflPos, reflNormal, normalize(gl_LightSource[0].position.xyz - reflPos));

        // Increase iteration limit
        if (iteration > maxIterations / 2)
            iteration = maxIterations / 2;

        // Now trace refraction inside object
        vec3 refrDir = normalize(refract(view, normal, material[object].indexOfRefraction));
        vec3 refrPos = origin + refrDir * (tracePrecision * 2);
        int refrObj = object;

        int hit = 0;
        while (++iteration < maxIterations && refrObj == object) {
            float distance = abs(distanceAt(refrPos, refrObj));
            refrPos += refrDir * distance;

            if (distance < tracePrecision) {
                vec3 N = normalAt(refrPos);
                float index = material[object].indexOfRefraction;
                if (dot(N, refrDir) > 0) {
                    N = -N;
                    index = 1.0 / index;
                }
                if (length(N) > 0.1) {
                    vec3 newDir = normalize(refract(refrDir, N, index));
                    if (length(newDir) > 0.1) {
                        refrDir = newDir;
                        hit++;
                    }
                }

                refrPos += refrDir * (tracePrecision * 2);
            }
        }

        // Increase iteration limit
        iteration = maxIterations / 2;
        // Finally, trace it to another object
        refrObj = trace(refrPos, refrDir);
        vec3 refrNormal = normalAt(refrPos);
        cRefracted = diffuseAtPoint(refrObj, refrPos, refrNormal);
        //cRefracted = material[refrObj].diffuse * shadowAt(refrPos, refrNormal, normalize(gl_LightSource[0].position.xyz - refrPos));

        float fFresnel = fresnel(dot(-view, normal), material[object].indexOfRefraction);
        vec3 rrColor = mix(cRefracted, cReflected, fFresnel);
        color = mix(material[object].diffuse, rrColor, material[object].refraction);
    }

    float specularTerm = phongSpecular(view, normal, material[object].shininness);
    color += material[object].specular * specularTerm;

    vec3 light = normalize(gl_LightSource[0].position.xyz - origin);
    float shadowTerm = shadowAt(origin, normal, light);
    color *= shadowTerm;

    //return vec3(shadowTerm);
    return color;
}

void main(void)
{
    vec3 eyePos = mat3(matrix) * vec3(0, 5, 12);
    vec3 origin = eyePos; // eye
    vec3 dir = normalize(mat3(matrix) * vec3(aspectRatio * pixel.x, pixel.y, -1));

    int object = trace(origin, dir);
    gl_FragColor = vec4(staticLighting(origin, dir, object), 1.0);

    // apply fog
    float distance = length(origin - eyePos);
    const float LOG2 = 1.442695;
    float fogFactor = exp2( -gl_Fog.density * gl_Fog.density * distance * distance * LOG2 );
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    gl_FragColor = mix(gl_Fog.color, gl_FragColor, fogFactor);
}
