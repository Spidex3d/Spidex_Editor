#version 460
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;
// using this as a sun
struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};
struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float radius;
};


// Uniforms
uniform sampler2D texture_diffuse;
uniform DirectionalLight SunLight;
uniform PointLight PointLights[10];


void main()
{
    vec3 norm = normalize(Normal);
    vec3 albedo = texture(texture_diffuse, TexCoords).rgb;
    vec3 ambient = 0.1 * albedo;
    vec3 result = ambient;
    
    // Light direction is incoming light, so we invert it
    vec3 lightDir = normalize(-SunLight.direction);
    // Simple Lambertian diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    result += SunLight.color * diff * SunLight.intensity * albedo;
    
    // ---------- Point light(s)
    for (int i = 0; i < 1; ++i) {
        vec3 toLight = PointLights[i].position - FragPos;
        float distance = length(toLight);
        //vec3 lightDir = normalize(toLight);
        vec3 lightDir = normalize(PointLights[i].position - FragPos);

        float attenuation = 1.0 / (distance * distance / PointLights[i].radius);
        float diffP = max(dot(norm, lightDir), 0.0);

        result += PointLights[i].color * diffP * PointLights[i].intensity * attenuation * albedo;
    }
    
    FragColor = vec4(result, 1.0);

  
}






//in vec3 FragPos;
//in vec3 Normal;
//in vec2 TexCoords;
//
//out vec4 FragColor;
//
//uniform sampler2D texture_diffuse;
//
////##
////uniform sampler2D texSampler1;
////uniform vec3 lightPos;
////uniform vec3 lightColor;
////uniform vec3 viewPos;
////uniform float ambientFactor;
//
////###
//
//struct Light {
//    vec3 position;
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//    vec3 color;
//};
//
//uniform Light light;
//uniform vec3 viewPos;
//
//struct Material {
//    vec3 specular;
//    float shininess;
//};
//
//uniform Material matirial;
//
//void main()
//{
//    // Texture color
//    vec3 texColor = texture(texture_diffuse, TexCoords).rgb;
//
//    // Ambient
//    vec3 ambient = light.ambient * texColor;
//
//    // Diffuse
//    vec3 norm = normalize(Normal);
//    vec3 lightDir = normalize(light.position - FragPos);
//    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 diffuse = light.diffuse * diff * texColor;
//
//    // Specular
//    vec3 viewDir = normalize(viewPos - FragPos);
//    vec3 reflectDir = reflect(-lightDir, norm);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), matirial.shininess);
//    vec3 specular = light.specular * spec * matirial.specular;
//
//    vec3 result = ambient + diffuse + specular;
//    FragColor = vec4(result, 1.0);
//}
//
//





//in vec2 TexCoord;
//in vec3 FragPos;
//in vec3 Normal;
//
//out vec4 FragColor;
//
//uniform sampler2D texture_diffuse;
//uniform vec3 lightPos;
//uniform vec3 lightColor;
//uniform vec3 viewPos;
//uniform float ambientFactor;
//
//void main() {
// // Ambient
// float ambientFactor = 0.3f;
// vec3 ambient = lightColor * ambientFactor;
// //vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;
//
// // Diffuse
// vec3 norm = normalize(Normal);
// vec3 lightDir = normalize(lightPos - FragPos);
// float diff = max(dot(norm, lightDir), 0.0);
// vec3 diffuse = lightColor * diff;
// //vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;
//
// // specular phong
// float specularFactor = 0.8f;
// float shininess = 32.0f;
// vec3 viewDir = normalize(viewPos - FragPos);
// vec3 reflectDir = reflect(-lightDir, norm);
// float RDotV = max(dot(reflectDir, viewDir), 0.0f); //, material.shininess);
// vec3 specular = lightColor * specularFactor * pow(RDotV, shininess); //material.specular);
//
// vec4 texel = texture(texture_diffuse, TexCoord);
// FragColor = vec4(ambient + diffuse + specular, 1.0f) * texel;
//
//
////FragColor = texture(baseColor, TexCoord); 
//	
//}
////



//in vec2 TexCoords;
//in vec3 FragPos;
//in vec3 Normal;
//
//out vec4 FragColor;
//
//uniform sampler2D texSampler1;
//uniform vec3 lightPos;
//uniform vec3 lightColor;
//uniform vec3 viewPos;
//uniform float ambientFactor;
//
//void main()
//{
//    // Ambient
//    //float ambientFactor = 0.3f;
//    vec3 ambient = lightColor * ambientFactor;
//    //vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
//
//    // Diffuse
//    vec3 norm = normalize(Normal);
//    vec3 lightDir = normalize(lightPos - FragPos);
//    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 diffuse = lightColor * diff;
//    //vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
//
//    // specular phong
//    float specularFactor = 0.8f;
//    float shininess = 32.0f;
//    vec3 viewDir = normalize(viewPos - FragPos);
//    vec3 reflectDir = reflect(-lightDir, norm);
//    float RDotV = max(dot(reflectDir, viewDir), 0.0f); //, material.shininess);
//    vec3 specular = lightColor * specularFactor * pow(RDotV, shininess); //material.specular);
//
//    vec4 texel = texture(texSampler1, TexCoords);
//    FragColor = vec4(ambient + diffuse + specular, 1.0f) * texel; 
//
//   
//}


