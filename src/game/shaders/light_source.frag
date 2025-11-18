#version 330 core
in vec3 vertexColor;

out vec4 FragColor;

uniform vec3 material_color;
uniform vec3 light_color;

void main()
{
    float ambient_strength = 1.0;
    vec3 ambient = ambient_strength * light_color;

    vec3 result = ambient * material_color;
    FragColor = vec4(result, 1.0);
}
