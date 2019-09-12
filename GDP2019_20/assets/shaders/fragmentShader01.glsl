#version 420

in vec3 color;
in vec4 vertWorld;

// Get rid of this
uniform vec3 newColour;
// Make the shader use these:
uniform float newColourRed;
uniform float newColourGreen;
uniform float newColourBlue;

uniform vec3 lightPosition;


void main()
{
    //gl_FragColor = vec4(color, 1.0);
   //gl_FragColor = vec4(newColour, 1.0);
   
   float dist = distance( lightPosition, vertWorld.xyz );
   float linearAtten = 1.0f; 
   
   float atten = 1.0f / ( linearAtten * dist );
   
	
	gl_FragColor = vec4(newColourRed, newColourGreen, 
	                    newColourBlue, 1.0 );
	
	gl_FragColor *= atten;
}
