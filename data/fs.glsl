#version 330

in vec2 v_texCoord;
out vec4 fragColor;

uniform sampler2D tex;

void main(){
	fragColor = texture(tex, v_texCoord); 
}
