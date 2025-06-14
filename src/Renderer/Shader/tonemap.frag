#version 450

layout(binding=0) uniform sampler2D uHdrTexture;

layout(location=0) in vec2 TexCoord;
// Reinhard
vec3 simpleTonemap(vec3 hdrColor) {
    return hdrColor / (1.0 + hdrColor);
    
    // float exposure = 1.0;
    // return vec3(1.0) - exp(-hdrColor * exposure);
}

void main() {
    vec4 hdrColor = texture(uHdrTexture, TexCoord);
    vec3 toneMapped = simpleTonemap(hdrColor.rgb);
    
    vec3 gammaCorrected = pow(toneMapped, vec3(1.0 / 2.2));
    
    gl_FragColor = vec4(gammaCorrected, hdrColor.a);
}