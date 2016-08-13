#version 430

in vec3 colorPass;

out vec4 colorOut;

void main() {
	colorOut = vec4(colorPass, 1.0);
}