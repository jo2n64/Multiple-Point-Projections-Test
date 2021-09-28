#version 330

uniform mat4 mvpMatrix;

uniform mat4 projMatrixPartOne;
uniform mat4 projMatrixPartTwo;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

in vec3 vertex;
in vec3 color;

out vec3 fColor;
out mat4 matrixPartTwo;

void main (void) {
    //gl_Position = mvpMatrix * vec4(vertex,1);
    mat4 fullProjection = projMatrixPartOne*projMatrixPartTwo;
    gl_Position = fullProjection*viewMatrix*modelMatrix*vec4(vertex,1);
    fColor = color;
    matrixPartTwo = projMatrixPartTwo;
}

