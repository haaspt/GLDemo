#version 330 core
in vec3 vertexColor;

out vec4 FragColor;

uniform vec3 material_color;
uniform vec3 light_color;

void main()
{
    vec3 ambient = light_color;

    vec3 result = ambient * material_color;
    FragColor = vec4(result, 1.0);
}
