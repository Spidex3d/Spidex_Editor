#version 460
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D spriteTexture;

void main()
{
    FragColor = texture(spriteTexture, TexCoord);
}
