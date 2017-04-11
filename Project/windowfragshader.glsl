varying vec2 tcoord;
uniform sampler2D tex;
uniform vec2 texelsize;
void main(void) 
{
    vec4 col = vec4(0);
    for(int xoffset = -2; xoffset <= 2; xoffset++)
    {
        for(int yoffset = -2; yoffset <= 2; yoffset++)
        {
            vec2 offset = vec2(xoffset,yoffset);
            col += texture2D(tex,tcoord+offset*texelsize);
        }
    }
    col.w = 1.0;
    gl_FragColor = col;
}
