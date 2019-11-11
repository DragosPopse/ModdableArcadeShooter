//https://www.youtube.com/watch?v=caQZKeAYgD8
//modified to be more customizable and support colored vignette, with the help of https://stackoverflow.com/questions/30122743/colored-vignette-shader-the-outer-part-libgdx

uniform sampler2D u_texture;
uniform vec2 u_resolution;
uniform vec4 u_vignetteColor = vec4(0., 1., 0., 1.);
uniform float u_intensity = .5;
uniform float u_innerRadius = .2;
uniform float u_outerRadius = .8;

void main()
{
    vec4 color = texture2D(u_texture, gl_TexCoord[0].xy);

    vec2 relativePosition = gl_FragCoord.xy / u_resolution - .5;

    float len = length(relativePosition);

    float opacity = smoothstep(u_innerRadius, u_outerRadius, len) * u_intensity;

    color.rgb = mix(color.rgb, u_vignetteColor, opacity);
    
    gl_FragColor = color;
}
