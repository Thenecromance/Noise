
//for showing need to be optimized
float interpolation(vec2 p, float vertex_value[4])
{
    return mix(
            mix(vertex_value[0],vertex_value[1],p.x),
            mix(vertex_value[2],vertex_value[3],p.x),
    p.y);
}



float grad(vec2 p)
{
    p = 50.0*fract(p*0.3183099+vec2(0.71,0.113));
    return -1.0+2.0*fract(p.x*p.y*(p.x+p.y));//normalized the grad to [-1,1]
}
vec2 fade(vec2 t)
{
    
    // f(t) = 6x^5-15x^4+10x^3
    return 
    //t*t*(3.0-2.0*t)
    t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f)
    ;
    //return t ;
}

float noise(vec2 p)
{
    vec2 i= floor(p);
    vec2 f = fract(p);
    
    float vertex_value[4];
    vertex_value[0] =    grad(i);
    vertex_value[1] =     grad(i+vec2(1,0));
    vertex_value[2] =     grad(i+vec2(0,1));
    vertex_value[3] =     grad(i+vec2(1,1));
    
    return interpolation(fade(f),vertex_value);
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
	vec2 uv = p*vec2(iResolution.x/iResolution.y,1.0) +cos( iTime)*0.2;

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