
float grad(vec2 p)
{
    p = 50.0*fract(p*0.3183099+vec2(0.71,0.113));
    return -1.0+2.0*fract(p.x*p.y*(p.x+p.y));//normalized the grad to [-1,1]
}


float noise(vec2 p )
{
    vec2 i= floor(p);
    vec2 f = fract(p);
    vec2 u =  f * f * f * (f * (f * 6.0f - 15.0f) + 10.0f);

     return mix(
            mix( grad(i), grad(i+vec2(1,0)),u.x),
            mix(grad(i+vec2(0,1)), grad(i+vec2(1,1)),u.x),
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