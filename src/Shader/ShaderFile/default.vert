#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    //FragPos = vec3(model * vec4(aPos, 1.0f));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}

// Basic vert Shader