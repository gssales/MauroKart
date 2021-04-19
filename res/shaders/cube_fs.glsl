#version 330 core

in vec4 position_world;
in vec4 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int n_lights;
uniform vec4 lights;
uniform vec3 colors;

out vec3 color;

void main()
{
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    vec4 p = position_world;
    vec4 n = normalize(normal);
    vec4 v = normalize(camera_position - p); // sentido da câmera em relação ao ponto atual
    
    vec3 Kd = vec3(0.0, 0.3, 0.8); // Refletância difusa
    vec3 Ks = vec3(1.0, 1.0, 1.0); // Refletância especular
    vec3 Ka = vec3(0.0, 0.15, 0.4); // Refletância ambiente
    float q = 120.0; // Expoente especular para o modelo de iluminação de Phong
    
    // Espectro da luz ambiente
    vec3 Ia = vec3(0.2, 0.2, 0.2);

    // vec3 lambert_diffuse_term = vec4(0.0, 0.0, 0.0);
    // vec3 phong_specular_term = vec4(0.0, 0.0, 0.0);
    // for (int i = 0; i < n_lights; i++)
    // {
    //     vec4 l = normalize(lights[i] - p);
    //     vec4 h = normalize(l + v);
    //     vec3 I = color[i];
    //     lambert_diffuse_term = lambert_diffuse_term + Kd * I * max(0, dot(n, l));
    //     phong_specular_term = phong_specular_term + Ks * I * pow(max(0, dot(n, h)), q)
    // }
    
    vec4 l = normalize(lights - p);
    vec4 h = normalize(l + v);
    vec3 I = colors;

    vec3 lambert_diffuse_term = Kd * I * max(0, dot(n, l));
    vec3 phong_specular_term = Ks * I * pow(max(0, dot(n, h)), q);

    // Termo ambiente
    vec3 ambient_term = Ka * Ia;

    // Cor final do fragmento calculada com uma combinação dos termos difuso,
    // especular, e ambiente. Veja slide 129 do documento Aula_17_e_18_Modelos_de_Iluminacao.pdf.
    color = lambert_diffuse_term + ambient_term + phong_specular_term;

    // Cor final com correção gamma, considerando monitor sRGB.
    // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
    color = pow(color, vec3(1.0,1.0,1.0)/2.2);
} 
