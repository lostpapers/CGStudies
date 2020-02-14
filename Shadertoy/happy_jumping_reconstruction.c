// Personal version of Inigo Quilez tutorial "Happy Jumping Deconstruction"
// 13-Feb-2020
//
// Great thanks to Inigo for his more than thorough tutorial
//
// I've understood most of the concepts and mathematical fundations underlying the SDF rendering.
// The following themes are the ones I still have to study:
//  - sdEllipsoid()
//  - Better version of calcNormal()
//  - calcAO()


// Min smoothing fonction
float smoothMin( in float a, in float b, in float k)
{
    // h is positive when the distance between a and b values is less than k.
    // Maximum is attained when a==b
    float h = max( k-abs(a-b), 0.0 );
    
    // To be sure that the returned value is smooth, a square fonction
    // is used to ensure derivative continuity at 0.
    return min( a, b )-h*h*0.25/k;
}


// Max smoothing fonction 
float smoothMax( in float a, in float b, in float k)
{
    // See smoothMin() comments
    float h = max( k - abs(a-b), 0.0 );
    return max( a, b )+h*h*0.25/k;
}


// SDF sphere
float sdSphere( in vec3 position, in float radius )
{
    return length( position )-radius;
}


// SDF ellipsoid (Formula by Inigo Quilez)
float sdEllipsoid(in vec3 position, in vec3 radius)
{
    
	float k0 = length( position/radius );
    float k1 = length( position/(radius*radius) );
    
    return k0*(k0-1.0)/k1;
}


// SDF stick (Optimal algorithm by Inigo Quilez)
float sdStick( in vec3 p, in vec3 a, in vec3 b, in float ra, in float rb )
{
    vec3 ba = b-a;
    vec3 pa = p-a;
    
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );

    return length( pa-h*ba )-mix( ra, rb, h);
}

// Returns a Vec2 where x=distance, y=material
//
// List of materials
//  - Body = 2.0
//  - Eyes = 3.0
vec2 sdMonster(in vec3 position, in float time )
{
    // extract fract part of time = [0.0, 1.0[
    float animationTime = fract(time);
    
    // Parabolic jump movement
    float parabolic = 4.0 * animationTime * (1.0 - animationTime);

    vec3 center = vec3( 0.0,
	                    parabolic + 0.1,
		                time );

	// Body
    vec2 uu = normalize(vec2( 1.0, -4.0*(1.0-2.0*animationTime) ));
    vec2 vv = vec2(-uu.y, uu.x);
    
    // Expand & Squash effect
    float expandY = 0.5 + 0.5*parabolic;
	expandY += (1.0-smoothstep( 0.0, 0.4, parabolic))*(1.0-expandY);
	float expandZ = 1.0 / expandY;
    
    vec3 positionRelative = position - center;
	vec3 positionBody = positionRelative;
	
    positionBody.yz = vec2( dot(uu,positionBody.yz), dot(vv,positionBody.yz) );
	
	vec2 result = vec2( sdEllipsoid( positionBody, vec3(0.25,0.25*expandY,0.25*expandZ) ), 2.0); // 2.0 = body color
	
	
    // Head
    vec3 symetric = vec3( abs(positionRelative.x),positionRelative.yz);
    float dPart     = sdEllipsoid(positionRelative-vec3( 0, 0.20, 0.02), vec3( 0.08, 0.2, 0.15 ));
    result.x = smoothMin( result.x, dPart, 0.1 );
	
    dPart = sdEllipsoid(positionRelative-vec3( 0, 0.21,-0.1),  vec3( 0.20, 0.2, 0.2 ));
    result.x = smoothMin( result.x, dPart, 0.1 );

    
    // Ears
    dPart = sdStick( symetric, vec3(0.15,0.32,-0.05), vec3(0.2,0.2,-0.07), 0.01, 0.04 ) ;
    
	result.x = smoothMin( result.x, dPart, 0.01 );
    
	    
    // Mouth - Extend sides by deforming with a parabola y=f(x)
	// smoothMax + négative dist to carve
    dPart = sdEllipsoid(positionRelative-vec3(0.0,0.15+3.0*positionRelative.x*positionRelative.x,0.2), vec3(0.1,0.04,0.2));
   
    result.x = smoothMax( result.x, -dPart, 0.02 );
    

    // Eyebrows
    vec3 eyeLids = symetric - vec3(0.12,0.34,0.15); 
    eyeLids.xy = mat2(3,4,-4,3)/5.0*eyeLids.xy;// Rotation, données de matrice avec Pythagore, pas besoin de sin/cos
    float dEyeLids  = sdEllipsoid(eyeLids, vec3(0.06,0.03,0.05));
    result.x = smoothMin( dEyeLids, result.x, 0.04);
    
    // Eyes
    // Duplication par particularité des SDF, avec utilisation de valeur 
    // absolue pour faire croire  au point courant qu'il y a une deuxième sphère
    
    float dEye      = sdSphere( symetric-vec3(0.08,0.27,0.06), 0.065);
    if( dEye<result.x ) { result = vec2(dEye, 3.0); }
    
    float dIris = sdSphere( symetric-vec3(0.075,0.28,0.102), 0.0395);
    if( dIris<result.x ) { result = vec2(dIris, 4.0); }

    return result;
}

