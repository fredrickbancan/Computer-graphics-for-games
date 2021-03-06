#shader tesscont
#version 410

layout(vertices = 4) out;

in vec4 vColor[];
in vec2 vTexCoord[];
in vec3 vNormal[];
in vec3 vWorldPos[];
uniform vec3 camWorldPos;

uniform float fogStart;
uniform float fogEnd;
uniform float detailStart;
uniform float detailEnd;

out Tess
{
    vec3 vWorldPos;
    vec3 normal;
    vec4 color;
    vec2 vTexCoord;
} Out[];

void main(void)
{
    if (gl_InvocationID == 0)
    {
        //Set level of teselation based on distance from camera and size of face.

        vec3 avgCenter = (vWorldPos[0] + vWorldPos[1] + vWorldPos[2] + vWorldPos[3]) / 4;
        float distToFace = distance(camWorldPos, avgCenter);
        float detail = distToFace < detailStart ? 1 : distToFace < detailEnd ? 0.5 : 0;
        float faceSize = distance(vWorldPos[1], vWorldPos[3]);//length of diagonal line of face
        float level = (faceSize / (sqrt(2) * 0.5)) * detail;

        level = distToFace > fogEnd ? 0 : level < 1 ? 1 : level;

        gl_TessLevelInner[0] = level;
        gl_TessLevelInner[1] = level;
        gl_TessLevelOuter[0] = level;
        gl_TessLevelOuter[1] = level;
        gl_TessLevelOuter[2] = level;
        gl_TessLevelOuter[3] = level;
    }

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    Out[gl_InvocationID].vWorldPos = vWorldPos[gl_InvocationID];
    Out[gl_InvocationID].normal = vNormal[gl_InvocationID];
    Out[gl_InvocationID].color = vColor[gl_InvocationID];
    Out[gl_InvocationID].vTexCoord = vTexCoord[gl_InvocationID];
}

#shader tesseval
#version 410
layout(quads) in;

in Tess
{
    vec3 vWorldPos;
    vec3 normal;
    vec4 color;
    vec2 vTexCoord;
} In[];


out vec4 vColor;
out vec2 vTexCoord;
out float visibility;

uniform float fogStart;
uniform float fogEnd;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 pointLights[8]; 
uniform int activeLights = 0;

uniform float positionResolution;

void main(void)
{
    //interpolate color
    vec4 xColorTop = mix(In[1].color, In[0].color, gl_TessCoord.x);// interpolate in horizontal color between vert. 0 and 1
    vec4 xColorBottom = mix(In[2].color, In[3].color, gl_TessCoord.x);// interpolate in horizontal color between vert. 3 and 2
    vColor = mix(xColorTop, xColorBottom, gl_TessCoord.y);
    //triangles do this: vColor = (gl_TessCoord.x * In[0].color) + (gl_TessCoord.y * In[1].color) + (gl_TessCoord.z * In[2].color);

    //interpolate texture coord
    vec2 xUVTop = mix(In[0].vTexCoord, In[1].vTexCoord, gl_TessCoord.x);// interpolate in horizontal coord between vert. 0 and 1
    vec2 xUVBottom = mix(In[3].vTexCoord, In[2].vTexCoord, gl_TessCoord.x);// interpolate in horizontal coord between vert. 3 and 2
    vTexCoord = mix(xUVTop, xUVBottom, gl_TessCoord.y);
    //triangles do this: vTexCoord = (gl_TessCoord.x * In[0].vTexCoord) + (gl_TessCoord.y * In[1].vTexCoord) + (gl_TessCoord.z * In[2].vTexCoord);

    //interpolate world pos
    vec3 wpTop = mix(In[1].vWorldPos, In[0].vWorldPos, gl_TessCoord.x);// interpolate in horizontal coord between vert. 0 and 1
    vec3 wpBottom = mix(In[2].vWorldPos, In[3].vWorldPos, gl_TessCoord.x);// interpolate in horizontal coord between vert. 3 and 2
    vec3 vWorldPos = mix(wpTop, wpBottom, gl_TessCoord.y);

    //interpolate normal
    vec3 normalTop = mix(In[1].normal, In[0].normal, gl_TessCoord.x);// interpolate in horizontal coord between vert. 0 and 1
    vec3 normalBottom = mix(In[2].normal, In[3].normal, gl_TessCoord.x);// interpolate in horizontal coord between vert. 3 and 2
    vec3 normal = normalize(mix(normalTop, normalBottom, gl_TessCoord.y));

    gl_Position = projectionMatrix * viewMatrix  * vec4(vWorldPos, 1);//recalculate gl_pos because we dont want the innacuracy of the vertex shader to stack on this one.

    //re-apply visibility for each tesselated vertex
    float distanceFromCam = length(gl_Position.xyz);
    visibility = (distanceFromCam - fogStart) / (fogEnd - fogStart);
    visibility = clamp(visibility, 0.0, 1.0);
    visibility = 1.0 - visibility;
    visibility *= visibility;

    //re-apply nostalgic vertex jitter to each tesselated vertex  
    distanceFromCam = clamp(gl_Position.w, -1, 1000);
    gl_Position.xy = round(gl_Position.xy * (positionResolution / distanceFromCam )) / (positionResolution / distanceFromCam );

    vec3 vertToLight;
    float dotToLight;
    float distToLight;
    float power;
    //apply lighting to each tesselated vertex
    for (int i = 0; i < activeLights; i++)
    {
        vertToLight = pointLights[i][0].xyz - vWorldPos;
        distToLight = length(vertToLight);
        power = (distToLight - pointLights[i][2][0]) / (pointLights[i][2][1] - pointLights[i][2][0]);
        power = clamp(power, 0.0, 1.0);
        power = 1.0 - power;
        power *= power;
        dotToLight = clamp(dot(normalize(vertToLight), normal), 0, 1);
        vColor += pointLights[i][1] * dotToLight * pointLights[i][2][2] * power;
    }
}