#version 430

struct DirLight {
	vec3 direction;
	float intensity;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material {
	sampler2D txtr;
	float shininess;
};

in vec2 UV;
in vec3 normalWorld;
in vec3 vertexPositionWorld;

uniform vec3 eyePositionWorld;
uniform DirLight dirlight;
uniform PointLight pointlight;
uniform Material material;
uniform int useNormal;
uniform sampler2D normalTxtr;

out vec4 Color;

vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir)
{
	vec3 lightDir = normalize(dirlight.direction - vertexPositionWorld);
	// diffuse shading
	float diff = max(dot(norm, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// combine results
	vec3 ambient = light.ambient * vec3(texture(material.txtr, UV));
	vec3 diffuse = light.intensity * light.diffuse * diff * vec3(texture(material.txtr, UV));
	vec3 specular = light.intensity * light.specular * spec * vec3(texture(material.txtr, UV));
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight pointlight, vec3 norm, vec3 vertexPositionWorld, vec3 viewDir)
{
	vec3 lightDir = normalize(pointlight.position - vertexPositionWorld);
	// diffuse shading
	float diff = max(dot(norm, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// attenuation
	float dist = length(pointlight.position - vertexPositionWorld);
	float attenuation = 1.0 / (pointlight.constant + pointlight.linear * dist + pointlight.quadratic * (dist * dist));  
	// combine results
    vec3 ambient = pointlight.ambient * vec3(texture(material.txtr, UV));
    vec3 diffuse = pointlight.diffuse * diff * vec3(texture(material.txtr, UV));
    vec3 specular = pointlight.specular * spec * vec3(texture(material.txtr, UV));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

void main()
{
	// properties
	vec3 norm = bool(useNormal) ? normalize(texture(normalTxtr, UV).rgb * 2.0 - 1.0) : normalize(normalWorld);
	vec3 viewDir = normalize(eyePositionWorld - vertexPositionWorld);

	// phase 1: Directional lighting
	vec3 result = CalcDirLight(dirlight, norm, viewDir);
	// phase 2: Point lights
	result += CalcPointLight(pointlight, norm, vertexPositionWorld, viewDir);

	Color = vec4(result, 1.0f);
}