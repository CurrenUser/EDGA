#version 460 core
uniform float effect;
out vec4 color;
in vec3 colorOut;

void main() {
   color = vec4(colorOut.r, effect ,colorOut.b, 1.0f);
}
