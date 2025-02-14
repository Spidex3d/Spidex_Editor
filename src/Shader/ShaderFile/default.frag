#version 460
    out vec4 FragColor;

    in vec2 TexCoord;
    in vec3 FragPos;

    uniform sampler2D myTexture;

    void main()
    {
     
       FragColor = texture(myTexture, TexCoord);
       

    }
    // Basic frag Shader