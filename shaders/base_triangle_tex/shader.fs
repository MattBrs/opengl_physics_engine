#version 330 core

out vec4 FragColor;

in vec3 customColor;
in vec2 texCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
  // FragColor = texture(ourTexture, texCoord) * vec4(customColor, 1.0f);  

  // texture1, texture2, interpolation value to use between x and y.
  // 0.2 returns 80% of the first texture and 20% of the second
  FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);
}
