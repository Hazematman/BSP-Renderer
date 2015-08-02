#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

uniform mat4 projView;
uniform mat4 model;

smooth out vec2 v_texCoord;

void main(){
	v_texCoord = texCoord;
	gl_Position = projView * model * vec4(position, 1.0);
}
