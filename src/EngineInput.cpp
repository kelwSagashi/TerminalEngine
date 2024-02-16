#include "../includes/EngineInput.h"

bool EngineInput::IsCapsLockOn() {
    return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

bool EngineInput::IsShiftPressed() {
    return (GetKeyState(VK_SHIFT) & 0x8000) != 0;
}

bool EngineInput::IsAltGrPressed() {
    return (GetKeyState(VK_RMENU) & 0x8000) != 0;
}

void EngineInput::setCharacters(std::string s){
    allowedCharacters = s;
}

void EngineInput::clearInput(){
    input.clear();
    cursorPosition = 0;
}

void EngineInput::AddChar(char key) {
    if (cursorPosition >= 0 && cursorPosition <= static_cast<int>(input.length())) {
        if (IsCharacterAllowed(key)){
            key = (IsCapsLockOn() || (IsShiftPressed() && !IsCapsLockOn())) ? key : tolower(key);
            if (IsAltGrPressed() && (key == 'q' || key == 'Q')) key = '/';
            if (IsAltGrPressed() && (key == 'w' || key == 'W')) key = '?';
            input.insert(cursorPosition, 1, key);
            cursorPosition++;
        }
    }
}

void EngineInput::RemoveChar() {
    if (cursorPosition > 0 && cursorPosition <= static_cast<int>(input.length())) {
        input.erase(cursorPosition - 1, 1);
        cursorPosition--;
    }
}

void EngineInput::MoveCursorLeft() {
    if (cursorPosition > 0) {
        cursorPosition--;
    }
}

void EngineInput::MoveCursorRight() {
    if (cursorPosition < static_cast<int>(input.length())) {
        cursorPosition++;
    }
}

bool EngineInput::IsCharacterAllowed(char c) {
    return allowedCharacters.find(c) != std::string::npos;
}

void EngineInput::sendText(){
    if (!input.empty())
        hasMsg = true;
    else
        chat = false;
}

void EngineInput::resetMouse(POINT m){
    mouse = m;
}

void EngineInput::start(std::string allowCharacters, LONG xx, LONG yy){
    input = "";
    cursorPosition = 0;
    allowedCharacters = allowCharacters;
    chat = hasMsg = leftMouse = debug = false;
    mouse.x = xx;
    mouse.y = yy;
    depthBufferValue = 0;
}

void EngineInput::ManageKeys(DWORD vkCode){
    if (vkCode == 'T' && !chat)
        chat = true;
    
    if (vkCode == VK_F3)
        debug = !debug;
    
    if (vkCode == VK_OEM_PLUS)
        depthBufferValue += 0.01f;

    if (vkCode == VK_OEM_MINUS)
        depthBufferValue -= 0.01f;

    // VK_CONTROL
    // VK_TAB
}

void EngineInput::recieveText(std::string& commands){
    if (hasMsg){
        commands = input;
        for (char& c : commands)
            c = tolower(c);
        clearInput();
        chat = hasMsg = false;
    }
}

void EngineInput::type(char key, DWORD vkCode){
    if (key >= 32 && key <= 126) {
        AddChar(key);
    }
    else if (vkCode == VK_BACK) {
        RemoveChar();
    }
    else if (vkCode == VK_LEFT) {
        MoveCursorLeft();
    }
    else if (vkCode == VK_RIGHT) {
        MoveCursorRight();
    }else if (vkCode == VK_RETURN){
        sendText();
    }
}

std::string EngineInput::getText() {
    return input;
}

int EngineInput::GetCursorPosition() {
    return cursorPosition;
}

EngineInput keyInput;
HHOOK keyboardHook;
HHOOK mouseHook;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            DWORD vkCode = p->vkCode;
            char key = MapVirtualKey(vkCode, MAPVK_VK_TO_CHAR);

            if (vkCode == VK_ESCAPE) {  // Se a tecla Esc for pressionada, sai do loop
                PostQuitMessage(0);
            }

            if (keyInput.chat)
                keyInput.type(key, vkCode);

            keyInput.ManageKeys(vkCode);   
        }
    }

    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        MSLLHOOKSTRUCT* p = (MSLLHOOKSTRUCT*)lParam;
        if (wParam == WM_LBUTTONDOWN)
            keyInput.leftMouse = true;
        
        keyInput.mouse = p->pt;
    }

    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}