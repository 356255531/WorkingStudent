// varying vec2 tcoord;
// uniform sampler2D tex;
// uniform vec2 texelsize;
// void main(void) 
// {
//     vec4 tm1m1 = texture2D(tex,tcoord + vec2(-1, -1)* texelsize);
//     vec4 tm10 = texture2D(tex,tcoord + vec2(-1, 0)* texelsize);
//     vec4 tm1p1 = texture2D(tex,tcoord + vec2(-1, 1)* texelsize);
//     vec4 tp1m1 = texture2D(tex,tcoord + vec2(1, -1)* texelsize);
//     vec4 tp10 = texture2D(tex,tcoord + vec2(1, 0)* texelsize);
//     vec4 tp1p1 = texture2D(tex,tcoord + vec2(1, 1)* texelsize);
//     vec4 t0m1 = texture2D(tex,tcoord + vec2(0, -1)* texelsize);
//     vec4 t0p1 = texture2D(tex,tcoord + vec2(0, -1)* texelsize);

//     vec4 x_gradient_3d = -1.0*tm1m1 + -2.0*tm10 + -1.0*tm1p1 + 1.0*tp1m1 + 2.0*tp10 + 1.0*tp1p1;
//     vec4 y_gradient_3d = -1.0*tm1m1 + -2.0*t0m1 + -1.0*tp1m1 + 1.0*tm1p1 + 2.0*t0p1 + 1.0*tp1p1;

//     float x_gradient = 0.21 * x_gradient_3d.x + 0.72 * x_gradient_3d.y + 0.07 * x_gradient_3d.z;
//     float y_gradient = 0.21 * y_gradient_3d.x + 0.72 * y_gradient_3d.y + 0.07 * y_gradient_3d.z;

//     gl_FragColor = vec4(x_gradient, y_gradient, 0.0, 1.0);
// }

varying vec2 tcoord;
uniform sampler2D tex;
uniform vec2 texelsize;
void main(void) 
{
    vec4 centroid_point = texture2D(tex, tcoord);
    vec4 ret = vec4(1);
    if  (
            (texture2D(tex, tcoord + vec2(-2, -2) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(-2, -1) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(-2, 0) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(-2, 1) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(-2, 2) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(-1, -2) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(-1, -1) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(-1, 0) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(-1, 1) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(-1, 2) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(0, -2) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(0, -1) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(0, 1) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(0, 2) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(1, -2) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(1, -1) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(1, 0) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(1, 1) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(1, 2) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(2, -2) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(2, -1) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(2, 0) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(2, 1) * texelsize).a > centroid_point.a) ||
            (texture2D(tex, tcoord + vec2(2, 2) * texelsize).a > centroid_point.a) ||
            (centroid_point.a < 1.0)
        ){
        ret.r = centroid_point.r;
        ret.g = centroid_point.g;
        ret.b = centroid_point.b;
    }
    
    gl_FragColor = ret;
}
