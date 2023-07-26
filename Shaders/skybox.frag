#version 330 core

out vec4 FragColor;

in vec3 texCoord;

uniform vec3 viewColor;

uniform samplerCube skybox;

void main() {
	FragColor = texture(skybox, texCoord) + vec4(viewColor, 0.01);
}