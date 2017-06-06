varying vec2 tcoord;

uniform sampler2D tex;
uniform vec2 texelsize;
uniform float threshold;

void main(void) 
{
    vec2 width_step = vec2(texelsize.r, 0.0);
    vec2 height_step = vec2(0.0, texelsize.g);

    float bottom_color =        texture2D(  tex, 
                                            tcoord - height_step).r;
    float bottom_left_color =   texture2D(  tex, 
                                            tcoord - height_step - width_step).r;
    float bottom_right_color =  texture2D(  tex, 
                                            tcoord - height_step + width_step).r;
    float left_color =          texture2D(  tex, 
                                            tcoord - width_step).r;
    float right_color =         texture2D(  tex, 
                                            tcoord + width_step).r;
    float top_color =           texture2D(  tex, 
                                            tcoord + height_step).r;
    float top_right_color =     texture2D(  tex, 
                                            tcoord + height_step + width_step).r;
    float top_left_color =      texture2D(  tex, 
                                            tcoord + height_step - width_step).r;

    vec4 center_color =         texture2D(  tex, 
                                            tcoord);

    // Use a tiebreaker for pixels to the left and immediately above this one
    // float multiplier = 1.0 - step(center_color.r, top_color);
    // multiplier = multiplier * 1.0 - step(center_color.r, top_left_color);
    // multiplier = multiplier * 1.0 - step(center_color.r, left_color);
    // multiplier = multiplier * 1.0 - step(center_color.r, bottom_left_color);

    // float max_value = max(center_color.r, bottom_color);
    // max_value = max(max_value, bottom_right_color);
    // max_value = max(max_value, right_color);
    // max_value = max(max_value, top_right_color);

    float max_value_1 = max(bottom_left_color, bottom_left_color);
    float max_value_2 = max(bottom_right_color, left_color);
    float max_value_3 = max(right_color, top_color);
    float max_value_4 = max(top_right_color, top_left_color);
    max_value_1 = max(max_value_1, max_value_2);
    max_value_3 = max(max_value_3, max_value_4);
    max_value_1 = max(max_value_1, max_value_3);
    max_value_1 = max(max_value_1, center_color.r);

    // gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    gl_FragColor = vec4((vec3(1.0, 0.0, 0.0) * step(0.5, max_value_1) + center_color.rgb * step(max_value_1, 0.5)), 1.0);
}
