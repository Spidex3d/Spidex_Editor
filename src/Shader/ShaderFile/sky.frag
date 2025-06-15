    #version 460 core
    uniform samplerCube skybox;

in vec3 TexCoords;
out vec4 FragColor;

void main() {
    FragColor = texture(skybox, TexCoords);
}

//out vec4 FragColor;
//
//in vec3 texCoords;
//
//uniform samplerCube skybox;
//
//void main()
//{
//	FragColor = texture(skybox, texCoords);
//}