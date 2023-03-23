#version 330 core

out vec4 FragColor;

in vec3 customColor;
in vec2 texCoord;

uniform sampler2D ourTexture;

void main() {
  FragColor = texture(ourTexture, texCoord) * vec4(customColor, 1.0f);  
}
