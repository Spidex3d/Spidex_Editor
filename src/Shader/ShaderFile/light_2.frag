#version 460
struct Material 
{
    vec3 ambient;
    sampler2D diffuseMap;
    vec3 specular;
    float shininess;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float exponent;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cosInnerCone;
	float cosOuterCone;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	int on;

	float constant;
	float linear;
	float exponent;
};

  
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

#define MAX_POINT_LIGHTS 3

uniform DirectionalLight sunLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;
uniform vec3 viewPos;

out vec4 frag_color;

vec3 calcDirectionalLightColor(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLightColor(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotLightColor(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

//-----------------------------------------------------------------------------------------------
// Main Shader Entry
//-----------------------------------------------------------------------------------------------
void main()
{ 
	vec3 normal = normalize(Normal);  
	vec3 viewDir = normalize(viewPos - FragPos);

    // Ambient ----------------------------------------------------------------------------------
	vec3 ambient = spotLight.ambient * material.ambient * vec3(texture(material.diffuseMap, TexCoord));
	vec3 outColor = vec3(0.0f);	

	outColor += calcDirectionalLightColor(sunLight, normal, viewDir);

   for(int i = 0; i < MAX_POINT_LIGHTS; i++)
        outColor += calcPointLightColor(pointLights[i], normal, FragPos, viewDir);  

	// If the light isn't on then just return 0 for diffuse and specular colors
	if (spotLight.on == 1)
		outColor += calcSpotLightColor(spotLight, normal, FragPos, viewDir);

	frag_color = vec4(ambient + outColor, 1.0f);
}

//-----------------------------------------------------------------------------------------------
// Calculate the direction light effect and return the resulting 
// diffuse and specular color summation
//-----------------------------------------------------------------------------------------------
vec3 calcDirectionalLightColor(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);  // negate => Must be a direction from fragment towards the light

	// Diffuse ------------------------------------------------------------------------- --------
    float NdotL = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * NdotL * vec3(texture(material.diffuseMap, TexCoord));
    
     // Specular - Blinn-Phong ------------------------------------------------------------------
	vec3 halfDir = normalize(lightDir + viewDir);
	float NDotH = max(dot(normal, halfDir), 0.0f);
	vec3 specular = light.specular * material.specular * pow(NDotH, material.shininess);

	return (diffuse + specular);
}

//-----------------------------------------------------------------------------------------------
// Calculate the point light effect and return the resulting diffuse and specular color summation
//-----------------------------------------------------------------------------------------------
vec3 calcPointLightColor(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	// Diffuse ----------------------------------------------------------------------------------
    float NdotL = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * NdotL * vec3(texture(material.diffuseMap, TexCoord));
    
     // Specular - Blinn-Phong ------------------------------------------------------------------
	vec3 halfDir = normalize(lightDir + viewDir);
	float NDotH = max(dot(normal, halfDir), 0.0f);
	vec3 specular = light.specular * material.specular * pow(NDotH, material.shininess);

	// Attenuation using Kc, Kl, Kq -------------------------------------------------------------
	float d = length(light.position - FragPos);
	float attenuation = 1.0f / (light.constant + light.linear * d + light.exponent * (d * d));

	diffuse *= attenuation;
	specular *= attenuation;
	
	return (diffuse + specular);
}

//------------------------------------------------------------------------------------------------
// Calculate the spotlight effect and return the resulting // diffuse and specular color summation
//------------------------------------------------------------------------------------------------
vec3 calcSpotLightColor(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 spotDir  = normalize(light.direction);

	float cosDir = dot(-lightDir, spotDir);  // angle between the lights direction vector and spotlights direction vector
	float spotIntensity = smoothstep(light.cosOuterCone, light.cosInnerCone, cosDir);

	// Diffuse ----------------------------------------------------------------------------------
    float NdotL = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = spotLight.diffuse * NdotL * vec3(texture(material.diffuseMap, TexCoord));
    
     // Specular - Blinn-Phong ------------------------------------------------------------------
	vec3 halfDir = normalize(lightDir + viewDir);
	float NDotH = max(dot(normal, halfDir), 0.0f);
	vec3 specular = light.specular * material.specular * pow(NDotH, material.shininess);

	// Attenuation using Kc, Kl, Kq -------------------------------------------------------------
	float d = length(light.position - FragPos);
	float attenuation = 1.0f / (light.constant + light.linear * d + light.exponent * (d * d));

	diffuse *= attenuation * spotIntensity;
	specular *= attenuation * spotIntensity;
	
	return (diffuse + specular);
}




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
//