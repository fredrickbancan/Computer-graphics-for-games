#shader vertex
#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec4 color;

uniform float fogStart = 10;
uniform float fogEnd = 40;

out vec2 vTexCoord;
out vec4 vColor;
out vec3 vNormal;
out vec3 vWorldPos;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

float positionResolution = 8192;
float innacuracyOverDistanceFactor = 16;

out float visibility;//for fog

void main()
{
	vec4 positionRelativeToCam = viewMatrix * modelMatrix * vec4(position, 1);

	float distanceFromCam = length(positionRelativeToCam.xyz);
	visibility = (distanceFromCam - fogStart) / (fogEnd - fogStart);
	visibility = clamp(visibility, 0.0, 1.0);
	visibility = 1.0 - visibility;
	visibility *= visibility;

	gl_Position = projectionMatrix * positionRelativeToCam;

	distanceFromCam = clamp(gl_Position.w, -1, 1000);
	//apply nostalgic vertex jitter
	gl_Position.xy = round(gl_Position.xy * (positionResolution / (distanceFromCam * innacuracyOverDistanceFactor))) / (positionResolution / (distanceFromCam * innacuracyOverDistanceFactor));


	vTexCoord = uv;
	vColor = color;
	vNormal = normal;
	vWorldPos = (modelMatrix * vec4(position, 1)).xyz;
}


#shader fragment
#version 330

layout(location = 0) out vec4 fragColor;  

in float visibility;

uniform vec3 fogColor;

//noperspective disables texture perspective for ps1 effect, must first impliment tesselation.
noperspective in vec2 vTexCoord;
in vec4 vColor;

uniform sampler2D uTexture;

void main()  
{  
	vec4 textureColor = texture(uTexture, vTexCoord) * vColor;
	if (textureColor.a < 0.01F)
	{
		discard;//cutout if texture has fully transparent texels
	}
	fragColor.rgb = mix(fogColor, textureColor.rgb, visibility);
	fragColor.a = 1;
}  
