#include "../includes/consoleTerminal.h"
using namespace std;

consoleTerminal::consoleTerminal(){
    size = new rectSize(640, 142);
    wSize = new rectSize(1);
}

void consoleTerminal::initWindow(){
    ConsoleFullScreen(size, wSize, rectSize(11, 24));
    
    pixelw = 11.0f;
    pixelh = 24.0f;
    cout << "Atencao!!" << std::endl;
    cout << "O seu console deve estar com as seguintes configuracoes:" << std::endl;
    cout << "Clique no canto superior esquerdo do console, depois em 'padrao'" << std::endl;
    cout << "|Layout|" << std::endl;
    cout << "Tamanho do buffer de tela" << std::endl;
    cout << "altura: 124" << std::endl;
    cout << "[sim] Ajustar saida de texto ao redimensionar" << std::endl << std::endl;
    cout << "Tamanho da janela" << std::endl;
    cout << "Largura: 455 | Altura: 124" << std::endl << std::endl;
    cout << "Determine o tamanho da fonte (Recomendado: 6): ";
    
    int op = 0;

    std::cin >> op;
    if (op > 5){
        int fontH = op;
        int fontW = op/2;
        pixelw = (float)fontW;
        pixelh = (float)fontH;
    }

    FreeConsole();

    ConsoleFullScreen(size, wSize, rectSize((int)pixelw, (int)pixelh));
    
    int width = wSize->width/(int)pixelw;
    int height = wSize->height/(int)pixelh; 

    size->width = static_cast<int>(width - 5);
    size->height = static_cast<int>(height - 6);

    SetWindow(size);
    
    hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	dwBytesWritten = 0;

}

void consoleTerminal::SetWindow(rectSize* size)
{
	_COORD coord;
	coord.X = size->width;
	coord.Y = size->height;
	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = size->height - 1;
	Rect.Right = size->width - 1;
	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    
	SetConsoleScreenBufferSize(Handle, coord);
	SetConsoleWindowInfo(Handle, TRUE, &Rect);
}

void consoleTerminal::getConsoleSize(rectSize* size){
    // Obtenha o tamanho da janela em uma variável
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    size->width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    size->height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void consoleTerminal::ConsoleFullScreen(rectSize* size, rectSize* wSize, rectSize font)
{
    AllocConsole();
    // Desative o redimensionamento do console
    HWND console = GetConsoleWindow();
    LONG style = GetWindowLong(console, GWL_STYLE);
    style = style & ~(WS_SIZEBOX | WS_MAXIMIZEBOX);
    SetWindowLong(console, GWL_STYLE, style);
    ShowWindow(console, SW_SHOWMAXIMIZED);

    // Obtenha o tamanho do console
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    int windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int windowHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    CONSOLE_FONT_INFOEX cfi = CONSOLE_FONT_INFOEX(); 
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    cfi.nFont = 0;
    cfi.dwFontSize.X = font.width;
    cfi.dwFontSize.Y = font.height;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    std::wcscpy(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);

    // Redirecione a saída para o novo console
    freopen("CONOUT$", "w", stdout);

    // Agora você pode usar windowWidth e windowHeight
    size->width = windowWidth;
    size->height = windowHeight;

    RECT rect;
    GetWindowRect(console, &rect);
    int wWidth = rect.right - rect.left;
    int wHeight = rect.bottom - rect.top;

    wSize->width = wWidth;
    wSize->height = wHeight;
}