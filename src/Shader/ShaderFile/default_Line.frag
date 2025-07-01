#version 460
out vec4 FragColor;

uniform vec3 color2;

void main()
{
    FragColor = vec4(color2, 1.0); 
   // FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}