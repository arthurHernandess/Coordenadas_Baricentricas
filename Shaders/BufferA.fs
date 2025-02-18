


in vec3 aColor;
in vec4 aPosition;
out vec4 FragColor;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel2;
uniform vec2 iResolution;
uniform vec4 iMouse;
uniform float iTime;
uniform int iFrame;



void main()
{

R = iResolution.xy; time = iTime; Mouse = iMouse;

vec2 pos = gl_FragCoord.xy;
    ivec2 p = ivec2(pos);
    // vec4 data = texel(ch0, pos);

    
    particle P;
    P.X=vec2(0.0);
    P.V=vec2(0.0);
    P.M = vec2(0.0);


    //initial condition
    if(iFrame < 1)
    {
        //random
        vec3 rand = hash32(pos);
        if(rand.z < 0.0)
        {
            P.X = pos;
            P.V = 0.5*(rand.xy-0.5) + vec2(0., -0.5);
            P.M = mix(P.M, vec2(fluid_rho, 1.0-rand.z), 0.4);
        }
        else
        {
            P.X = pos;
            P.V = vec2(0.0);
            P.M = vec2(0.0);
        }
    }
    else
         Reintegration(ch0, P, pos);
    gl_FragColor = saveParticle(P, pos);

}
