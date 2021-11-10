
//simply perlin noise 
//for showing need to be optimized
vec2 fade(vec2 t)
{
    
   
    return 
    //f(t)= 3x^2-2x^3
    //t*t*(3.0-2.0*t)
    // f(t) = 6x^5-15x^4+10x^3
    t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f)
    ;
    //return t ;
}
vec2 hash22(ivec2 pos)
{
    vec2 p=vec2(
    dot(vec2(pos),vec2(127.1f, 311.7f)),
    dot(vec2(pos),vec2(269.5f, 183.3f))
    );
    return fract(sin(p+ 20.0f) * 256132.) * 2.0f - 1.0f;
}

float inerpolation(vec2 pos , float vertex_random[4])
{
    vec2 u = fade(pos);
    return mix(
            mix(vertex_random[0],vertex_random[1], u.x),
            mix(vertex_random[2],vertex_random[3], u.x),
           u.y);
}

float grad(ivec2 pos, vec2 des)
{
    return dot(hash22(pos),des);
}

float noise(vec2 pos)
{
    ivec2 i =ivec2( floor(pos));
    vec2 f = fract(pos );
    ivec2 vertex[4];

    vertex[0]= i+ivec2(0,0);
    vertex[1]= i+ivec2(1,0);
    vertex[2]= i+ivec2(0,1);
    vertex[3]= i+ivec2(1,1);


    vec2 vertex_dis[4];
    vertex_dis[0]= f-vec2(0,0);
    vertex_dis[1]= f-vec2(1,0);
    vertex_dis[2]= f-vec2(0,1);
    vertex_dis[3]= f-vec2(1,1);

    float vertex_random[4];
    for(int i = 0;i<4 ;i++)
    {
        vertex_random[i]=grad(vertex[i],vertex_dis[i]);
    }
    
    return inerpolation(f,vertex_random);
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