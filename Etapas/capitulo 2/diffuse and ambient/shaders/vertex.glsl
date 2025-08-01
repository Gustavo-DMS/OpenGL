#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

// out vec3 ourColor;
out vec2 texCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos.x , aPos.y, aPos.z, 1.0);
    FragPos = vec3(model * vec4(aPos,1.0));
    Normal = aNormal;
    // texCoord = vec2(aTexCoord.x, aTexCoord.y); // Flip the texture coordinate on the y-axis
};
