#version 330
in vec4 vColor;
out vec4 fColor;
uniform vec2 iMouse;
uniform float uTime;

void main()
{
    //fColor = vColor;

    float speed = .1;
    float scale = 0.002;
    vec2 p = gl_FragCoord.xy * scale;
    for(int i=1; i<10; i++){
        p.x+=0.3/float(i)*sin(float(i)*3.*p.y+uTime*speed)+iMouse.x/1000.;
        p.y+=0.3/float(i)*cos(float(i)*3.*p.x+uTime*speed)+iMouse.y/1000.;
    }
    float r=cos(p.x+p.y+1.)*.5+.5;
    float g=sin(p.x+p.y+1.)*.5+.5;
    float b=(sin(p.x+p.y)+cos(p.x+p.y))*.3+.5;
    vec3 color = vec3(r,g,b);
    fColor = vec4(color,1);
}
