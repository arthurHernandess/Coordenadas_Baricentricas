


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
//Mouse.x=(1.0+iMouse.x)*0.5*iResolution.x;
//Mouse.y=(1.0-iMouse.y)*0.5*iResolution.y;
vec2 pos = gl_FragCoord.xy;
    //ivec2 p = ivec2(pos);

    vec4 data = texel(ch0, pos);
    particle P = getParticle(data, pos);

    //particle render
    vec4 rho = vec4(0.);
    range(i, -1, 1) range(j, -1, 1)
    {
        vec2 ij = vec2(i,j);
        vec4 data = texel(ch0, pos + ij);
        particle P0 = getParticle(data, pos + ij);

        vec2 x0 = P0.X; //update position
        //how much mass falls into this pixel
        rho += 1.*vec4(P.V, P.M)*G((pos - x0)/0.75);
    }

    gl_FragColor = rho;

}
