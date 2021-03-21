#shader vertex
#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec4 color;

out vec2 vTexCoord;

void main()
{
	gl_Position = position;
	gl_Position.x *= 2.0;
	gl_Position.y *= 2.0;
	vTexCoord = uv;
	vTexCoord.y = 1.0 - uv.y;
}

#shader fragment
#version 330

layout(location = 0) out vec4 color;

in vec2 vTexCoord;
uniform sampler2D uTexture;
void main()
{
	vec4 texColor = texture2D(uTexture, vTexCoord);
	color = texColor;
	color.a = 1;
}