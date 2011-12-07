#version 120

float sdSphere( vec3 p, float s )
{
    return length(p)-s;
}

float udBox( vec3 p, vec3 b )
{
    return length(max(abs(p)-b, 0.0));
}

float udRoundBox( vec3 p, vec3 b, float r )
{
    return length(max(abs(p)-b, 0.0))-r;
}

float sdTorus( vec3 p, vec2 t )
{
    vec2 q = vec2(length(p.xz)-t.x,p.y);
    return length(q)-t.y;
}

float sdCylinder( vec3 p, vec3 c )
{
    return length(p.xz-c.xy)-c.z;
}

float sdCone( vec3 p, vec2 c )
{
    // c must be normalized
    float q = length(p.xy);
    return dot(c,vec2(q,p.z));
}

float sdPlane( vec3 p, vec4 n )
{
    // n must be normalized
    return dot(p,n.xyz) + n.w;
}

float opS( float d1, float d2 )
{
    return max(d1,-d2);
}

float mug( vec3 p )
{
    return min(
                opS(
                    min(
                        max(sdCylinder(p, vec3(0, 0, 1.5)), abs(p.y) - clamp(p.y, 0.0, 3.0)),
                        sdTorus(p.yzx + vec3(-1.5, 0, -1), vec2(1.0, 0.2))
                        ),
                    sdCylinder(p, vec3(0, 0, 1.3))
                    ),
                max(sdCylinder(p, vec3(0, 0, 1.3)), abs(p.y) - clamp(p.y, 0.0, .2))
                );
}

//
// Fractals
//

vec4 quatSq( vec4 q )
{
   vec4 r;
   r.x   = q.x*q.x - dot( q.yzw, q.yzw );
   r.yzw = 2*q.x*q.yzw;
   return r;
}

vec4 quatMult( vec4 q1, vec4 q2 )
{
   vec4 r;
   r.x   = q1.x*q2.x - dot( q1.yzw, q2.yzw );
   r.yzw = q1.x*q2.yzw + q2.x*q1.yzw + cross( q1.yzw, q2.yzw );
   return r;
}

vec4 c = vec4(-.4, .2, .3, -.2);

float julia( vec3 p )
{
    const float ESCAPE_THRESHOLD = 1e1;
    vec4 q = vec4(p, 0);
    vec4 qp = vec4(1, 0, 0, 0);

    for (int i = 0; i < 8; ++i) {
        qp = 2.0 * quatMult(q, qp);
        q = quatSq(q) + c;

        if( dot( q, q ) > ESCAPE_THRESHOLD )
           break;
    }

    float normQ = length(q);
    return 0.5 * normQ * log( normQ ) / length( qp );
}

float mandelBulb(vec3 w)
{
    const float ESCAPE_THRESHOLD = 1;
    vec3 dw = vec3(1, 0, 0);

    for (int i = 0; i < 20; ++i) {
        dw = 2.0 * cross(w, dw);

        float x = w.x; float x2 = x*x; float x4 = x2*x2;
        float y = w.y; float y2 = y*y; float y4 = y2*y2;
        float z = w.z; float z2 = z*z; float z4 = z2*z2;

        float k3 = x2 + z2;
        float k2 = inversesqrt( k3*k3*k3*k3*k3*k3*k3 );
        float k1 = x4 + y4 + z4 - 6.0*y2*z2 - 6.0*x2*y2 + 2.0*z2*x2;
        float k4 = x2 - y2 + z2;

        w.x =  64.0*x*y*z*(x2-z2)*k4*(x4-6.0*x2*z2+z4)*k1*k2;
        w.y = -16.0*y2*k3*k4*k4 + k1*k1;
        w.z = -8.0*y*k4*(x4*x4 - 28.0*x4*x2*z2 + 70.0*x4*z4 - 28.0*x2*z2*z4 + z4*z4)*k1*k2;

        w += vec3(2, 0, 0);

        if( dot(w, w) > ESCAPE_THRESHOLD )
            break;
    }

    float normW = length(w);
    return 0.5 * normW * log( normW ) / length( dw );
}

