


//out vec4 FragColor;
//uniform vec2 iResolution;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel2;
uniform vec2 iResolution;
uniform vec4 iMouse;
uniform float iTime;
uniform bool inflow;



void main()
{
R = iResolution.xy; time = iTime; Mouse = iMouse;
    //Mouse.x=(1.0+iMouse.x)*0.5*iResolution.x;
    //Mouse.y=(1.0-iMouse.y)*0.5*iResolution.y;
    vec2 pos = gl_FragCoord.xy;
    ivec2 p = ivec2(pos);
        
    vec4 data = texel(ch0, pos);
    
    particle P = getParticle(data, pos);
    //P.X=pos;
    
    if(P.M.x != 0.) //not vacuum
    {
        Simulation(ch0, P, pos);
    }
    if(inflow)
{
    if(length(P.X - R*vec2(0.525, 0.8)) < 20.)
    {
        P.X = pos;
        P.V = 1.0*Dir(-PI*0.4 + 0.1*sin(1.3*time));
        P.M = mix(P.M, vec2(fluid_rho, 1.), 0.4);
    }

    if(length(P.X - R*vec2(0.425, 0.8)) < 20.)
    {
        P.X = pos;
        P.V = 1.0*Dir(-PI*0.6 + 0.1*sin(1.3*time));
        P.M = mix(P.M, vec2(fluid_rho, 0.), 0.4);
    }
}
    
    gl_FragColor = saveParticle(P, pos);
}
