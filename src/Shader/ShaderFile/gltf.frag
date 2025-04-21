#version 460
// glft.frag
// Inputs from vertex shader
in vec3 FragPos;
in vec3 Normal;
//in vec3 color;         // Base color passed from vertex shader
in vec2 TexCoords;     // Texture coordinates passed from vertex shader

// Outputs to the framebuffer
out vec4 FragColor;

// Material Textures
uniform sampler2D baseColorMap;       // Albedo/BaseColor map
uniform sampler2D metallicRoughnessMap; // Metallic-Roughness map
uniform sampler2D normalMap;          // Normal map
uniform sampler2D emissiveMap;        // Emissive map (optional)


// Uniform for the texture
uniform sampler2D textureMap;  // Texture sampler

void main()
{
   // float checker = mod(floor(TexCoords.x * 10.0) + floor(TexCoords.y * 10.0), 2.0);
    //FragColor = vec4(vec3(checker), 1.0); // Black and white checker pattern
    FragColor = texture(textureMap, TexCoords);
    //FragColor = vec4(TexCoords, 0.0, 1.0);
    //FragColor = vec4(0.0, TexCoords.y, 0.0, 1.0);
    
    
}
