varying vec2 tcoord;
uniform sampler2D tex;
uniform vec2 texelsize;
void main(void) 
{
	vec3 tm1m1 = texture2D(tex,tcoord+vec2(-1,-1)*texelsize);
	vec3 tm10 = texture2D(tex,tcoord+vec2(-1,0)*texelsize);
	vec3 tm1p1 = texture2D(tex,tcoord+vec2(-1,1)*texelsize);
	vec3 tp1m1 = texture2D(tex,tcoord+vec2(1,-1)*texelsize);
	vec3 tp10 = texture2D(tex,tcoord+vec2(1,0)*texelsize);
	vec3 tp1p1 = texture2D(tex,tcoord+vec2(1,1)*texelsize);
	vec3 t0m1 = texture2D(tex,tcoord+vec2(0,-1)*texelsize);
	vec3 t0p1 = texture2D(tex,tcoord+vec2(0,-1)*texelsize);

	vec3 x_gradient_3d = -1.0*tm1m1 + -2.0*tm10 + -1.0*tm1p1 + 1.0*tp1m1 + 2.0*tp10 + 1.0*tp1p1;
	vec3 y_gradient_3d = -1.0*tm1m1 + -2.0*t0m1 + -1.0*tp1m1 + 1.0*tm1p1 + 2.0*t0p1 + 1.0*tp1p1;

	float x_gradient = (x_gradient_3d.x + x_gradient_3d.y + x_gradient_3d.z) / 3.0;
	float y_gradient = (y_gradient_3d.x + y_gradient_3d.y + y_gradient_3d.z) / 3.0;
	float gradient_product = x_gradient * y_gradient;

	gl_FragColor = vec3(x_gradient, y_gradient, gradient_product);
}
