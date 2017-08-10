#version 330 core

in vec3 ourColor;
in vec2 ourTex;

out vec4 color;

//texture samplers
uniform sampler2D ourTexture1;

void main() {
    //Linearly interpolate between both textures
    color = texture(ourTexture1, ourTex);
}
