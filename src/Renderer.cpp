#include "../includes/Renderer.h"

Renderer::Renderer(){
    
}

Renderer::~Renderer(){
    delete []screen;
}

void Renderer::start(){
    console = new consoleTerminal();

    timeElapsed = 0;

    charsForText = "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz_-+0123456789.,!?:()/|@#''";
    keyInput.start(charsForText, WindowWidth()/2, WindowHeight()/2);

    {
        console->initWindow();
        aspect = (float)console->size->width / console->size->height;
        pixelAspect = console->pixelw / console->pixelh;
        char g[] = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@\xB1\xB2\xDB";
        gradient = g;
        gradientSize = sizeof(g) - 2;
        screen = new char[console->size->width * console->size->height + 1];
        running = onUserCreate();
        pDepthBuffer = new float[ScreenWidth() * ScreenHeight()];
        wireframe = false;
        solid = true;
        // thread t = thread(&Renderer::terminalThread, this);
        terminalThread();
    }
}

int Renderer::ScreenHeight() {return console->size->height;}
int Renderer::ScreenWidth() {return console->size->width;}
int Renderer::WindowHeight() {return console->wSize->height;}
int Renderer::WindowWidth() {return console->wSize->width;}

void Renderer::terminalThread(){
    frames = 0;
    framesCounter = 0;
    deltaTime = 0;
    showMsg = false;
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0);
    MSG msg;
    auto startTime = std::chrono::high_resolution_clock::now();
    
    while (running)
    {
        auto lastTime = std::chrono::high_resolution_clock::now();
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (GetKeyState(VK_ESCAPE) & 0x8000 && !keyInput.chat)
            running = false;
        
        if (!update())
            running = false;

        screen[console->size->width * console->size->height] = '\0';
        WriteConsoleOutputCharacter(console->hConsole, (LPCTSTR)screen, console->size->width * console->size->height, { 0, 0 }, &console->dwBytesWritten);
        
        if (!currentCommand.empty())
            currentCommand.clear();
        framesCounter++;
        
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsedTime = currentTime - startTime;

        std::chrono::duration<double> frameTime = currentTime - lastTime;
        deltaTime += static_cast<float>(frameTime.count());

        if (elapsedTime.count() >= 1.0) {
            frames = framesCounter;
            framesCounter = 0;
            startTime = std::chrono::high_resolution_clock::now();
            timeElapsed++;

        }	
    }

    UnhookWindowsHookEx(keyboardHook);
    UnhookWindowsHookEx(mouseHook);
}