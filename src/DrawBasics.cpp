#include "../includes/Renderer.h"

void Renderer::DrawText(char pixel, string text, int posX, int posY, int space, Sprite* textImage){

    int image_width = textImage->nWidth;

    int CHAR_WIDTH = 8;

    for (auto c : text) {
        // Encontrar o índice do caractere no vetor de caracteres
        size_t charIndex = charsForText.find(c);
        if (charIndex != string::npos){
            // Calcular a posição inicial do caractere na imagem de dados
            int charStartX = (charIndex % (image_width / CHAR_WIDTH)) * CHAR_WIDTH;
            int charStartY = (charIndex / (image_width / CHAR_WIDTH)) * CHAR_WIDTH;

            // Copiar os pixels correspondentes ao caractere da imagem para a posição na tela
            for (int row = 0; row < CHAR_WIDTH; row++) {
                for (int col = 0; col < CHAR_WIDTH; col++) {
                    int screenPixelIndex = ((posY + row) * ScreenWidth()) + posX + col; 

                    int pixelPos = ((charStartY + row) * 8 * 56) + charStartX;

                    screen[screenPixelIndex] = (textImage->datas[0][pixelPos + col] != ' ') ? pixel : screen[screenPixelIndex];
                }
            }

            // Avançar para a próxima posição na tela para o próximo caractere
            posX += CHAR_WIDTH + space;
        }
    }
}

void Renderer::DrawLine(char pixel, int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;
    int err2;

    for (int t = 0; t < 1; t++) {
        int currentX = x1;
        int currentY = y1;

        while (1) {
            // Define a cor no pixel atual
            if (currentX >= 0 && currentX < ScreenWidth() && currentY >= 0 && currentY < ScreenHeight()) {
                screen[currentX + currentY * ScreenWidth()] = pixel;
            }

            if (currentX == x2 && currentY == y2) {
                break;
            }

            err2 = 2 * err;
            if (err2 > -dy) {
                err -= dy;
                currentX += sx;
            }
            if (err2 < dx) {
                err += dx;
                currentY += sy;
            }
        }
    }
}

void Renderer::DrawTriangle(char pixel, int x1, int y1, int x2, int y2, int x3, int y3) {
    DrawLine(pixel, x1, y1, x2, y2);
    DrawLine(pixel, x2, y2, x3, y3);
    DrawLine(pixel, x3, y3, x1, y1);
}

void Renderer::DrawThickenedLine(int thickness, char pixel, int x1, int y1, int x2, int y2){
    for (int i = 0; i < thickness; i++)
        DrawLine(pixel, x1+i, y1, x2+i, y2);
}

void Renderer::CleanPixels(char pixel){
    for (int y = 0; y < ScreenHeight(); y++) {
        for (int x = 0; x < ScreenWidth(); x++) {
            screen[x + y * ScreenWidth()] = pixel;
        }
    }
}

void Renderer::Draw(int x, int y, char col){
    if (x >= 0 && x < ScreenWidth() && y >= 0 && y < ScreenHeight())
        screen[x + y * ScreenWidth()] = col;
}

void Renderer::FillTriangle(char pixel, int x1, int y1, int x2, int y2, int x3, int y3) {
    // Ordena os pontos para que y1 <= y2 <= y3
    if (y1 > y2) {
        int tempX = x1;
        int tempY = y1;
        x1 = x2;
        y1 = y2;
        x2 = tempX;
        y2 = tempY;
    }
    if (y1 > y3) {
        int tempX = x1;
        int tempY = y1;
        x1 = x3;
        y1 = y3;
        x3 = tempX;
        y3 = tempY;
    }
    if (y2 > y3) {
        int tempX = x2;
        int tempY = y2;
        x2 = x3;
        y2 = y3;
        x3 = tempX;
        y3 = tempY;
    }

    // Calcula as inclinações das linhas
    float slopeA = (x2 - x1) / (float)(y2 - y1);
    float slopeB = (x3 - x1) / (float)(y3 - y1);
    float slopeC = (x3 - x2) / (float)(y3 - y2);
    

    // Preenche a primeira metade do triângulo
    for (int scanlineY = y1 + 1; scanlineY <= y2; scanlineY++) {
        int startX = (int)(x1 + slopeA * (scanlineY - y1));
        int endX   = (int)(x1 + slopeB * (scanlineY - y1));
        
        DrawLine(pixel, startX, scanlineY, endX, scanlineY);
    }

    // Preenche a segunda metade do triângulo
    for (int scanlineY = y2 + 1; scanlineY <= y3; scanlineY++) {
        int startX = (int)(x2 + slopeC * (scanlineY - y2));
        int endX   = (int)(x1 + slopeB * (scanlineY - y1));
        
        DrawLine(pixel, startX, scanlineY, endX, scanlineY);
    }
}