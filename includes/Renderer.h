#ifndef RENDERER_C_H
#define RENDERER_C_H
#include <thread>
#include <list>
#include <wchar.h>
#include <stdio.h>
#include <iomanip>
#include <chrono>
#include <string.h>
#include <windows.h>
#include <iostream>
#include <cstring>
#include <cwchar>
#include <Windows.h>
#include <unordered_set>
#include <sstream>
#include "_3dFunctions.h"
#include "Camera.h"
#include "consoleTerminal.h"
#include "EngineInput.h"
#include "Sprite.h"
#include "Chunks.h"

using namespace std;
using namespace Math3d;

struct Render3dParams{
	ChunkMap* chunk; 
	set<triangle>* meshLoadedTris; 
	Camera& camera; 
	vector<Sprite*>& textures; 
	float fTheta;
};

class Renderer
{
public:
    consoleTerminal *console;
    float aspect;
    float pixelAspect;
    char *gradient;
    int gradientSize;
	string charsForText;
	string currentCommand;
    char* screen;
    int timeElapsed;
	int framesCounter;
	int frames;
	float deltaTime;
	bool showMsg;
    bool running;
    float *pDepthBuffer = nullptr;
	bool wireframe = false;
	bool solid = true;

    Renderer();
    ~Renderer();
    void start();
    int ScreenHeight();
    int ScreenWidth();
	int WindowHeight();
    int WindowWidth();
    void terminalThread();

    //drawBasics
	void DrawText(char pixel, string text, int posX, int posY, int space, Sprite* textImage);
    void DrawLine(char pixel, int x1, int y1, int x2, int y2);
    void DrawTriangle(char pixel, int x1, int y1, int x2, int y2, int x3, int y3);
	void DrawThickenedLine(int thickness, char pixel, int x1, int y1, int x2, int y2);
    void CleanPixels(char pixel);
	void Draw(int x, int y, char col);
    void FillTriangle(char pixel, int x1, int y1, int x2, int y2, int x3, int y3);

    //override functions
    virtual bool onUserCreate() = 0;
    virtual bool update() = 0;

    //draw 3d functions
    void Draw3DLine(char pixel, Math3d::vec3d& pointA, Math3d::vec3d& pointB, Camera& camera);
	void ClipLine(vec2& A, vec2& B);
	void render3d(Render3dParams params);
	void TexturedTriangle(	int x1, int y1, float u1, float v1, float w1,
							int x2, int y2, float u2, float v2, float w2,
							int x3, int y3, float u3, float v3, float w3,
		Sprite *tex, uint8_t face);
    void rasterTriangles(Camera& camera, Math3d::mat4x4 matWorld, vector<Math3d::triangle>& vecTrianglesToRaster, set<Math3d::triangle>& tris);
};

class Engine : public Renderer
{
public:
	Engine(){
	}

private:
	Math3d::MeshObj mesh;
	Math3d::ChunkMap chunk;
	vector<vector<Math3d::vec3d>> meshVertexs;
	vector<vector<Math3d::Indices>> meshIndices;
	Math3d::vec3d light_direction;
	ostringstream ossCustom;
	vector<Sprite*> textures;
	Sprite *textMap;
	Camera camera;
	float fTheta;

public:
    bool onUserCreate() override;
    bool update() override;
	void DebugPlay();
};
#endif