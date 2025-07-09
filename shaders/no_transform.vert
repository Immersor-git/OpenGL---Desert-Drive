#version 330
layout (location=0) in vec3 vPosition;
out vec3 FragPos;
void main() {
    // Don't move the vertex at all; assume its local space == clip space.
    gl_Position = vec4(vPosition.x, vPosition.y, vPosition.z, 1) ;
    FragPos = vPosition;
}