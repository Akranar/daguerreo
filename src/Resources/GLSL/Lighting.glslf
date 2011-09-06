
#version 120

in vec3 position;
in vec3 normal;
in vec2 texcoord;
varying out vec4 color;
uniform sampler2D texture;


uniform vec3 global_ambient = vec3(0.4f, 0.4f, 0.4f);
uniform vec3 light_color = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 light_position = vec3(1.0f, -4.0f, 4.0f);
uniform vec3 eye_position = vec3(0.0f, -15.0f, 0.0f);
uniform vec3 Ke = vec3(0, 0, 0);
uniform vec3 Ka = vec3(1.0f, 0.5f, 0.5f) * 0.5f;
uniform vec3 Kd = vec3(0.5f, 0.5f, 1.0f);
uniform vec4 Ks = vec4(0.0f, 0.5f, 0.2f, 50.0f);

void main()
{
    vec3 emissive = Ke;
    vec3 ambient = Ka * global_ambient;
    vec3 N = normalize(normal);
    vec3 L = normalize(light_position - position);
    vec3 diffuse = Kd * light_color * max(dot(N, L), 0);
    
    vec3 V = normalize(eye_position - position);
    vec3 H = normalize(L + V);
    float specular_light = pow(max(dot(N, H), 0), Ks.w);
    vec3 specular = Ks.xyz * light_color * specular_light;
    
    vec3 texcolor = texture2D(texture, texcoord).xyz;
    vec3 lighting_color = emissive + ambient + diffuse;
    color.xyz = lighting_color * texcolor + specular;
    color.w = 1.0f;
    return;
}