// Test si le point est à l'intérieur de la forme ou non
// et à quelle distance se trouve t il. 0 indique que
// l'on est sur la surface. On combine deux objets en retournant 
// celui qui est le plus proche
vec2 map( in vec3 position, float time )
{
    // test proximité du Monstre
    vec2 result = sdMonster( position, time );
    
    // test proximité Sphere de rayon 0.25 au point d'origine
    //float dSphere = sdSphere(position,0.25);
    //if( dSphere<result.x)
    //{
    //    result = vec2(dSphere,1.0);
    //}
    
    // test proximité Sol
    float dGround = position.y - (-0.1 + 0.05*(sin(2.0*position.x)+sin(2.0*position.z)));
    if( dGround<result.x)
    {
        result = vec2( dGround, 1.0 );
    }
    
    return result;
}

vec2 rayMarching( in vec3 ray_origin, vec3 ray_direct, float time )
{
#define NEAR_CLIP 0.5
#define FAR_CLIP 20.0
#define MAX_STEPS 250

	vec2 result = vec2(NEAR_CLIP, 0.0);
	
    float distance = NEAR_CLIP;
    for( int i=0; i<MAX_STEPS && distance<FAR_CLIP; i++)
    {
        // Get nearest point from scene
        vec2 impact = map(ray_origin + distance*ray_direct, time);
        
        // Point is sufficiently near
        if( impact.x<0.01 )
        {
        	result = vec2( distance, impact.y );
            break;
        }
        
        distance += impact.x;
    }
    
    return result;
}



// Compute normal with localized gradient
// cf. Inigo for better version
vec3 calcNormal(in vec3 pos, float time )
{
    vec2 e = vec2 (0.001,0.0);
    
    return normalize(
		vec3( map(pos+e.xyy, time).x-map(pos-e.xyy, time).x,
		      map(pos+e.yxy, time).x-map(pos-e.yxy, time).x,
			  map(pos+e.yyx, time).x-map(pos-e.yyx, time).x ));
   
}



// Unrealistic cast shadows
float castShadow( in vec3 ray_origin, vec3 ray_direct, float time )
{
    float res = 1.0;
    
    float distance = 0.0;
    for( int i=0; i<100; i++)
    {
        // pos = position du point d'échantillonage le long du rayon
        vec3 pos = ray_origin + distance*ray_direct;
        
        // test d'impact de l'objet
        float impact = map(pos, time).x;
        
        // Ombre fonction de proximité rayon/objet et proximité origin/objet
        res = min(res,10.0*impact/distance);
        if( impact<0.0)
            break;
        
        distance += impact;
        if( distance>20.0)
            break;
    }
    
    return clamp( res,0.0,1.0);
}

// Thanks to Inigo Quilez
// ... to be studied
float calcOcclusion( in vec3 pos, in vec3 nor, float time )
{
	float occ = 0.0;
    float sca = 1.0;
    for( int i=0; i<5; i++ )
    {
        float h = 0.01 + 0.11*float(i)/4.0;
        vec3 opos = pos + h*nor;
        float d = map( opos, time ).x;
        occ += (h-d)*sca;
        sca *= 0.95;
    }
    return clamp( 1.0 - 2.0*occ, 0.0, 1.0 );
}


