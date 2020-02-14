#define AA 

#define ZERO min(iFrame,0)


float smoothMax( in float a, in float b, in float k )
{
    float h = max( k-abs(a-b),0.0);
    return max(a,b) + (0.25/k)*h*h;
}

vec2 rot45( in vec2 pos )
{
    return vec2( pos.x-pos.y, pos.x+pos.y) * 0.707107;
}

// SDF objects (Thanks to Inigo Quilez tutorials)

float sdBox( in vec3 pos, in vec3 sides )
{
    return length( max(abs(pos)-sides, 0.0 ) );
}

float sdBox2DExtruted( in vec2 pos, in vec2 sides )
{
    return length( max(abs(pos)-sides, 0.0 ) );
}

float sdSphere( in vec3 pos, in float r )
{
    return length( pos )-r;
}

float sdCross( in vec3 pos, in vec3 r )
{
    pos =  abs(pos);
    
    // Conditional move optimization?
    pos.xz = (pos.z>pos.x)? pos.zx : pos.xz;
    
    return sdBox( pos, r );
}

float sdStickV( in vec3 pos, in float l )
{
    pos.y = max(pos.y-l, 0.0);
    return length( pos );
}


vec4 sdGear( in vec3 pos, in float time, in float offset )
{
#define SECTOR_ANGLE (2.0*3.14159/12.0)
    pos.y = abs(pos.y);
    
    float angle = time + offset*SECTOR_ANGLE;
    pos.xz = mat2( cos(angle), -sin(angle),
                   sin(angle), cos(angle)) * pos.xz;
    
    // Gear teeth extrusion
    //---------------------
    
    
    // Only one box, with multiple instances through SDF duplication.
    // We compute in which sector, on XZ plane, is pos (Y is vertical, so rotation is in XZ plane)
    float sector = round( atan(pos.z,pos.x)/SECTOR_ANGLE );
    vec3 rotated_pos = pos;
    angle = sector*SECTOR_ANGLE;
    rotated_pos.xz = mat2( cos(angle), -sin(angle), sin(angle), cos(angle))*rotated_pos.xz;
    
    float dist = sdBox2DExtruted( rotated_pos.xz-vec2(0.17,0.0), vec2( 0.04, 0.018 ) ) - 0.01;
	                   
    // Gear Ring extrusion
    //--------------------
    
    // Infinite ring
    float dObject = abs(length(pos.xz)-0.155)-0.018; 
    dist = min( dist, dObject );
    
    
    // Smooth Clipping plane for top and bottom
    //-----------------------------------------
    // (disabled to be replaced by sphere clipping below)
    //dist = smoothMax( dist, abs(pos.y)-0.03, 0.005 ); 
    
    // Cross shape
    //------------
    dObject = sdCross( pos-vec3(0.0,0.48,0.0), vec3(0.2,0.01,0.01) );
    dist = min( dist, dObject );
    
    
    // Smooth Clipping sphere 
    //-----------------------
    
    float r = length( pos );
    dist = smoothMax( dist, abs(r-0.5)-0.03, 0.003 ); 
    
    // Vertical stick
    dObject = sdStickV( pos, 0.5)-0.01;
    dist = min( dist, dObject );
    
    return vec4( dist, pos );;
}


// Definition of the scene through SDF. Returns the distance to the nearest surface
vec4 map( in vec3 pos, in float time )
{
    // Central sphere
    vec4 res = vec4( sdSphere(pos, 0.12), pos );
    vec4 d;
    
    // Mirroring gears through easy rotation by swapping axis
    // as everything is nicely aligned to system coord axis

    vec3 pa = abs(pos);
    
    pa = ( pa.x>pa.y && pa.x>pa.z) ? pa = pos.yxz*vec3(pos.x>0.0?-1:1,1,1) :
                     ( pa.z>pa.y ) ? pa = pos.xzy*vec3(pos.z>0.0?-1:1,1,1) :
                                     pa = pos.xyz*vec3(pos.y<0.0?-1:1,1,1);
        
    
    // Gear tilted in YZ plane,  XZ plane and  XY plane
    vec3 px = vec3( rot45(pos.zy), pos.x*((pos.z>0.0)?1.0:-1.0) );     if( abs(px.x)>abs(px.y)) px = px.zxy;
    vec3 py = vec3( rot45(pos.xz), pos.y*((pos.x>0.0)?1.0:-1.0) );     if( abs(py.x)>abs(py.y)) py = py.zxy;
    vec3 pz = vec3( rot45(pos.yx), pos.z*((pos.y>0.0)?1.0:-1.0) );     if( abs(pz.x)>abs(pz.y)) pz = pz.zxy;
    
    // Renderings
    d = sdGear( pa, 3.0*time,0.0 ); if(d.x<res.x ) res = d;
    d = sdGear( px, 3.0*time,0.5 ); if(d.x<res.x ) res = d;
    d = sdGear( py, 3.0*time,0.5 ); if(d.x<res.x ) res = d;
    d = sdGear( pz, 3.0*time,0.5 ); if(d.x<res.x ) res = d;

    
    return res;
}


// Approximation de normale par dérivation
// Le gradient est équivalent à la normale de la surface
vec3 calcNormal(in vec3 pos, in float time )
{
    vec2 e = vec2 (0.001,0.0);
    
    return normalize(
        vec3( map( pos+e.xyy, time ).x - map( pos-e.xyy, time ).x,
              map( pos+e.yxy, time ).x - map( pos-e.yxy, time ).x,
              map( pos+e.yyx, time ).x - map( pos-e.yyx, time ).x ) );
}

