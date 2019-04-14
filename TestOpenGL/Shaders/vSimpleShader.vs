#version 330

layout (location = 0) in vec3 aPosition;

uniform mat4 transform;

void main()
{
    gl_Position =  transform * vec4(aPosition.x,aPosition.y,aPosition.z,1.0f);
}