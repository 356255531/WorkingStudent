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

    // gl_FragColor = vec4(
    //                         tex0_pixel.r + tex1_pixel.r,
    //                         tex0_pixel.r - tex1_pixel.r,
    //                         0,
    //                         1.0
    //                     );

    gl_FragColor = vec4(
                            0.5 + (tex0_pixel.r + tex1_pixel.r) / 4.0,
                            0.5 + (tex0_pixel.r - tex1_pixel.r) / 4.0,
                            0,
                            1.0
                        );
}