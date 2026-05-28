$input aPos, aNormal, aTexCoord
$output TexCoord, Normal

#include <bgfx_shader.sh>

void main() {
    gl_Position = mul(u_modelViewProj, vec4(aPos, 1.0));
    TexCoord = aTexCoord;
    Normal = aNormal;
}