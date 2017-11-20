#version 130

uniform sampler2D tex;

in vec2 fragTexCoord;

out vec4 finalColor;
in float trans;

void main() {
    finalColor = texture(tex, fragTexCoord);
    finalColor.w = trans;
}