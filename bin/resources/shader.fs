#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;

// Input uniform values
uniform sampler2D texture0;
uniform float frame;

// Output fragment color
out vec4 finalColor;

void main()
{

    vec4 texelColor = texture(texture0, fragTexCoord+vec2(0.1,frame));

    finalColor = texelColor;
}
