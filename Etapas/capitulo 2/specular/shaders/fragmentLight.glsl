#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform float mixValue;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 lightColor;
uniform vec3 objectColor;
void main() {
    FragColor = vec4(1.0);

};
