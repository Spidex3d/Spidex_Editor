#version 460
 //Basic gltf vert Shader

layout(location = 0) in vec3 aPos;      // Position
layout(location = 1) in vec3 aNormal;   // Normal
layout(location = 2) in vec2 aTexCoords; // UV Coordinates
//layout(location = 3) in vec3 aTangent;  // Tangent

uniform mat4 model;
uniform mat4 view;   // camMatrix
uniform mat4 projection;

out vec3 FragPos;   // World-space position of the vertex
out vec3 Normal;    // World-space normal vector
out vec2 TexCoords; // Texture coordinates
//out vec3 Tangent;   // Tangent vector

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal; // Transform normals to world space
    //Tangent = mat3(model) * aTangent;                  // Transform tangent to world space
    TexCoords = aTexCoords;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}


//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;
////layout (location = 2) in vec3 aColor;
//layout (location = 2) in vec2 aTexCoord;
//
//out vec3 crntPos;
//out vec3 Normal;
////out vec3 Color;
//out vec2 TexCoord;
//
//uniform mat4 view; // camMatrix
//uniform mat4 model;
//uniform mat4 projection;
//uniform mat4 rotation;
//uniform mat4 scale;
//
//void main()
//{
//    crntPos = vec3(model * projection * -rotation * scale * vec4(aPos, 1.0f));
//    Normal = aNormal;
//    //Color = aColor;
//    TexCoord = aTexCoord;
//
//    gl_Position = view * vec4(crntPos, 1.0);
//   
//}