#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 Color;
layout (location = 2) in vec2 TexCoord;
//uniform vec4 mouse;
out vec3 aColor;
out vec4 aPosition;
out vec2 aTexCoord;
void main()
{
   gl_Position = vec4(aPos, 1.0);
   aColor = Color;
   aPosition = gl_Position;
   //aMouse = mouse;
   aTexCoord = TexCoord;
}
