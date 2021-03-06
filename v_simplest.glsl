#version 330
//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 color; //kolor związany z wierzchołkiem
in vec4 normal; //wektor normalny w przestrzeni modelu
in vec2 texCoord0; //wsp teksturowania


//Zmienne interpolowane
out vec4 ic;    // przesłanie koloru do fragment shadera
out vec4 l[2];
out vec4 n;
out vec4 v;
out vec2 iTexCoord0;
void main(void) {
    int mapSize = 100;
    vec4 lp0 = vec4(mapSize/2, 15, mapSize/2, 1); //pozcyja światła, przestrzeń świata
    vec4 lp1 = vec4(-mapSize/2, 15, mapSize/2, 1); //pozcyja światła, przestrzeń świata
    vec4 temp = V * M * vertex;
    l[0] = normalize(V * lp0 - temp); //wektor do światła w przestrzeni oka
    l[1] = normalize(V * lp1 - temp); //wektor do światła w przestrzeni oka
    v = normalize(vec4(0, 0, 0, 1) - temp); //wektor do obserwatora w przestrzeni oka
    n = normalize(V * M * normal); //wektor normalny w przestrzeni oka
    iTexCoord0 = texCoord0;
    ic = color;
    gl_Position=P*V*M*vertex;
}