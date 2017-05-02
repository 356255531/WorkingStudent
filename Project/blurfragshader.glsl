varying vec2 tcoord;
uniform sampler2D tex;
uniform vec2 texelsize;
void main(void) 
{
    vec2 width_step = vec2(texelsize, 0.0);
    vec2 height_step = vec2(0.0, texelsize);

    vec4 ret = vec4(0);
    ret +=  2.0 *   (   texture2D(  tex, 
                                    tcoord - 2 * height_step - 2 * width_step) + 
                        texture2D(  tex, 
                                    tcoord - 2 * height_step + 2 * width_step) + 
                        texture2D(  tex, 
                                    tcoord + 2 * height_step - 2 * width_step) + 
                        texture2D(  tex, 
                                    tcoord + 2 * height_step + 2 * width_step)） + 
            4.0 *   (   texture2D(  tex, 
                                    tcoord - 2 * height_step - width_step) + 
                        texture2D(  tex, 
                                    tcoord - 2 * height_step + width_step) + 
                        texture2D(  tex, 
                                    tcoord + 2 * height_step - width_step) + 
                        texture2D(  tex, 
                                    tcoord + 2 * height_step + width_step) +
                        texture2D(  tex, 
                                    tcoord - height_step - 2 * width_step) + 
                        texture2D(  tex, 
                                    tcoord - height_step + 2 * width_step) + 
                        texture2D(  tex, 
                                    tcoord + height_step - 2 * width_step) + 
                        texture2D(  tex, 
                                    tcoord + height_step + 2 * width_step)) +
            5.0 *   (   texture2D(  tex, 
                                    tcoord - 2 * height_step) +
                        texture2D(  tex, 
                                    tcoord + 2 * height_step) +
                        texture2D(  tex, 
                                    tcoord - 2 * width_step) +
                        texture2D(  tex, 
                                    tcoord + 2 * width_step)) +
            9.0 *   (   texture2D(  tex, 
                                    tcoord - height_step - width_step) + 
                        texture2D(  tex, 
                                    tcoord - height_step + width_step) + 
                        texture2D(  tex, 
                                    tcoord + height_step - width_step) + 
                        texture2D(  tex, 
                                    tcoord + height_step + width_step)) +
            12.0 *  (   texture2D(  tex, 
                                    tcoord - height_step) + 
                        texture2D(  tex, 
                                    tcoord + height_step) + 
                        texture2D(  tex, 
                                    tcoord - width_step) + 
                        texture2D(  tex, 
                                    tcoord + width_step)) +
            15.0 * texture2D(   tex, 
                                tcoord);

    ret /= 159.0;
    ret.w = 1.0;
    gl_FragColor = ret;
}
