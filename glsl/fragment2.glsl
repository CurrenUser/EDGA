#version 460 core
out vec4 color;
in vec3 colorOut;

void main() {
   color = vec4(colorOut.r ,colorOut.g, colorOut.b, 1.0f);
}