
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

    vec4 x_gradient_3d =    -1.0 * bottom_left_color + -2.0 * left_color + -1.0 * top_left_color +
                             1.0 * bottom_right_color + 2.0 * right_color + 1.0 * top_right_color;
    vec4 y_gradient_3d =    -1.0 * bottom_left_color + -2.0 * bottom_color + -1.0 * bottom_right_color + 
                            1.0 * top_left_color + 2.0 * top_color + 1.0 * top_right_color;

    float x_gradient = 0.21 * x_gradient_3d.r + 0.72 * x_gradient_3d.g + 0.07 * x_gradient_3d.b;
    float y_gradient = 0.21 * y_gradient_3d.r + 0.72 * y_gradient_3d.g + 0.07 * y_gradient_3d.b;

    gl_FragColor = vec4(    x_gradient, 
                            y_gradient,     
                            x_gradient * y_gradient, 
                            1.0);
}