#include "../includes/Sprite.h"

Sprite::Sprite(const std::string& filename, short nPerCol, bool invert){
    n_PerCol = nPerCol;
    LoadTextFile(filename, nPerCol, invert);
}

void Sprite::LoadTextFile(const std::string& filename, short nPerCol, bool invert) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Failed to open file!" << std::endl;
        return;
    }

    std::string line;
    nHeight = 0;
    nWidth = 0;
    vector<string> sf;
    while (std::getline(file, line)) {
        sf.push_back(line);
        nHeight++;
    }
    if (invert)
        reverse(sf.begin(), sf.end());

    int w = 0;
    for (auto line : sf){
        for (char c : line){
            data.push_back(c);
            w++;
        }
    }
    nWidth = w/nHeight;
    // data.erase(std::remove(data.begin(), data.end(), '\n'), data.end());
    
    for (int i = 0; i < nPerCol * nPerCol; i++){
        vector<char> d;
        Math3d::vec2d t[4];
        Math3d::getUVCoordinates((uint8_t)i&0xFF, 3, (int)nPerCol, t);

        int startWidth = static_cast<int>(nWidth * t[0].u);
        int startHeight = static_cast<int>(nHeight * t[0].v);

        int endWidth = static_cast<int>(nWidth * t[2].u);
        int endHeight = static_cast<int>(nHeight * t[2].v);

        for (int y = startHeight; y < endHeight; y++){
            for (int x = startWidth; x < endWidth; x++){
                d.push_back(data[x + y * nWidth]);
            }
        }
        datas.push_back(d);
    }

    nWidth /= nPerCol;
    nHeight /= nPerCol;
    file.close();
}

char Sprite::SampleGlyph(float x, float y, uint8_t face)
{
    int sx = static_cast<int>((x * (float)nWidth))%nWidth;
    int sy = static_cast<int>((y * (float)nHeight))%nHeight;
    if (face >= datas.size())
        return '.';
    return datas[face][sy * nWidth + sx];
}

char Sprite::SampleGlyphQuad(float x, float y, uint8_t face)
{
    using namespace Math3d;
    vec2d t[4];
    getUVCoordinates(face, 3, 2, t);
    vec2d coord = repeatSample({x, y}, {0, 0}, {1, 1});

    coord.u = map(coord.u, 0, 1, t[0].u, t[2].u);
    coord.v = map(coord.v, 0, 1, t[0].v, t[2].v);

    int sx = static_cast<int>((coord.u * (float)nWidth))%nWidth;
    int sy = static_cast<int>((coord.v * (float)nHeight))%nHeight;

    int index = sy * nWidth + sx;
    if (index > nWidth * nHeight)
        return '.';
    return data[index];
}