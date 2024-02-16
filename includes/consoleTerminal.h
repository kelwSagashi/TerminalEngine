#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include <string>
#include <string.h>
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cwchar>
#include <Windows.h>

using namespace std;

// typedef struct _CONSOLE_FONT_INFOEX {
//     ULONG cbSize;
//     DWORD nFont;
//     COORD dwFontSize;
//     UINT FontFamily;
//     UINT FontWeight;
//     WCHAR FaceName[LF_FACESIZE];
// } CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;

// #ifdef __cplusplus
// extern "C"{
// #endif
// BOOL WINAPI SetCurrentConsoleFontEx(
//     HANDLE               hConsoleOutput,
//     BOOL                 bMaximumWindow,
//     PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx
// );
// #ifdef __cplusplus
// }
// #endif

struct rectSize{
    int width, height;

    rectSize(int value) : width(value), height(value) {};
    rectSize(int _width, int _height) : width(_width), height(_height) {};

    rectSize operator*(int const& other) { return rectSize(width * other, height * other); }
};

class consoleTerminal
{
public:
    rectSize* size;
    rectSize* wSize;
    float pixelw, pixelh;
    HANDLE hConsole;
    DWORD dwBytesWritten;
    
    
    consoleTerminal();

    void initWindow();
    void initKeyboard();
    void SetWindow(rectSize* size);
    void getConsoleSize(rectSize* size);
    void ConsoleFullScreen(rectSize* size, rectSize* wSize, rectSize font);  

};