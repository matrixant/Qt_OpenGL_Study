#version 330

uniform vec3 lightColor;
out vec4 fColor;

void main()
{
   fColor = vec4(lightColor, 1.0f);
//    fColor = vec4(0, 0, 0.3f, 1);
}
