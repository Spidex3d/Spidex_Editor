#version 460
// gltf.vert
layout(location = 0) in vec3 aPos;      // Position
layout(location = 1) in vec3 aNormal;   // Normal
layout(location = 2) in vec2 aTexCoords; // UV Coordinates
//layout(location = 2) in vec3 aColor;     // base color

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 rotation;
uniform mat4 scale;

out vec3 FragPos;   // World-space position of the vertex
out vec3 Normal;    // World-space normal vector
out vec2 TexCoords; // Texture coordinates
//out vec3 color;     // base color

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal; // Transform normals to world space
    TexCoords = aTexCoords;
   // color = aColor;
	    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}





/* // calculates current position
	crntPos = vec3(model * translation * -rotation * scale * vec4(aPos, 1.0f));
	// Assigns the normal from the Vertex Data to "Normal"
	Normal = aNormal;
	// Assigns the colors from the Vertex Data to "color"
	color = aColor;
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
	
	// Outputs the positions/coordinates of all vertices
	gl_Position = camMatrix * vec4(crntPos, 1.0);*/