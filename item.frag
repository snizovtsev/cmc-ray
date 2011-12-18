float distanceAt(vec3 p, out int object);

#define EPSILON 0.01
#define PROBES 5

float shadowAt(vec3 p, vec3 n, vec3 light)
{
    float ambientOcclusion = 0.0;
    float softShadow = 1.0;
    float weigth = .5;

    float t = EPSILON * 12.0 * float(PROBES * PROBES);
    int object;

    for (int i = 0; i < PROBES; ++i) {
        float delta = float(i + 1) * float(i + 1) * EPSILON * 12.0;
        ambientOcclusion += weigth * (delta - distanceAt(p + n * delta, object));
        weigth *= 0.5;

        float h = max(distanceAt(p + light * t, object), 0.2);
        softShadow = min(softShadow, 8.0 * h / t);
        t += h;
    }

    ambientOcclusion = clamp(ambientOcclusion, 0.0, 1.0);
    return min(softShadow, 1.0 - ambientOcclusion);
}
