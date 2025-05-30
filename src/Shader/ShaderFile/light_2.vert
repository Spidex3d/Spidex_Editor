#version 460
layout (location = 0) in vec3 pos;			
layout (location = 1) in vec3 normal;	
layout (location = 2) in vec2 texCoord;

uniform mat4 model;			// model matrix
uniform mat4 view;			// view matrix
uniform mat4 projection;	// projection matrix

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
    FragPos = vec3(model * vec4(pos, 1.0f));			// vertex position in world space
    Normal = mat3(transpose(inverse(model))) * normal;	// normal direction in world space

	TexCoord = texCoord;

	gl_Position = projection * view *  model * vec4(pos, 1.0f);
}



//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;
//layout (location = 2) in vec2 aTexCoords;
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//
//
//out vec3 FragPos;
//out vec3 Normal;
//out vec2 TexCoords;
//
//
//void main() {
//    FragPos = vec3(model * vec4(aPos, 1.0f));
//    Normal = mat3(transpose(inverse(model))) * aNormal;
//    TexCoords = aTexCoords;
//    gl_Position = projection * view * model * vec4(aPos, 1.0f);
//    //gl_Position = projection * view * model * vec4(FragPos, 1.0);
//}
//