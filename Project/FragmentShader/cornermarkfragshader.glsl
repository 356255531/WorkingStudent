varying vec2 tcoord;

uniform sampler2D tex;
uniform sampler2D decision_tex;

uniform vec2 texelsize;

void main(void) 
{
    vec2 width_step = vec2(texelsize, 0.0);
    vec2 height_step = vec2(0.0, texelsize);

    float bottom_color =        texture2D(  decision_tex, 
                                            tcoord - height_step).r;
    float bottom_left_color =   texture2D(  decision_tex, 
                                            tcoord - height_step - width_step).r;
    float bottom_right_color =  texture2D(  decision_tex, 
                                            tcoord - height_step + width_step).r;
    float left_color =          texture2D(  decision_tex, 
                                            tcoord - width_step).r;
    float right_color =         texture2D(  decision_tex, 
                                            tcoord + width_step).r;
    float top_color =           texture2D(  decision_tex, 
                                            tcoord + height_step).r;
    float top_right_color =     texture2D(  decision_tex, 
                                            tcoord + height_step + width_step).r;
    float top_left_color =      texture2D(  decision_tex, 
                                            tcoord + height_step - width_step).r;

    float center_color =         texture2D(  decision_tex, 
                                            tcoord).r;

    // Use a tiebreaker for pixels to the left and immediately above this one
    float multiplier =  step(top_left_color.r) + 
                        step(top_color.r) + 
                        step(top_right_color.r) + 
                        step(left_color.r) + 
                        step(center_color.r) + 
                        step(right_color.r) + 
                        step(bottom_left_color.r) + 
                        step(bottom_color.r) + 
                        step(bottom_right_color.r);
    multiplier /= multiplier;

    float red_color = vec4(1.0, 0.0, 0.0, 1.0);

    vec4 grey_scale_ret = texture2D(tex, tcoord);

    gl_FragColor = multiplier * red_color + (1 - multiplier) * grey_scale_ret;
}
