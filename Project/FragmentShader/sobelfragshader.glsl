
varying vec2 tcoord;
uniform sampler2D tex;
uniform vec2 texelsize;
void main(void) 
{
    vec2 width_step = vec2(texelsize.r, 0.0);
    vec2 height_step = vec2(0.0, texelsize.g);

    float bottom_left_color =    texture2D( tex,
                                            tcoord + height_step - width_step).r;
    float bottom_color =         texture2D( tex,
                                            tcoord + height_step).r;
    float bottom_right_color =   texture2D( tex,
                                            tcoord + height_step + width_step).r;
    float top_left_color =       texture2D( tex,
                                            tcoord - height_step - width_step).r;
    float top_color =            texture2D( tex,
                                            tcoord - height_step).r;
    float top_right_color =      texture2D( tex,
                                            tcoord - height_step + width_step).r;
    float left_color =           texture2D( tex,
                                            tcoord - width_step).r;
    float right_color =          texture2D( tex,
                                            tcoord + width_step).r;

    float x_gradient =    abs(-1.0 * bottom_left_color + -2.0 * left_color + -1.0 * top_left_color +
                             1.0 * bottom_right_color + 2.0 * right_color + 1.0 * top_right_color);
    float y_gradient =    abs(-1.0 * bottom_left_color + -2.0 * bottom_color + -1.0 * bottom_right_color + 
                            1.0 * top_left_color + 2.0 * top_color + 1.0 * top_right_color);

    gl_FragColor = vec4(    x_gradient * x_gradient,
                            y_gradient * y_gradient,     
                            x_gradient * y_gradient, 
                            1.0);
}