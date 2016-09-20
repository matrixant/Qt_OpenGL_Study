#version 330
in vec4 vColor;
in vec2 vTexCoord;

out vec4 fColor;

uniform sampler2D texture;

void main()
{
   fColor = texture2D(texture, vTexCoord);
//   fColor = texture2D(texture, vTexCoord) * vColor;
}
