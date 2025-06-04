#version 460
//layout(location = 0) in vec3 aPos;
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//
//void main()
//{
//    gl_Position = projection * view * model * vec4(aPos, 1.0);
//}
layout (location = 0) in vec3 aPos;

out float Height;
//out vec3 Position;

uniform mat4 model;
uniform mat4 view;  
uniform mat4 projection;

void main()
{
Height = aPos.y;
//Position = (view * model * vec4(aPos, 1.0)).xyz;
gl_Position = projection * view * model * vec4(aPos, 1.0);

}




//layout(location = 0) in vec3 aPos;       // Position
//layout(location = 1) in vec3 aNormal;   // Normal
//layout(location = 2) in vec2 aTexCoords; // Texture Coordinates
//
//uniform mat4 model;
//uniform mat4 view;   // camMatrix
//uniform mat4 projection;
//
//out vec3 FragPos;   // World-space position of the vertex
//out vec3 Normal;    // World-space normal vector
//out vec2 TexCoords; // Texture coordinates
//
//
//void main() {
//
//    FragPos = vec3(model * vec4(aPos, 1.0));
//    Normal = mat3(transpose(inverse(model))) * aNormal; // Transform normals to world space
//    TexCoords = aTexCoords;
//    
//   gl_Position = projection * view * vec4(FragPos, 1.0);
//}