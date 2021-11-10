//Optimization shader remove lots of unecessary vars and method entry 

vec2 hash22(vec2 pos)
{
    vec2 p=vec2(
    dot(pos,vec2(127.1f, 311.7f)),
    dot(pos,vec2(269.5f, 183.3f))
    );
    return fract(sin(p+ 20.0f) * 256132.) * 2.0f - 1.0f;
}
float noise(vec2 pos)
{
    vec2 i = floor(pos);
    vec2 f = fract(pos );
    vec2 u  = f*f*f*(f*(f*6.0f-15.0f)+10.f);
    return mix(
            mix(
                dot(hash22(i+vec2(0,0)),f-vec2(0.0,0.0)),
                dot(hash22(i+vec2(1,0)),f-vec2(1.0,0.0)),
                 u.x),
            mix(     
                dot(hash22(i+vec2(0,1)),f-vec2(0.0,1.0)),
                dot(hash22(i+vec2(1,1)),f-vec2(1.0,1.0)), 
                u.x),
           u.y);
}

float fbm(vec2 pos )
{
    float f = 0.0f;
    pos*=4.0;
    f += 1.000000 * noise(pos);
    pos*=2.0;
    f += 0.500000 * noise(pos);
    pos*=2.0;
  	f += 0.250000 * noise(pos);
    pos*=2.0;
    f += 0.1250000 * noise(pos);
    pos*=2.0;
    f += 0.062500 * noise(pos);
    pos*=2.0;
    f += 0.312500 * noise(pos);
    return f ;
}
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 p = fragCoord / iResolution.xy;
	vec2 uv = p*vec2(iResolution.x/iResolution.y,1.0) + iTime*0.2;

    float n;
     //left noise using fbm 
    if( p.x<0.5)
    {
      n = fbm(uv*8.0);
    }
    //right side simply using noise 
    else
    {
      n = noise(uv*32.0);
    }
    n =0.5f*n+0.5f;
    n *= smoothstep(0.0f,0.005,abs(p.x-0.5)); 
    // Output to screen
    fragColor = vec4(n,n,n,1.0);
}