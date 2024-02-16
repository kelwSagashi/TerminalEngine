#ifndef CAMERA_H
#define CAMERA_H
#include "../includes/_3dFunctions.h"
#include "../includes/EngineInput.h"

using namespace std;

class Camera
{
public:
	Math3d::mat4x4 matProj;	// Matrix that converts from view space to screen space
	Math3d::vec3d vCamera;	// Location of camera in world space
	Math3d::vec3d vLookDir;	// Direction vector along the direction camera points
    Math3d::vec3d vLookSide;
	vector<Math3d::vec3d> pointsAround;
	float fov; float pixelAspect; float aspect; float sensibility;
	float fYaw;
    float fPitch;
	int windowWidth;
	int windowHeight;	
	bool freeLook;

public:

	void init(float mfov, float mpixelAspect, float maspect, bool fLook, int w, int h);
	void configureProjection();
	Math3d::mat4x4 view();
	Math3d::vec3d getPointInLookDirection(float n);
	void rotate();
	void move();
public:
	POINT g_CurrentMousePos;
};

#endif