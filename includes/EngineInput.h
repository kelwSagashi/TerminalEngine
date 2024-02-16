#ifndef ENGINEINPUT_H
#define ENGINEINPUT_H
#include <string>
#include <Windows.h>
#include <windows.h>
#include <iostream>

class EngineInput {
private:
    std::string input;
    int cursorPosition;
    std::string allowedCharacters;

    bool IsCapsLockOn();
    bool IsShiftPressed();
    bool IsAltGrPressed();
    void setCharacters(std::string s);
    void clearInput();
    void AddChar(char key);
    void RemoveChar();
    void MoveCursorLeft();
    void MoveCursorRight();
    bool IsCharacterAllowed(char c);
    void sendText();
public:
    float depthBufferValue; 
    bool chat;
    bool hasMsg;
    bool leftMouse;
    bool debug;
    POINT mouse;

    void resetMouse(POINT m);
    void start(std::string allowCharacters, LONG xx, LONG yy);
    void ManageKeys(DWORD vkCode);
    void recieveText(std::string& commands);
    void type(char key, DWORD vkCode);
    std::string getText();
    int GetCursorPosition();
};


extern EngineInput keyInput;
extern HHOOK keyboardHook;
extern HHOOK mouseHook;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);

#endif