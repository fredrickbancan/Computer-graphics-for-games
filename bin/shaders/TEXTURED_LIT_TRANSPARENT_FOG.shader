#shader vertex
#version 330
//location is the location of the value in the vertex atrib array
//for vec4 position, the gpu automatically fills in the 4th component with a 1.0F. This means you can treat position as a vec4 no problem. (no need for messy conversions)
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec4 color;

 out vec2 vTexCoord;

//matrix for projection transformations.
uniform mat4 projectionMatrix;
//matrix for camera transformations.
uniform mat4 viewMatrix;
//matrix for model transformations. All transformations in this matrix are relative to the model origin.
uniform mat4 modelMatrix;

float positionResolution = 256.0F;
float innacuracyOverDistanceFactor = 1.0F;
void main()  
{  
   gl_Position =  projectionMatrix * viewMatrix * modelMatrix * position;
   //vertex jitter
   gl_Position.xy = floor(gl_Position.xy * (positionResolution / (gl_Position.w  * innacuracyOverDistanceFactor))) / (positionResolution / (gl_Position.w * innacuracyOverDistanceFactor));
   vTexCoord = texCoord;
}  

//Out variables from vertex shader are passed into the fragment shaders in variables, part of glsl language.
#shader fragment
#version 330

layout(location = 0) out vec4 color;  

//noperspective disables texture perspective for ps1 effect, must first impliment tesselation.
 in vec2 vTexCoord;

uniform sampler2D uTexture;

void main()  
{  
   vec4 texColor = texture(uTexture, vTexCoord);
   color = texColor;
}  
