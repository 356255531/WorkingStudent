varying vec2 tcoord;
uniform sampler2D gradient_tex;
uniform sampler2D harris_tex;
uniform vec2 texelsize;
void main(void) 
{
    vec4 centroid_point_tex = texture2D(harris_tex, tcoord);
    float centroid_point = centroid_point_tex.x;
    gl_fragColor = texture2D(gradient_tex, tcoord);
    if  (
            (texture2D(harris_tex, tcoord + vec2(-2, -2) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(-2, -1) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(-2, 0) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(-2, 1) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(-2, 2) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(-1, -2) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(-1, -1) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(-1, 0) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(-1, 1) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(-1, 2) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(0, -2) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(0, -1) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(0, 1) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(0, 2) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(1, -2) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(1, -1) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(1, 0) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(1, 1) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(1, 2) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(2, -2) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(2, -1) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(2, 0) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(2, 1) * texelsize).x < centroid_point) &&
            (texture2D(harris_tex, tcoord + vec2(2, 2) * texelsize).x < centroid_point) &&
        ) {
            gl_fragColor = vec4(1, 1, 1, 1);
        }
}