// Thanks to Inigo Quilez
float calcAmbientOcclusion( in vec3 pos, in vec3 nor, in float time )
{
	float occ = 0.0;
    float sca = 1.0;
    for( int i=ZERO; i<5; i++ )
    {
        float h = 0.01 + 0.12*float(i)/4.0;
        float d = map( pos+h*nor, time ).x;
        occ += (h-d)*sca;
        sca *= 0.95;
    }
    return clamp( 1.0 - 3.0*occ, 0.0, 1.0 );
}

vec4 rayMarching( in vec3 ray_origin, vec3 ray_direct, in float time )
{
    vec4 impact = vec4(0.0);
    float t = 0.0;
    for( int i=0; i<250; i++)
    {
        // pos = position du point d'échantillonage le long du rayon
        vec3 pos = ray_origin + t*ray_direct;
        
        // test d'impact de l'objet
        impact = map( pos, time );
        
        // on est à l'intérieur
        if( impact.x<0.0001 )
            break;
        
        t += impact.x;
        
        // Si on est trop loin, on n'a rien trouvé d'intéressant 
        // Equivalent du Far Clipping Plane
        if( t>20.0)
            break;
    }
    
    return vec4(t,impact.yzw);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    float time = iTime;
    float animation = -10.0*(iMouse.x/iResolution.x-0.5);
    
    // Recentrage des coordonnées, entre -1 et 1
    vec2 p = (2.0*fragCoord-iResolution.xy)/iResolution.y;

    // Camera reculée de 1 en Z, et on regarde vers l'origine
    // X up, Z right, Z toward us
	vec3 ray_origin = vec3(1.0*sin(animation),0.25,1.0*cos(animation)); // c'est la positon de la caméra
    //vec3 ray_origin = vec3( 0,0.25,1);
    vec3 ray_direct = normalize( vec3( p, -1.) );
    
    // Camera
    vec3 target = vec3 (0.0);
    vec3 ww = normalize(target-ray_origin);
    vec3 uu = normalize( cross(ww, vec3(0,1,0)));
    vec3 vv = normalize( cross(uu,ww) );
    
    // direction du rayon dans le monde, depuis le repère de la caméra
    ray_direct = normalize( p.x*uu + p.y*vv + 1.5*ww );
        
    // La couleur par défaut est celle du ciel, qui devient plus claire vers le bas
    //vec3 col = vec3(0.4, 0.75, 1.0) - 0.7 * ray_direct.y;
    // Effet atmosphérique vers l'horizon
    //col = mix( col, vec3(0.7, 0.75, 0.8), exp(-10.0*ray_direct.y)); 

    vec3 col = vec3(0.0);
    
    //for( int m=0; m<AA; m++ )
    {
    // ray marching
    vec4 impact = rayMarching( ray_origin, ray_direct, time );
    
    // On a trouvé quelque chose
    if( impact.x<=20.0 )
    {
        vec3 position = ray_origin + impact.x*ray_direct;
        vec3 normal = calcNormal( position, time );
        
        float occlusion = calcAmbientOcclusion( position + normal*0.001, normal, time);
        
        // Box mapping
        vec3 material = 0.5 * texture( iChannel0, impact.yz).xyz
                      + 0.5 * texture( iChannel0, impact.yw).xyz;
        
        material = material*material;
        
        vec3 light = vec3(3.0)*(0.5 + 0.5* normal.y)*occlusion;
        
        col = material * light;
        
        // Color = f( normale )
        //col = 0.5 + 0.5* normal;
        
        // calcul d'éclairage diffus par lumière directionnelle (soleil)
        // Produit scalaire de la normale avec la direction de la lumière
        // qu'on limite entre 0 et 1 (ce qui est négatif est dans l'ombre)
        //vec3 sun_direct = normalize(vec3 (1.0,0.50, 0.20));
        //float sun_diffuse = clamp(dot(normal,sun_direct), 0.0, 1.0);
        
        // on lance un nouveau rayon vers le soleil pour savoir si
        // le point est éclairé ou non en fonction des objets qui sont 
        // entre le point et le soleil
        // On décale le point de la surface pour éviter une intersection parasite
        //float sun_shadow = rayMarching( position+normal*0.001, sun_direct, time ).x;
        //if( sun_shadow<=20.0 )
        //    sun_shadow = 0.0;
        //else
        //    sun_shadow = 1.0;

        
        // Calcul de l'éclairage venant du ciel, avec un décalage pour ce qui vient du sol
        //float sky_diffuse = clamp(0.5+0.5*dot(normale,vec3(0.0,1.0,0.0)), 0.0, 1.0);
         
        // Calcul de reflet de lumière du sol
        //
        //float bounce_diffuse =  clamp(0.5+0.5*dot(normale,vec3(0.0,-1.0,0.0)), 0.0, 1.0);
        
        // Combinaison des couleurs
        //col = material*vec3(7.0,4.5,3.0)*sun_diffuse*sun_shadow;
        //col += material*vec3(0.5,0.8,0.9)*sky_diffuse;
        //col += material*vec3(0.7,0.3,0.2)*bounce_diffuse;
    }
    
    // gamma correction, à mettre dès le début pour travailler dans un rendu d'éclairage correct
    col = pow( col, vec3(0.4545) );
    }
    
    fragColor = vec4(col,1.0);
}
