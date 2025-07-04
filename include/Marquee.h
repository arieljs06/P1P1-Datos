#ifndef MARQUEE_H
#define MARQUEE_H

#include <string>
#include <windows.h>

class Marquee {
public:
    Marquee(int width = 80);
    ~Marquee();
    
    void start(const std::string& message, int speed = 100);
    void stop();
    void updateMessage(const std::string& newMessage);

private:
    void run();
    static DWORD WINAPI threadFunction(LPVOID lpParam);
    
    std::string currentMessage;
    bool isRunning;
    int consoleWidth;
    int scrollSpeed;
    HANDLE consoleHandle;
    HANDLE hThread;
};

#endif // MARQUEE_H