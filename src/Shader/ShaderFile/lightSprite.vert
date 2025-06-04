#version 460
//layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec3 aNormal;
//layout(location = 2) in vec2 aTexCoord;
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//
//out vec3 FragPos;
//out vec3 Normal;
//out vec2 TexCoords;
//
//void main()
//{
//    vec4 worldPosition = model * vec4(aPos, 1.0);
//    FragPos = worldPosition.xyz;
//
//    // Transform normal using inverse transpose of the model matrix
//    Normal = mat3(transpose(inverse(model))) * aNormal;
//
//    TexCoords = aTexCoord;
//    gl_Position = projection * view * worldPosition;
//}



layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;


uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
uniform float scale;

out vec2 TexCoord;

void main()
{
    // Remove rotation from view matrix (billboarding)
    mat4 billboardView = mat4(mat3(view)); 
    vec3 scaledPos = aPos * scale;
    vec4 worldPos = billboardView * vec4(scaledPos, 1.0);
    worldPos.xyz += lightPos;
       
    TexCoord = aTexCoord;
    gl_Position = projection * view * worldPos;
}


