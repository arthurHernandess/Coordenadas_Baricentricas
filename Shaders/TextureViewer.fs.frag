out vec4 FragColor;
in  vec2 TexCoords;


uniform sampler2D fboAttachment;
uniform vec2 iResolution;
uniform float iTime;
void main()
{
    vec2 pos = gl_FragCoord.xy/iResolution;
    FragColor = texture(fboAttachment, pos);
    /*vec4 data = texelFetch(fboAttachment, ivec2(gl_FragCoord), 0);
        uint u = floatBitsToUint(data[0]);
        uint v = floatBitsToUint(data[1]);
            FragColor = vec4(sin(vec2(u, v)/64.0 + vec2(0.5, -0.7)*iTime), data[2] ,1.0);*/
}
