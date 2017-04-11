varying vec2 tcoord;
uniform sampler2D tex;
uniform vec2 texelsize;
uniform int half_window_size;
void main(void) 
{
    vec4 ret = vec4(0);
    ret +=  texture2D(tex, vec2(-1, -1) * texelsize) + texture2D(tex, vec2(1, -1) * texelsize) + texture2D(tex, vec2(-1, 1) * texelsize) + texture2D(tex, vec2(1, 1) * texelsize) +
            texture2D(tex, vec2(0, -1) * texelsize) + texture2D(tex, vec2(0, 1) * texelsize) + texture2D(tex, vec2(-1, 0) * texelsize) + texture2D(tex, vec2(1, 0) * texelsize) +
            texture2D(tex, vec2(0, 0) * texelsize);
    ret.a = 1.0;
    gl_FragColor = ret;
}
