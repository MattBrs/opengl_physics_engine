#version 330 core
uniform vec4 customColor;
out vec4 fragColor; 

void main() 
{ 
  fragColor = customColor; 
}
