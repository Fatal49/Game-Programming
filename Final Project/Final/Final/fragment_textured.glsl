uniform sampler2D diffuse;
uniform float fade;

varying vec2 texCoordVar;

void main() {
    vec4 finalColor;
    
    finalColor = texture2D(diffuse, texCoordVar);
    finalColor.a *= fade;
    
    gl_FragColor = finalColor;
}