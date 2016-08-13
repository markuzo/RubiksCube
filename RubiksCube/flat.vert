#version 430

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

in vec3 position;
in vec3 colorIn;

out vec3 colorPass;

void main() {
	colorPass = colorIn;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4( position, 1.0 );
}