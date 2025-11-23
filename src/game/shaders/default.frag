#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 UV;

out vec4 FragColor;

uniform vec3 material_color;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

uniform vec3 material_ambient;
uniform vec3 material_diffuse;
uniform vec3 material_specular;
uniform float material_shininess;

uniform bool useTexture;
uniform sampler2D albedoTex;

void main()
{
    vec3 ambient = material_ambient * light_color;

    vec3 norm = normalize(Normal);
    vec3 light_dir = normalize(light_pos - FragPos);

    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * material_diffuse * light_color;

    vec3 view_dir = normalize(view_pos - FragPos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material_shininess);
    vec3 specular = material_specular * spec * light_color;

    vec3 tex = useTexture ? texture(albedoTex, UV).rgb : vec3(1.0, 1.0, 1.0);

    vec3 result = (ambient + diffuse * tex + specular);

    FragColor = vec4(result, 1.0);
}