vec3 RenderScene( vec3 ray_origin, vec3 ray_direct, float time ) 
{
    // La couleur par défaut est celle du ciel, qui devient plus claire vers le bas
    vec3 col = vec3(0.4, 0.75, 1.0) - 0.7 * ray_direct.y;
    
    // Effet atmosphérique vers l'horizon
    col = mix( col, vec3(0.7, 0.75, 0.8), exp(-10.0*ray_direct.y)); 

    // ray marching: on récupère distance d'impact et matériau
    vec2 impact = rayMarching( ray_origin, ray_direct, time );
    
    // On a trouvé quelque chose: materiau est défini
    if( impact.y>0.0 )
    {
        vec3 position = ray_origin + impact.x*ray_direct;
        vec3 normale = calcNormal(position, time);
        
        vec3 material = vec3(0.18);
        
        if( impact.y==1.0)
        {
            // Ground
        	material = vec3(0.05,0.1,0.02);
            float f = -1.0+2.0*smoothstep(-0.2, 0.2, sin( 18.0*position.x)+sin(18.0*position.y)+sin(18.0*position.z));
            material += f*vec3( 0.06, 0.06, 0.02 );
        }
        else if( impact.y==2.0)
        {
			// Body
        	material = vec3(0.2,0.1,0.02);
        }
        else if( impact.y==3.0)
        {
            // Eye
        	material = vec3(0.4,0.4,0.4);
        }
        else
        {
            // Iris
        	material = vec3(0.02);
        }
            
        
        // Calcul d'éclairage diffus par lumière directionnelle (soleil)
        // Produit scalaire de la normale avec la direction de la lumière
        // qu'on limite entre 0 et 1 (ce qui est négatif est dans l'ombre)
        vec3 sun_direct = normalize(vec3 (1.0,0.50, 0.20));
        float sun_diffuse = clamp(dot(normale,sun_direct), 0.0, 1.0);
        
        // On lance un nouveau rayon vers le soleil pour savoir si
        // le point est éclairé ou non en fonction des objets qui sont 
        // entre le point et le soleil
        // On décale le point de la surface pour éviter une intersection parasite
        //float sun_shadow = castShadow( position+normale*0.0001, sun_direct );
        //vec2 impact_sun = rayMarching( position+normale*0.01, sun_direct );
        
        float sun_shadow = 1.0;
        //if( impact_sun.y>0.0)
        //    sun_shadow = 0.0;
        
        
        // Calcul de l'éclairage venant du ciel, avec un décalage pour ce qui vient du sol
        float sky_diffuse = clamp(0.5+0.5*dot(normale,vec3(0.0,1.0,0.0)), 0.0, 1.0);
         
        // Calcul de reflet de lumière du sol
        float bounce_diffuse =  clamp(0.5+0.5*dot(normale,vec3(0.0,-1.0,0.0)), 0.0, 1.0);
        
        // Combinaison des couleurs
		vec3 lighting = vec3( 7.0, 4.5, 3.0 )*sun_diffuse*sun_shadow
		              + vec3( 0.5, 0.8, 0.9 )*sky_diffuse
					  + vec3( 0.7, 0.3, 0.2 )*bounce_diffuse;
					  
        col = material*lighting;
        
        // Rebond de lumière 
    }
	
	return col;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Screen normalization [-1.0, 1.0]
    vec2 p = (2.0*fragCoord-iResolution.xy)/iResolution.y;

	// Copy iTime, for fine control
    float time = iTime;

    // Camera: Y up, X right, Z toward us
    float animation = 10.0*iMouse.x/iResolution.x;
	vec3 ray_origin = vec3(2.0*sin(animation),0.5,time + 2.0*cos(animation));
    
    vec3 target = vec3 (0,0.5,time);
    vec3 ww = normalize(target-ray_origin);
    vec3 uu = normalize( cross(ww, vec3(0,1,0)));
    vec3 vv = normalize( cross(uu,ww) );
    
    // direction du rayon dans le monde, depuis le repère de la caméra
    // le facteur associé à ww permet de gérer la focale
    vec3 ray_direct = normalize( p.x*uu + p.y*vv + 1.8*ww );
        
	vec3 col = RenderScene( ray_origin, ray_direct, time );
    
    // gamma correction, à mettre dès le début pour travailler dans un rendu d'éclairage correct
    col = pow( col, vec3(0.4545) );
    
    fragColor = vec4(col,1.0);
}
