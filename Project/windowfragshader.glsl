varying vec2 tcoord;
uniform sampler2D tex;
uniform vec2 texelsize;
uniform int half_window_size;
void main(void) 
{
    vec4 ret = vec4(0);
    for(int x_offset = -1; x_offset <= 1; x_offset++)
    {
        for(int y_offset = -1; y_offset <= 1; y_offset++)
        {
            vec2 offset = vec2(x_offset, y_offset);
            ret += texture2D(tex,tcoord + offset * texelsize);
        }
    }
    ret.a = 1.0;
    gl_FragColor = ret;
}
