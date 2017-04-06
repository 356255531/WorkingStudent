varying vec2 tcoord;
uniform sampler2D tex;
uniform vec2 texelsize;
uniform int half_window_size;
void main(void) 
{
    vec4 ret = vec4(0.0);
    for (int row = -half_window_size; row <= half_window_size; row++)
        for (int col = -half_window_size; col <= half_window_size; col++) 
            ret += texture2D(tex, tcoord + vec2(row, col) * texelsize);
    gl_FragColor = ret;
}
