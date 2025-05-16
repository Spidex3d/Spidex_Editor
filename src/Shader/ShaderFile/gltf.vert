#version 460
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent; // If you want normal mapping

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoords;
out vec3 fragPos;
out vec3 normal;
out vec3 Tangent;

void main()
{
    texCoords = aTexCoords;
    fragPos = vec3(model * vec4(aPos, 1.0));
    normal = mat3(transpose(inverse(model))) * aNormal;
    Tangent = mat3(model) * aTangent;  

    gl_Position = projection * view * vec4(fragPos, 1.0);
}





//layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec3 aNormal;
//layout(location = 2) in vec2 aTexCoords;
////layout(location = 3) in vec3 aTangent; // If you want normal mapping
//
//out vec2 TexCoords;
//out vec3 FragPos;
//out vec3 Normal;
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//
//void main() {
//    FragPos = vec3(model * vec4(aPos, 1.0));
//    Normal = mat3(transpose(inverse(model))) * aNormal;
//    TexCoords = aTexCoords;
//
//    gl_Position = projection * view * vec4(FragPos, 1.0);
//}




//layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec2 aTexCoord;
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//
//out vec2 TexCoord;
//
//void main() {
//    TexCoord = aTexCoord;
//    gl_Position = projection * view * model * vec4(aPos, 1.0);
//}
//

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
//    //FragPos = vec3(model * vec4(aPos, 1.0));
//    //Normal = mat3(transpose(inverse(model))) * aNormal; // Transform normals to world space
//
//   gl_Position = projection * view * model * vec4(aPos, 1.0);
//   TexCoords = aTexCoords;
//    
//}