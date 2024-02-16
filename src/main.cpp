#define _CRT_SECURE_NO_WARNINGS 1
#define ENGINE 1
#if ENGINE
#include "../includes/Renderer.h"

using namespace std;

int main()
{
    Engine demo;
    demo.start();

    return 0;
}

#else

#endif