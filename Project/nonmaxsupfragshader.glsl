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
    float multiplier = 1.0 - step(center_color.r, top_color);
    multiplier = multiplier * 1.0 - step(center_color.r, top_left_color);
    multiplier = multiplier * 1.0 - step(center_color.r, left_color);
    multiplier = multiplier * 1.0 - step(center_color.r, bottom_left_color);

    float max_value = max(center_color.r, bottom_color);
    max_value = max(max_value, bottom_right_color);
    max_value = max(max_value, right_color);
    max_value = max(max_value, top_right_color);
    max_value = max(max_value, threshold);

    gl_FragColor = vec4((center_color.rgb * step(max_value, center_color.r) * multiplier), 1.0);
}