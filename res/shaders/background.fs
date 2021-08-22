#version 120

uniform float deltaTime;
uniform sampler2D tex;

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(tex, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y - deltaTime));

    // multiply it by the color
    gl_FragColor = gl_Color * pixel;
}