#include "../includes/Camera.h"
#include <windows.h>
#include <Windows.h>

void Camera::init(float mfov, float mpixelAspect, float maspect, bool fLook, int w, int h){
    fov = mfov;
    pixelAspect = mpixelAspect;
    aspect = maspect;
    fPitch = 1.0f;
    fYaw = 0.0f;
    windowWidth = w;
    windowHeight = h;
    freeLook = fLook;
    vCamera = {0, 2, -2};
    sensibility = 0.3f;
    pointsAround = Math3d::generateSphere({0, 0, 0}, 4, 10);
    SetCursorPos(windowWidth/2, windowHeight/2);
    g_CurrentMousePos = keyInput.mouse;
}

void Camera::configureProjection(){
    matProj = Math3d::Matrix_MakeProjection(fov, pixelAspect, aspect, 1000.0f);	
}

Math3d::mat4x4 Camera::view(){
    Math3d::vec3d front;
    front.x = cos(Math3d::radians(fYaw)) * cos(Math3d::radians(fPitch));
    front.y = sin(Math3d::radians(fPitch));
    front.z = sin(Math3d::radians(fYaw)) * cos(Math3d::radians(fPitch));
    vLookDir = Math3d::Vector_Normalise(front);
    Math3d::vec3d up = {0, 1, 0};
    Math3d::vec3d dir = vLookDir + vCamera;
    Math3d::mat4x4 mat = Math3d::Matrix_PointAt(vCamera, dir, up);
    return Math3d::Matrix_QuickInverse(mat);
}

Math3d::vec3d Camera::getPointInLookDirection(float n) {
    // Calcula o ponto em frente à câmera a uma distância 'n'
    Math3d::vec3d vecMul = vLookDir * n;
    Math3d::vec3d pointInLookDirection = vCamera + vecMul;
    
    return pointInLookDirection;
}

void Camera::rotate(){
    g_CurrentMousePos = keyInput.mouse;

    POINT center;
    center.x = (LONG)(windowWidth /2);
    center.y = (LONG)(windowHeight/2);

    if (g_CurrentMousePos.x != center.x){
        int deltaX = g_CurrentMousePos.x - center.x;
        fYaw += static_cast<float>(deltaX) * sensibility;
        fYaw = fmod(fYaw + 180.0f, 360.0f);
        fYaw = (fYaw < 0 ? fYaw + 360.0f : fYaw) - 180.0f; 
    }

    if (g_CurrentMousePos.y != center.y) {     
        int deltaY = g_CurrentMousePos.y - center.y;
        fPitch -= static_cast<float>(deltaY) * sensibility;
        // Restringindo a rotação vertical entre limites
        const float verticalLimit = 85.0f; // Altere esse valor conforme necessário
        if (fPitch > verticalLimit)
            fPitch = verticalLimit;
        else if (fPitch < -verticalLimit)
            fPitch = -verticalLimit;
    }

    keyInput.resetMouse(center);
    SetCursorPos(center.x, center.y);
}

void Camera::move() {
    const float cameraSpeed = 0.1f;

    if (GetAsyncKeyState(VK_SPACE))
        vCamera.y += cameraSpeed;	// Travel Upwards

    if (GetAsyncKeyState(VK_LSHIFT))
        vCamera.y -= cameraSpeed;	// Travel Downwards

    if (GetAsyncKeyState('W')) {
        if (freeLook){
            Math3d::vec3d w = vLookDir * cameraSpeed;
            vCamera = vCamera + w;
        }else{
            Math3d::vec3d v1 = { cos(Math3d::radians(fYaw)), 0.0f, sin(Math3d::radians(fYaw)) };
            v1 = Math3d::Vector_Normalise(v1);
            Math3d::vec3d v2 = v1 * cameraSpeed;
            vCamera = vCamera + v2;
        }
    }
    if (GetAsyncKeyState('S')) {
        if (freeLook){
            Math3d::vec3d s = vLookDir * cameraSpeed;
            vCamera = vCamera - s;
        }else{
            Math3d::vec3d v1 = { cos(Math3d::radians(fYaw)), 0.0f, sin(Math3d::radians(fYaw)) };
            v1 = Math3d::Vector_Normalise(v1);
            Math3d::vec3d v2 = v1 * cameraSpeed;
            vCamera = vCamera - v2;
        }
    }

    if (GetAsyncKeyState('D')) {
        if (freeLook){
            Math3d::vec3d up = {0, 1, 0};
            Math3d::vec3d cross = Math3d::Vector_CrossProduct(vLookDir, up);
            Math3d::vec3d cnormal = Math3d::Vector_Normalise(cross);
            Math3d::vec3d d = cnormal* cameraSpeed;
            vCamera = vCamera + d;
        }else{
            Math3d::vec3d v1 = { sin(Math3d::radians(fYaw)), 0.0f, -cos(Math3d::radians(fYaw)) };
            v1 = Math3d::Vector_Normalise(v1);
            Math3d::vec3d v2 = v1 * cameraSpeed;
            vCamera = vCamera - v2;
        }
    }

    if (GetAsyncKeyState('A')) {
        if (freeLook){
            Math3d::vec3d up = {0, 1, 0};
            Math3d::vec3d cross = Math3d::Vector_CrossProduct(vLookDir, up);
            Math3d::vec3d cnormal = Math3d::Vector_Normalise(cross);
            Math3d::vec3d a = cnormal * cameraSpeed;
            vCamera = vCamera - a;
        }else{
            Math3d::vec3d v1 = { sin(Math3d::radians(fYaw)), 0.0f, -cos(Math3d::radians(fYaw)) };
            v1 = Math3d::Vector_Normalise(v1);
            Math3d::vec3d v2 = v1 * cameraSpeed;
            vCamera = vCamera + v2;
        }
    }
}