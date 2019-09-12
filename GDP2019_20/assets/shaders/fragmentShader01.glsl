#version 110

varying vec3 color;

// Get rid of this
uniform vec3 newColour;
// Make the shader use these:
uniform float newColourRed;
uniform float newColourGreen;
uniform float newColourBlue;

void main()
{
   // gl_FragColor = vec4(color, 1.0);
    gl_FragColor = vec4(newColour, 1.0);
}
