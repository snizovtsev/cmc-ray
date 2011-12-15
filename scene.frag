#version 120

#define LIGHT %1

//
// Uniforms
//

varying vec2    pixel;
uniform float   aspectRatio;
uniform mat4    matrix;

//
// Constants
//

const float     tracePrecision  = 0.005;
const float     normalPrecision = 0.001;
const float     skyDistance     = 500.0;
const int       skyObject       = -1;
const int       noneObject      = -2;

//
// Imports
//

float distanceAt(vec3 p, out int object);
vec3 colorAt(int object, vec3 point, vec3 normal, vec3 view, vec3 light);
vec3 applyFog(vec3 color, vec3 distance);

//
// Main
//

vec3 normalAt(vec3 p)
{
    int object;
    vec3 eps = vec3(normalPrecision, 0, 0);

    vec3 n;
    n.x = distanceAt(p + eps.xyz, object) - distanceAt(p - eps.xyz, object);
    n.y = distanceAt(p + eps.yxz, object) - distanceAt(p - eps.yxz, object);
    n.z = distanceAt(p + eps.yzx, object) - distanceAt(p - eps.yzx, object);

    return normalize(n);
}

int trace(inout vec3 origin, in vec3 dir, int maxIterations)
{
    float totalDistance = 0.0;
    int iteration = 0;

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

void main(void)
{
    vec3 eyePos = mat3(matrix) * vec3(0, 5, 12);
    vec3 origin = eyePos;
    vec3 dir = normalize(mat3(matrix) * vec3(aspectRatio * pixel.x, pixel.y, -1));

    int object = trace(origin, dir, 150);
    gl_FragColor = vec4(colorAt(object, origin, normalAt(origin), -dir, LIGHT - origin), 1.0);
    gl_FragColor = applyFog(gl_FragColor, length(eyePos - origin));
}
