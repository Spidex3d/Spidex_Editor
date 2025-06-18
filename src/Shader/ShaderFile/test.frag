#version 460

out vec4 FragColor;

void main()
{
    FragColor = vec4(0.2, 0.5, 1.0, 1.0); // Bright green
}


//out vec4 FragColor; // Output color
//
//in float Height;
//
//void main()
//{
//float h = (Height + 16)/32.0f;
// FragColor = vec4(h, h, h, 1.0);
////FragColor = vec4(0.2, 1.0, 0.3, 1.0); 
//}
//


//in vec3 FragPos;
//in vec3 Normal;
//in vec2 TexCoords;  // UV coordinates from the vertex shader
//
//
//uniform sampler2D baseColorMap;			// Albedo/BaseColor map
//uniform sampler2D metallicRoughnessMap; // Metallic-Roughness map
//uniform sampler2D normalMap;			// Normal map
//uniform sampler2D emissiveMap;        // Emissive map (optional)
//
//
//void main() {
//
// vec4 baseColor = texture(baseColorMap, TexCoords);
//
//    FragColor = texture(baseColorMap, TexCoords); // Sample texture color at UV coordinates
//}