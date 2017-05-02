varying vec2 tcoord;
uniform sampler2D tex;

void main(void) 
{
    vec4 gradient = texture2D(tex, tcoord);
    float gradient_sum = gradient.r + gradient.g;
    float harris_response = (gradient.r * gradient.g - (gradient.b * gradient.b)) / (gradient_sum);
    gl_FragColor = vec4(vec3(harris_response * 10.0), 1.0);
}