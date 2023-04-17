#version 330 core 
layout (location = 0) in vec3 pos; 
layout (location = 1) in vec3 color; 
out vec3 customColor;


void main()
{
        gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
        customColor = color;
        // customColor = vec3(1.0f, 0.0f, 0.0f);
}
