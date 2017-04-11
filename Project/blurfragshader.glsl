varying vec2 tcoord;
uniform sampler2D tex;
uniform vec2 texelsize;
void main(void) 
{
    // vec4 ret = vec4(0);
    // ret +=  2.0 *   (
    //                     texture2D(tex, tcoord + vec2(-2, -2) * texelsize) + 
    //                     texture2D(tex, tcoord + vec2(2, -2) * texelsize) + 
    //                     texture2D(tex, tcoord + vec2(-2, 2) * texelsize) + 
    //                     texture2D(tex, tcoord + vec2(2, 2) * texelsize)
    //                 ) + 
    //         4.0 *   (
    //                     texture2D(tex, tcoord + vec2(-1, -2) * texelsize) + 
    //                     texture2D(tex, tcoord + vec2(1, -2) * texelsize) + 
    //                     texture2D(tex, tcoord + vec2(-1, 2) * texelsize) + 
    //                     texture2D(tex, tcoord + vec2(1, 2) * texelsize) +
    //                     texture2D(tex, tcoord + vec2(-2, -1) * texelsize) + 
    //                     texture2D(tex, tcoord + vec2(2, -1) * texelsize) + 
    //                     texture2D(tex, tcoord + vec2(-2, 1) * texelsize) + 
    //                     texture2D(tex, tcoord + vec2(2, 1) * texelsize)
    //                 ) +
    //         5.0 *   (
    //                     texture2D(tex, tcoord + vec2(0, -2) * texelsize) +
    //                     texture2D(tex, tcoord + vec2(0, 2) * texelsize) +
    //                     texture2D(tex, tcoord + vec2(-2, 0) * texelsize) +
    //                     texture2D(tex, tcoord + vec2(2, 0) * texelsize)
    //                 ) +
    //         9.0 *   (
    //                     texture2D(tex, tcoord + vec2(-1, -1) * texelsize) + 
    //                     texture2D(tex, tcoord + vec2(1, -1) * texelsize) + 
    //                     texture2D(tex, tcoord + vec2(-1, 1) * texelsize) + 
    //                     texture2D(tex, tcoord + vec2(1, 1) * texelsize)
    //                 ) +
    //         12.0 *  (
    //                     texture2D(tex, tcoord + vec2(0, -1) * texelsize) + 
    //                     texture2D(tex, tcoord + vec2(0, 1) * texelsize) + 
    //                     texture2D(tex, tcoord + vec2(-1, 0) * texelsize) + 
    //                     texture2D(tex, tcoord + vec2(1, 0) * texelsize)
    //                 ) +
    //         15.0 * texture2D(tex, vec2(0, 0));
    // ret /= 159.0;
    // ret.z = ret.x * ret.y;
    // ret.w = 1.0;
    // gl_FragColor = ret;

    vec4 col = vec4(0);
    float total_added = 0.0;
    for(int xoffset = -2; xoffset <= 2; xoffset++)
    {
        for(int yoffset = -2; yoffset <= 2; yoffset++)
        {
            vec2 offset = vec2(xoffset,yoffset);
            float prop = 1.0/(offset.x*offset.x+offset.y*offset.y+1.0);
            total_added += prop;
            col += prop*texture2D(tex,tcoord+offset*texelsize);
        }
    }
    col /= total_added;
    col.z = col.x * col.y;
    col.w = 1.0;
    gl_FragColor = col;
}
