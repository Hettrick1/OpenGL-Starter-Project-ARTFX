#version 330 core

in vec3 aPos;
in vec3 color;

out vec3 ourColor;
out vec3 pos;

uniform vec3 offset;

void main()
{
   gl_Position = vec4(aPos.x + offset.x, -aPos.y + offset.y, aPos.z + offset.z, 1.0);
   pos = vec3(color.x + offset.x, color.y - offset.x, color.z + offset.y);
};
