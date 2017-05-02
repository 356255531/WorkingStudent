varying vec2 tcoord;
uniform sampler2D tex;
uniform vec2 texelsize;
void main(void) 
{
    vec2 width_step = vec2(texelsize.r, 0.0);
    vec2 height_step = vec2(0.0, texelsize.g);

    vec4 bottom_left_color =    texture2D(  tex,
                                            tcoord + height_step - width_step);
    vec4 bottom_color =         texture2D(  tex,
                                            tcoord + height_step);
    vec4 bottom_right_color =   texture2D(  tex,
                                            tcoord + height_step + width_step);
    vec4 top_left_color =       texture2D(  tex,
                                            tcoord - height_step - width_step);
    vec4 top_color =            texture2D(  tex,
                                            tcoord - height_step);
    vec4 top_right_color =      texture2D(  tex,
                                            tcoord - height_step + width_step);
    vec4 left_color =           texture2D(  tex,
                                            tcoord - width_step);
    vec4 right_color =          texture2D(  tex,
                                            tcoord + width_step);
    vec4 center_color =         texture2D(  tex,
                                            tcoord);

    vec4 ret =  (top_left_color + top_right_color + bottom_left_color + bottom_right_color) / 16.0 + 
                    (top_color + bottom_color + left_color + right_color) / 8.0 + center_color / 4.0;

    ret.a = 1.0;
    gl_FragColor = ret;
}