float mandelBox(vec3 pos)
{
    const int Iterations = 8;
    const float Scale = 2.04344;
    const float absScalem1 = abs(Scale - 1.0);
    const float AbsScaleRaisedTo1mIters = pow(abs(Scale), float(1-Iterations));

    vec4 p = vec4(pos,1), p0 = p;  // p.w is the distance estimate

    for (int i=0; i<Iterations; i++) {
        p.xyz = clamp(p.xyz, -1.0, 1.0) * 2.0 - p.xyz;  // min;max;mad
        float r2 = dot(p.xyz, p.xyz);
        p *= clamp(max(0.25/r2, 0.25), 0.0, 1.0);  // dp3,div,max.sat,mul
        p = p*Scale + p0;
        if ( r2>1000.0) break;
    }
    return ((length(p.xyz) - absScalem1) / p.w - AbsScaleRaisedTo1mIters);
}

//
// Main
//

float distanceAt(vec3 p, out int object)
{
    float tmpDist = .0;

    /*vec2 fm = floor(mod(p.xz, vec2(2.0)));
    if ( abs(fm.x - fm.y)  < .1)
        object = 0;
    else
        object = 1;
    float dist = min(p.y + 2, p.z + 10);*/
    object = 0;
    float dist = p.y + 2;

    tmpDist = p.z + 15;
    if (tmpDist < dist) {
       object = 1;
       dist = tmpDist;
    }

    tmpDist = udBox(p, vec3(2, 2, 2));
    if (tmpDist < dist) {
        dist = tmpDist;
        object = 2;
    }

    tmpDist = julia(p/2+vec3(0, -2, 0))*2;
    if (tmpDist < dist) {
        dist = tmpDist;
        object = 5;
    }

    tmpDist = sdSphere(p+vec3(-3, -2, -3), 1.0);
    if (tmpDist < dist) {
        dist = tmpDist;
        object = 3;
    }
    tmpDist = sdSphere(p+vec3(3, -2, -3), 1.0);
    if (tmpDist < dist) {
        dist = tmpDist;
        object = 3;
    }

    tmpDist = mug(p + vec3(0, 1, -6));
    if (tmpDist < dist) {
        dist = tmpDist;
        object = 4;
    }

    p += vec3(-4, 0, 0);
    /*float c = cos(1.5*p.y);
    float s = sin(1.5*p.y);
    mat2  m = mat2(c,-s,s,c);
    vec3  q = vec3(m*p.xz,p.y);*/

    tmpDist = sdTorus(p, vec2(1, 0.5));
    if (tmpDist < dist) {
        dist = tmpDist;
        object = 6;
    }

    //tmpDist = mandelBulb(p/4.0)*4.0;
    //tmpDist = mandelBox(p);
    /*if (tmpDist < dist) {
        dist = tmpDist;
        object = 3;
    }*/

    return dist;
}

vec3 normalAt(vec3 p)
{
    int object;
    vec3 eps = vec3(0.001, 0, 0);

    vec3 n;
    n.x = distanceAt(p + eps.xyz, object) - distanceAt(p - eps.xyz, object);
    n.y = distanceAt(p + eps.yxz, object) - distanceAt(p - eps.yxz, object);
    n.z = distanceAt(p + eps.yzx, object) - distanceAt(p - eps.yzx, object);

    return normalize(n);
}

float shadowAt(vec3 p, vec3 n, vec3 light)
{
    const float EPSILON = 0.01;
    const int probes = 5;

    float ambientOcclusion = 0.0;
    float softShadow = 1.0;
    float weigth = .5;
    float t = probes * probes * EPSILON * 12.0;
    int object;

    for (int i = 0; i < probes; ++i) {
        float delta = (i + 1) * (i + 1) * EPSILON * 12.0;
        ambientOcclusion += weigth * (delta - distanceAt(p + n * delta, object));
        weigth *= 0.5;

        float h = max(distanceAt(p + light * t, object), 0.2);
        softShadow = min(softShadow, 8 * h / t);
        t += h;
    }

    ambientOcclusion = clamp(ambientOcclusion, 0.0, 1.0);

    return min(softShadow, 1.0 - ambientOcclusion);
}
