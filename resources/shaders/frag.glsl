#version 460 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform int useTexture = 0;
uniform vec3 baseColor = vec3(1.0, 0.5, 0.2);

void main(){
  vec4 color;
  if(useTexture == 1){
    color = texture(texture_diffuse1, TexCoords);
  } else {
    color = vec4(baseColor, 1.0);
  }

  FragColor = color;
}
