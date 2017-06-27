varying vec2 tcoord;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform vec2 texelsize;
void main(void) 
{
    vec4 tex0_pixel =   texture2D(  tex0,
                                    tcoord);
    vec4 tex1_pixel =   texture2D(  tex1,
                                    tcoord);

    gl_FragColor = tex1_pixel * tex0_pixel;
}