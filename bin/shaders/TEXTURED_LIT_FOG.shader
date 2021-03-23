#shader vertex
#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec4 color;

uniform float fogStart;
uniform float fogEnd;

out vec2 vTexCoord;
out vec4 vColor;
out vec3 vNormal;
out vec3 vWorldPos;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

uniform mat4 pointLights[8];
uniform int activeLights = 0;
uniform float positionResolution;

out float visibility;//for fog

void main()
{
	vec4 positionRelativeToCam = viewMatrix * modelMatrix * vec4(position, 1);

	gl_Position = projectionMatrix * positionRelativeToCam;

	float distanceFromCam = length(gl_Position.xyz);
	visibility = (distanceFromCam - fogStart) / (fogEnd - fogStart);
	visibility = clamp(visibility, 0.0, 1.0);
	visibility = 1.0 - visibility;
	visibility *= visibility;


	distanceFromCam = clamp(gl_Position.w, -0.1, 1000);
	//apply nostalgic vertex jitter
	gl_Position.xy = round(gl_Position.xy * (positionResolution / distanceFromCam)) / (positionResolution / distanceFromCam);

	vTexCoord = uv;
	vNormal = normalize(normalMatrix * normal);
	vWorldPos = (modelMatrix * vec4(position, 1)).xyz;

	vColor = vec4(0);
	vec3 vertToLight;
	float dotToLight;
	float distToLight;
	float power;

	for (int i = 0; i < activeLights; i++)
	{
		vertToLight = pointLights[i][0].xyz - vWorldPos;
		distToLight = length(vertToLight);
		power = (distToLight - pointLights[i][2][0]) / (pointLights[i][2][1] - pointLights[i][2][0]);//calc power based on distance to light and lights start/end distances.
		power = clamp(power, 0.0, 1.0);
		power = 1.0 - power;
		power *= power;
		dotToLight = clamp(dot(normalize(vertToLight), vNormal), 0, 1);
		vColor += color * pointLights[i][1] * dotToLight * pointLights[i][2][2] * power;
	}
	vColor.a = color.a;
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
uniform sampler2D ditherTexture;
void main()  
{  
	vec4 textureColor = texture2D(uTexture, vTexCoord) * vColor;
	if (textureColor.a < 0.01F)
	{
		discard;//cutout if texture has fully transparent texels
	}
	fragColor.rgb = mix(fogColor, textureColor.rgb, visibility);
	fragColor += vec4(texture2D(ditherTexture, gl_FragCoord.xy / 8.0).r / 32.0 - (1.0 / 128.0));//dithering
	fragColor.a = 1;
	if (visibility < 0) fragColor.r = 1.0;
}  
