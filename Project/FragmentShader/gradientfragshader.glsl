varying vec2 tcoord;
uniform sampler2D tex;

void main(void) 
{
    vec4 gradient = texture2D(tex, tcoord);
    float gradient_r = gradient.r - 0.5;
    float gradient_g = gradient.g - 0.5;

    gl_FragColor = vec4(
                            gradient_r * gradient_r + 0.5,
                            gradient_g * gradient_g + 0.5,
                            gradient_r * gradient_g + 0.5,
                            1.0
                        );
}