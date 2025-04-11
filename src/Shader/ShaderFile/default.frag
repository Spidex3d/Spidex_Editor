#version 460

    in vec2 TexCoord;
    out vec4 FragColor;

    in vec3 FragPos;

    uniform sampler2D myTexture;
    void main()
    {
     
       FragColor = texture(myTexture, TexCoord);
       

    }
    // Basic frag Shader