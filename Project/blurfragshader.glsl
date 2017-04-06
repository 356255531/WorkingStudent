varying vec2 tcoord;
uniform sampler2D tex;
uniform vec2 texelsize;
void main(void) 
{
    vec4 ret = vec4(0);
    float total_added = 0.0;
    for(int x_offset = -2; x_offset <= 2; x_offset++)
    {
        for(int y_offset = -2; y_offset <= 2; y_offset++)
        {
            vec2 offset = vec2(x_offset, y_offset);
            float prop = 1.0 / (offset.x * offset.x + offset.y * offset.y + 1.0);
            total_added += prop;
            ret += prop * texture2D(tex,tcoord + offset * texelsize);
        }
    }
    ret /= total_added;
    ret.z = ret.x * ret.y;
    gl_FragColor = ret;
}
