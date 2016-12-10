attribute vec4 position;
attribute vec4 color;

varying vec4 finalColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    finalColor = color;
	vec4 p = viewMatrix * modelMatrix  * position;
	gl_Position = projectionMatrix * p;
}