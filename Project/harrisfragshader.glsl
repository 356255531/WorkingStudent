varying vec2 tcoord;
uniform sampler2D tex;
uniform float threshold;
void main(void) 
{
    vec4 gradient_info = texture2D(tex, tcoord);

    float harris_response =   pow(gradient_info.r, 2.0) * pow(gradient_info.g, 2.0) - pow(gradient_info.b, 2.0) - 
    threshold * pow((pow(gradient_info.r, 2.0) + pow(gradient_info.g, 2.0)), 2.0);

    gradient_info.a = harris_response;

    gl_FragColor = gradient_info;
}
