#include <iostream>
#include "Administrador.hpp"

int main(){
    Administrador a("za","31221", "22/12/2006","tata", "22");
    EstudanteGraduacao toby("Toby Rodrigues Neves","16602866621","04/04/2024","toby2024@gmail.com","22bonito", "120558", "141", "aluno");
    a.gerarCarteirinha(&toby); 
    return 0;
}