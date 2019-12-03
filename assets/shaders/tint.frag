//https://stackoverflow.com/questions/48548292/sfml-c-flashing-hit-effect-on-sfsprite

uniform sampler2D u_texture;

uniform vec4 u_flashColor;

void main()
{


    vec4 color = texture2D(u_texture, gl_TexCoord[0].xy);
    float percent = u_flashColor.a;

    vec4 colorDifference = vec4(0,0,0,1);

    colorDifference.r = u_flashColor.r - color.r;
    colorDifference.g = u_flashColor.g - color.g;
    colorDifference.b = u_flashColor.b - color.b;
    color.r = color.r + colorDifference.r * percent;
    color.g = color.g + colorDifference.g * percent;
    color.b = color.b + colorDifference.b * percent;


    gl_FragColor = color; 

}