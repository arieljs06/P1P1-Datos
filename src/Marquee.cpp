#include "Marquee.h"
#include <chrono>

Marquee::Marquee(int width) 
    : consoleWidth(width), isRunning(false), scrollSpeed(100) {
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}

Marquee::~Marquee() {
    stop();
}

void Marquee::start(const std::string& message, int speed) {
    if (!isRunning) {
        currentMessage = message;
        scrollSpeed = speed;
        isRunning = true;
        hThread = CreateThread(NULL, 0, threadFunction, this, 0, NULL);
    }
}

void Marquee::stop() {
    if (isRunning) {
        isRunning = false;
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }
}

void Marquee::updateMessage(const std::string& newMessage) {
    currentMessage = newMessage;
}

DWORD WINAPI Marquee::threadFunction(LPVOID lpParam) {
    Marquee* marquee = static_cast<Marquee*>(lpParam);
    marquee->run();
    return 0;
}

void Marquee::run() {
    while (isRunning) {
        std::string paddedMessage = std::string(consoleWidth, ' ') + currentMessage + std::string(consoleWidth, ' ');
        
        for (size_t i = 0; i < currentMessage.length() + consoleWidth && isRunning; ++i) {
            // Guardar posición actual del cursor
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(consoleHandle, &csbi);
            COORD originalPos = csbi.dwCursorPosition;
            
            // Escribir en la primera línea
            COORD marqueePos = {0, 0};
            DWORD written;
            FillConsoleOutputCharacter(consoleHandle, ' ', consoleWidth, marqueePos, &written);
            WriteConsoleOutputCharacter(consoleHandle, paddedMessage.substr(i, consoleWidth).c_str(), 
                                      consoleWidth, marqueePos, &written);
            
            // Restaurar posición del cursor
            SetConsoleCursorPosition(consoleHandle, originalPos);
            
            Sleep(scrollSpeed);
        }
    }
}