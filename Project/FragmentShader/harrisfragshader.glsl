varying vec2 tcoord;
uniform sampler2D tex;

void main(void) 
{
    vec4 gradient = texture2D(tex, tcoord);

    float gradient_sum = gradient.x + gradient.y;
    float harris_response = (gradient.x * gradient.y - (gradient.z * gradient.z)) / (gradient_sum);
    // float harrisIntensity = gradient.x * gradient.y - (gradient.z * gradient.z) - 0.05 * gradient_sum * gradient_sum;
    gl_FragColor = vec4(vec3(harris_response * 10.0), 1.0);
}