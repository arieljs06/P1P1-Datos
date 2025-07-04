#include <iostream>
#include "Menu.h"
#ifdef _WIN32
#include <windows.h>
#include "Marquee.h"
#endif


int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); 
    SetConsoleCP(CP_UTF8);      
    #endif
    Marquee marquee;
    marquee.start("  Universidad de Las Fuerzas Armadas \"ESPE\"  ");
    Menu menu;
    menu.mostrarMenu();
    return 0;
}