$input TexCoord, Normal

SAMPLER2D(DiffuseTexture, 0)

#include <bgfx_shader.sh>

void main() {
    vec4 texColor = texture2D(DiffuseTexture, TexCoord);
    gl_FragColor = texColor;
}