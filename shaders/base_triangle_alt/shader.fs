#version 330 core
in vec3 positionColor;
uniform vec4 customColor;
out vec4 fragColor; 

void main() 
{ 
  // fragColor = customColor; 
  fragColor = vec4(positionColor, 1.0f);
}
