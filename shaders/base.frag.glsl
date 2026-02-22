#version 410 core

out vec4 fragColor;

in vec3 normalVec;
in vec3 fragPos;
in vec2 texCoord;

uniform sampler2D texture1;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;  

void main()
{
   float ambientStrength = 0.1f;
   float specularStrength = 0.5;
   
   vec3 norm = normalize(normalVec);
   vec3 lightDir = normalize(lightPos - fragPos);

   vec3 cameraDir = normalize(cameraPos - fragPos);
   vec3 reflectDir = reflect(-lightDir, norm); 
   
   // ambient
   vec3 ambient = ambientStrength * lightColor;

   // diffuse
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightColor;

   // speculate
   float spec = pow(max(dot(cameraDir, reflectDir), 0.0), 32);
   vec3 specular = specularStrength * spec * lightColor;  

   vec4 result = vec4((ambient + diffuse + specular) * lightColor, 1.0f);
   fragColor = texture(texture1, texCoord) * result;
}
