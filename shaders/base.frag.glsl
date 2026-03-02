#version 410 core

struct Material
{
   float shininess;
   sampler2D diffuse;
   sampler2D specular;
};

struct Light 
{
   vec3 position;  
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

struct Camera 
{
   vec3 position;
};

out vec4 FragColor;
in vec3 normalVec;
in vec3 fragPos;
in vec2 texCoord;  

uniform Material material;
uniform Light light;  
uniform Camera camera;

void main()
{
   // ambient
   vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));
   
   // diffuse
   vec3 norm = normalize(normalVec);
   vec3 lightDir = normalize(light.position - fragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord)); 

   // specular
   vec3 cameraDir = normalize(camera.position - fragPos);
   vec3 reflectDir = reflect(-lightDir, norm); 
   float spec = pow(max(dot(cameraDir, reflectDir), 0.0), material.shininess);
   vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));  

   FragColor = vec4(ambient + diffuse + specular, 1.0);
}
