uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel2;
uniform float iTime;
uniform vec2 iResolution;
uniform vec4 iMouse;
#define EPS  .01
#define COL0 vec3(.2, .35, .55)
#define COL1 vec3(.9, .43, .34)
#define COL2 vec3(.96, .66, .13)
#define COL3 vec3(0.0)
#define COL4 vec3(0.99,0.1,0.09)

float df_circ(in vec2 p, in vec2 c, in float r)
{
    return abs(r - length(p - c));
}

vec2 intersect (vec2 p, vec2 a, vec2 b)
{
    vec2 ba = normalize(b - a);
    return a + ba * dot(ba, p - a);
}

// Visual line for debugging purposes.
bool line (vec2 p, vec2 a, vec2 b, float w)
{
    vec2 ab = normalize(b - a);
    vec2 ap = p - a;
    return length((a + ab * dot(ab, ap)) - p) < w;
}

float df_line(in vec2 p, in vec2 a, in vec2 b)
{
    vec2 pa = p - a, ba = b - a;
    float h = clamp(dot(pa,ba) / dot(ba,ba), 0., 1.);
    return length(pa - ba * h);
}

float sharpen(in float d, in float w)
{
    float e = 1. / min(iResolution.y , iResolution.x);
    return 1. - smoothstep(-e, e, d - w);
}

float df_bounds(in vec2 uv, in vec2 p, in vec2 a, in vec2 b, in vec2 c, in vec3 barycoords)
{
    float cp = 0.;
    return cp;
}

/*vec3 scene(in vec2 uv, in vec2 a, in vec2 b, in vec2 c, in vec2 p)
{
    float d = df_bounds(uv, p, a, b, c);
    return d > 0. ? COL3 : COL1;
}*/

vec3 bary(in vec2 a, in vec2 b, in vec2 c, in vec2 p)
{
    // barycoords = (a1,a2,a3)
    // p = a1*a + a2*b + a3*c
    // p = a1*(x1,y1) + a2*(x2,y2) + a3*(x3,y3)
    // p.x = a1*x1 + a2*x2 + a3*x3
    // p.y = a1*y1 + a2*y2 + a3*y3
    // a1 + a2 + a3 = 1

    // vertices do triangulo
    float x1 = a.x, y1 = a.y; // ver A
    float x2 = b.x, y2 = b.y; // ver B
    float x3 = c.x, y3 = c.y; // ver C

    float ABC = abs(x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2))/2;

    float a1 =-((y3-y2)*p.x + x2*(p.y-y3) + x3*(y2-p.y)) / ABC;
    float a2 = ((y3-y1)*p.x + x1*(p.y-y3) + x3*(y1-p.y)) / ABC;
    float a3 = ((y1-y2)*p.x + x1*(y2-p.y) + x2*(p.y-y1)) / ABC;

    return vec3(a1,a2,a3);
}

bool test(in vec2 a, in vec2 b, in vec2 c, in vec2 p, inout vec3 barycoords)
{
    barycoords = bary(a,b,c,p);
    return (barycoords.x >= 0.0 && barycoords.y >= 0.0 && barycoords.z >= 0.0);
}

vec3 globalColor (in vec2 uv, in vec2 a, in vec2 b, in vec2 c)
{
    vec3 r=vec3(1.0);
    return r;
}

void main()
{
    vec3 color=vec3(0.0);

    float ar = iResolution.x / iResolution.y;
    vec2 uv = (gl_FragCoord.xy / iResolution.xy * 2. - 1.) * vec2(ar, 1.);
    vec2 mc;
    // if(iMouse.z == 1.0)
    mc = (iMouse.xy / iResolution.xy * 2. - 1.) * vec2(ar, 1.);

    vec2 p = mc;
    if(df_circ(uv,p,EPS) < 0.5*EPS)
        color = vec3(1.0, 1.0, 1.0);

    vec2 a = vec2( .73,  .75);
    vec2 b = vec2(-.85,  .15);
    vec2 c = vec2( .25, -.75);
    vec3 barycoords;
    
    bool t0 = test(a, b, c, p, barycoords);

    if (line(uv, a, b, 0.0025)) // aresta AB
        color = vec3(1.0, 1.0, 0.0);
    if (line(uv, b, c, 0.0025)) // aresta BC
        color = vec3(1.0, 0.0, 1.0);
    if (line(uv, c, a, 0.0025)) // aresta CA
        color = vec3(0.0, 1.0, 1.0);

    if (df_circ(uv, a,EPS)<0.5*EPS)
        color = vec3(0.0, 1.0, 0.0);
    if (df_circ(uv, b,EPS)<0.5*EPS)
        color = vec3(1.0, 0.0, 0.0);
    if (df_circ(uv, c,EPS)<0.5*EPS)
        color = vec3(0.0, 0.0, 1.0);

    float i = 0;

    if(barycoords.x < 0 && barycoords.y > 0 && barycoords.z > 0 ){  // coord x negativa, passou pela aresta CA
        i = abs(barycoords.x)*0.03;
        if(line(uv,b-(i,i)*0.5,c-(i,i)*0.5,i))
            color = vec3(1.0, 0.0, 0.0);
    }
    if(barycoords.x > 0 && barycoords.y < 0 && barycoords.z > 0 ){  // coord y negativa, passou pela aresta BC
        i = abs(barycoords.y)*0.03;
        if(line(uv,c+(i,i),a+(i,i),i))
            color = vec3(0.0, 1.0, 0.0);
    }
    if(barycoords.x > 0 && barycoords.y > 0 && barycoords.z < 0 ){  // coord z negativa, passou pela aresta AB
        i = abs(barycoords.z)*0.03;
        if(line(uv,a+(i,i),b+(i,i),i))
            color = vec3(1.0, 1.0, 1.0);
    }

    vec3 col = t0 ? vec3(1.0,1.0,1.0)-color : COL2-color;
    gl_FragColor = vec4(col, 1);
}
