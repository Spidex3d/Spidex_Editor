#version 460

    layout (location = 0) in vec3 aPos;
    //layout (location = 1) in vec3 aColor;
    layout (location = 1) in vec2 aTexCoord;

    in vec3 vertexIn;
    in vec4 colorIn;
    out vec4 colorOut;

    out vec3 FragPos;
    out vec3 myColor;
    out vec2 TexCoord;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
       colorOut = colorIn;

       FragPos = vec3(model * vec4(aPos, 1.0f));
       TexCoord = vec2(aTexCoord.x, aTexCoord.y);
       gl_Position = projection * view * (vertexIn, 1.0) * vec4(FragPos, 1.0);
       
       //gl_Position = projection * view * model * vec4(vertexIn, 1.0);

       //gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    };