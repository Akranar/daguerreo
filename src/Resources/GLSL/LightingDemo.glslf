
#version 120

in vec3 position;
in vec3 normal;
in vec2 texcoord;
varying out vec4 color;
uniform sampler2D texture;


uniform vec3 global_ambient;
uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 eye_position;
uniform vec3 Ke;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec4 Ks;

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
