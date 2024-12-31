#version 460
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.0, 0.2, 1.0); 
}

//#version 460
//out vec4 FragColor;
//
//
//    in vec3 myColor;
//    in vec2 TexCoord;
//    in vec3 FragPos;
//
//    uniform sampler2D myTexture;
//    void main()
//    {
//     
//       FragColor = texture(myTexture, TexCoord);
//       //FragColor = vec4(myColor, 1.0f);
//       // FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
//
//    }