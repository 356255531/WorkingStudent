varying vec2 tcoord;
uniform sampler2D tex;
uniform vec2 texelsize;
void main(void) 
{
    vec4 tm1m1 = texture2D(tex,tcoord+vec2(-1,-1)*texelsize);
    vec4 tm10 = texture2D(tex,tcoord+vec2(-1,0)*texelsize);
    vec4 tm1p1 = texture2D(tex,tcoord+vec2(-1,1)*texelsize);
    vec4 tp1m1 = texture2D(tex,tcoord+vec2(1,-1)*texelsize);
    vec4 tp10 = texture2D(tex,tcoord+vec2(1,0)*texelsize);
    vec4 tp1p1 = texture2D(tex,tcoord+vec2(1,1)*texelsize);
    vec4 t0m1 = texture2D(tex,tcoord+vec2(0,-1)*texelsize);
    vec4 t0p1 = texture2D(tex,tcoord+vec2(0,-1)*texelsize);

    vec4 x_gradient = -1.0*tm1m1 + -2.0*tm10 + -1.0*tm1p1 + 1.0*tp1m1 + 2.0*tp10 + 1.0*tp1p1;
    vec4 y_gradient = -1.0*tm1m1 + -2.0*t0m1 + -1.0*tp1m1 + 1.0*tm1p1 + 2.0*t0p1 + 1.0*tp1p1;
    vec4 product_gradient= sqrt(x_gradient*x_gradient+y_gradient*y_gradient);

    vec4 ret = vec4(x_gradient, y_gradient, product_gradient, 1)

    gl_FragColor = clamp(ret,vec4(0),vec4(1));
}
