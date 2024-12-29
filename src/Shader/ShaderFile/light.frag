#version 460


in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D texSampler1;
uniform vec3 lightPos[10]; 
uniform int numLights;
uniform vec3 lightColor;
uniform float ambientFactor;
uniform vec3 viewPos;

void main()
{
    // Ambient
    vec3 ambient = lightColor * ambientFactor;
    // Diffuse
    vec3 norm = normalize(Normal);
    // new bit
    vec3 result = vec3(0.0);

    for (int i = 0; i < numLights; i++){
    //vec3 lightDir = normalize(lightPos - FragPos);
    vec3 lightDir = normalize(lightPos[i] - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * diff;
    
    // specular phong
    float specularFactor = 0.8f;
    float shininess = 32.0f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float RDotV = max(dot(reflectDir, viewDir), 0.0f); //, material.shininess);
    vec3 specular = lightColor * specularFactor * pow(RDotV, shininess); //material.specular);
     
    result += (diffuse + specular); // new
    }
    result += ambient; // new
    vec4 texel = texture(texSampler1, TexCoords);
    FragColor = vec4(result, 1.0f) * texel;  // new

}
