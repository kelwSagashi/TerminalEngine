#include "../includes/_3dFunctions.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <cwchar>
#include <unordered_set>
#include <unordered_map>
#include <array>
#include <set>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <vector>
# define M_PI           3.14159265358979323846f  /* pi */

using namespace std;

namespace Math3d{
    float frac(float x) {return abs(x) - floor(abs(x));}

    float repeat(float x, float min, float max){
        if (x < min)
            return max - frac(x);
        return frac(x) + min;
    }

    vec2d repeatSample(const vec2d v, const vec2d min, const vec2d max){
        return {repeat(v.u, min.u, max.u), repeat(v.v, min.v, max.v)};
    }

    float map(float value, float fromMinValue, float fromMaxValue, float toMinValue, float toMaxValue) {
        return (value - fromMinValue) * (toMaxValue - toMinValue) / (fromMaxValue - fromMinValue) + toMinValue;
    }

    float clamp1(float valor, float min, float max) {return std::max(min, std::min(valor, max));}  

    float radians(float degrees) {return degrees * M_PI / 180.0f;}

    float Vector_Magnitude(const vec3d& vec) { return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);}

    mat4x4 Matrix_MakeIdentity() {
        mat4x4 matrix;
        matrix.m[0][0] = 1.0f;
        matrix.m[1][1] = 1.0f;
        matrix.m[2][2] = 1.0f;
        matrix.m[3][3] = 1.0f;
        return matrix;
    }

    mat4x4 Matrix_Scale(vec3d& v) {
        mat4x4 matrix;
        matrix.m[0][0] = v.x;
        matrix.m[1][1] = v.y;
        matrix.m[2][2] = v.z;
        matrix.m[3][3] = 1.0f;
        return matrix;
    }

    mat4x4 Matrix_MakeRotationX(float fAngleRad) {
        mat4x4 matrix;
        matrix.m[0][0] = 1.0f;
        matrix.m[1][1] = cosf(fAngleRad);
        matrix.m[1][2] = sinf(fAngleRad);
        matrix.m[2][1] = -sinf(fAngleRad);
        matrix.m[2][2] = cosf(fAngleRad);
        matrix.m[3][3] = 1.0f;
        return matrix;
    }

    mat4x4 Matrix_MakeRotationY(float fAngleRad) {
        mat4x4 matrix;
        matrix.m[0][0] = cosf(fAngleRad);
        matrix.m[0][2] = sinf(fAngleRad);
        matrix.m[2][0] = -sinf(fAngleRad);
        matrix.m[1][1] = 1.0f;
        matrix.m[2][2] = cosf(fAngleRad);
        matrix.m[3][3] = 1.0f;
        return matrix;
    }

    mat4x4 Matrix_MakeRotationZ(float fAngleRad) {
        mat4x4 matrix;
        matrix.m[0][0] = cosf(fAngleRad);
        matrix.m[0][1] = sinf(fAngleRad);
        matrix.m[1][0] = -sinf(fAngleRad);
        matrix.m[1][1] = cosf(fAngleRad);
        matrix.m[2][2] = 1.0f;
        matrix.m[3][3] = 1.0f;
        return matrix;
    }

    mat4x4 Matrix_MakeTranslation(float x, float y, float z) {
        mat4x4 matrix;
        matrix.m[0][0] = 1.0f;
        matrix.m[1][1] = 1.0f;
        matrix.m[2][2] = 1.0f;
        matrix.m[3][3] = 1.0f;
        matrix.m[3][0] = x;
        matrix.m[3][1] = y;
        matrix.m[3][2] = z;
        return matrix;
    }

    mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar) {
        float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
        mat4x4 matrix;
        matrix.m[0][0] = fAspectRatio * fFovRad;
        matrix.m[1][1] = fFovRad;
        matrix.m[2][2] = fFar / (fFar - fNear);
        matrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
        matrix.m[2][3] = 1.0f;
        matrix.m[3][3] = 0.0f;
        return matrix;
    }

    mat4x4 Matrix_MultiplyMatrix(mat4x4 &m1, mat4x4 &m2) {
        mat4x4 matrix;
        for (int c = 0; c < 4; c++)
            for (int r = 0; r < 4; r++)
                matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
        return matrix;
    }

    mat4x4 Matrix_AddMatrix(mat4x4 &m1, mat4x4 &m2) {
        mat4x4 matrix;
        for (int c = 0; c < 4; c++)
            for (int r = 0; r < 4; r++)
                matrix.m[r][c] = m1.m[r][c] + m2.m[r][c];
        return matrix;
    }

    mat4x4 Matrix_SubMatrix(mat4x4 &m1, mat4x4 &m2) {
        mat4x4 matrix;
        for (int c = 0; c < 4; c++)
            for (int r = 0; r < 4; r++)
                matrix.m[r][c] = m1.m[r][c] - m2.m[r][c];
        return matrix;
    }

    mat4x4 Matrix_PointAt(vec3d &pos, vec3d &target, vec3d &up) {
        // Calculate new forward direction
        vec3d newForward = target - pos;
        newForward = Vector_Normalise(newForward);

        // Calculate new Up direction
        vec3d a = newForward * Vector_DotProduct(up, newForward);
        vec3d newUp = up - a;
        newUp = Vector_Normalise(newUp);

        // New Right direction is easy, its just cross product
        vec3d newRight = Vector_CrossProduct(newUp, newForward);

        // Construct Dimensioning and Translation Matrix	
        mat4x4 matrix;
        matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
        matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
        matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
        matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0f;
        return matrix;

    }
    
    // Only for Rotation/Translation Matrices
    mat4x4 Matrix_QuickInverse(mat4x4 &m) {
        mat4x4 matrix;
        matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
        matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
        matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
        matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
        matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
        matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
        matrix.m[3][3] = 1.0f;
        return matrix;
    }

    float Vector_DotProduct(vec3d &v1, vec3d &v2) { return v1.x*v2.x + v1.y*v2.y + v1.z * v2.z;}

    float Vector_distance(vec3d& v1, vec3d& v2) {
        float dx = v2.x - v1.x;
        float dy = v2.y - v1.y;
        float dz = v2.z - v1.z;
        //PQ = d = √ [(x2 – x1)² + (y2 – y1)² + (z2 – z1)²]
        return sqrt(dx * dx + dy * dy + dz * dz);
    }

    float Vector_Length(vec3d &v)
    {
        return sqrtf(Vector_DotProduct(v, v));
    }

    vec3d Vector_Normalise(vec3d &v) { float l = Vector_Length(v); return { v.x / l, v.y / l, v.z / l }; }

    vec3d Vector_CrossProduct(vec3d &v1, vec3d &v2) {
        vec3d v;
        v.x = v1.y * v2.z - v1.z * v2.y;
        v.y = v1.z * v2.x - v1.x * v2.z;
        v.z = v1.x * v2.y - v1.y * v2.x;
        return v;
    }

    vec3d Vector_IntersectPlane(vec3d &plane_p, vec3d &plane_n, vec3d &lineStart, vec3d &lineEnd, float &t) {
        plane_n = (plane_n);
        float plane_d = -Vector_DotProduct(plane_n, plane_p);
        float ad = Vector_DotProduct(lineStart, plane_n);
        float bd = Vector_DotProduct(lineEnd, plane_n);
        t = (-plane_d - ad) / (bd - ad);
        vec3d lineStartToEnd = lineEnd - lineStart;
        vec3d lineToIntersect = lineStartToEnd * t;
        return lineStart + lineToIntersect;
    }

    vec3d triangleCenterPoint(triangle t) {
        vec3d centroid;
        centroid.x = (t.p[0].x + t.p[1].x + t.p[2].x) / 3.0f;
        centroid.y = (t.p[0].y + t.p[1].y + t.p[2].y) / 3.0f;
        centroid.z = (t.p[0].z + t.p[1].z + t.p[2].z) / 3.0f;
        return centroid;
    }

    vec3d getNormal(triangle t) {
        vec3d line1 = t.p[1] - t.p[0];
        vec3d line2 = t.p[2] - t.p[0];
        vec3d normal = Vector_CrossProduct(line1, line2);
        return Vector_Normalise(normal);
    }

    vec3d getPointFromNormal(vec3d startPoint, vec3d normalVector, float distance) {
        // Normaliza o vetor normal
        vec3d normalizedNormal = Vector_Normalise(normalVector);

        // Calcula o novo ponto deslocando o ponto inicial na direção da normal pelo valor da distância
        vec3d point = normalizedNormal * distance;
        vec3d newPoint = startPoint + point;

        return newPoint;
    }

    void Line_ClipAgainstPlane(vec3d& pointA, vec3d& pointB, vec3d planePoint, vec3d planeNormal) {
        vec3d direction = pointB - pointA;

        vec3d line1 = pointA - planePoint;
        vec3d line2 = pointB - planePoint;
        float dotA = Vector_DotProduct(planeNormal, line1);
        float dotB = Vector_DotProduct(planeNormal, line2);

        // Se ambos os pontos estiverem do mesmo lado do plano, a linha está completamente fora
        if (dotA >= 0 && dotB >= 0) {
            // A linha está completamente fora do plano, descarte-a
            // Aqui você pode optar por descartar a linha ou manipular de acordo com sua necessidade
            // Se for descartar, você pode sair da função aqui ou fazer outras operações
            return;
        }

        // Se ambos os pontos estiverem do lado oposto do plano, a linha está completamente dentro, nada a fazer
        if (dotA < 0 && dotB < 0) {
            // A linha está completamente dentro do plano, não há necessidade de clip, mantenha-a como está
            return;
        }

        // A linha cruza o plano, então precisamos fazer o recorte
        float t = dotA / (dotA - dotB);
        vec3d d = direction * t;
        vec3d intersectionPoint = pointA + d;

        if (dotA < 0) {
            // Point A está fora do plano, então substituímos A pelo ponto de interseção
            pointA = intersectionPoint;
        } else {
            // Point B está fora do plano, então substituímos B pelo ponto de interseção
            pointB = intersectionPoint;
        }
    }


    int Triangle_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle &in_tri, triangle &out_tri1, triangle &out_tri2) {
        // Make sure plane normal is indeed normal
        plane_n = Vector_Normalise(plane_n);

        // Return signed shortest distance from point to plane, plane normal must be normalised
        auto dist = [&](vec3d &p)
        {
            // vec3d n = Vector_Normalise(p);
            return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - Vector_DotProduct(plane_n, plane_p));
        };

        // Create two temporary storage arrays to classify points either side of plane
        // If distance sign is positive, point lies on "inside" of plane
        vec3d* inside_points[3];  int nInsidePointCount = 0;
        vec3d* outside_points[3]; int nOutsidePointCount = 0;
        vec2d* inside_tex[3]; int nInsideTexCount = 0;
        vec2d* outside_tex[3]; int nOutsideTexCount = 0;


        // Get signed distance of each point in triangle to plane
        float d0 = dist(in_tri.p[0]);
        float d1 = dist(in_tri.p[1]);
        float d2 = dist(in_tri.p[2]);

        if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[0]; inside_tex[nInsideTexCount++] = &in_tri.t[0]; }
        else {
            outside_points[nOutsidePointCount++] = &in_tri.p[0]; outside_tex[nOutsideTexCount++] = &in_tri.t[0];
        }
        if (d1 >= 0) {
            inside_points[nInsidePointCount++] = &in_tri.p[1]; inside_tex[nInsideTexCount++] = &in_tri.t[1];
        }
        else {
            outside_points[nOutsidePointCount++] = &in_tri.p[1];  outside_tex[nOutsideTexCount++] = &in_tri.t[1];
        }
        if (d2 >= 0) {
            inside_points[nInsidePointCount++] = &in_tri.p[2]; inside_tex[nInsideTexCount++] = &in_tri.t[2];
        }
        else {
            outside_points[nOutsidePointCount++] = &in_tri.p[2];  outside_tex[nOutsideTexCount++] = &in_tri.t[2];
        }

        // Now classify triangle points, and break the input triangle into 
        // smaller output triangles if required. There are four possible
        // outcomes...

        if (nInsidePointCount == 0)
        {
            // All points lie on the outside of plane, so clip whole triangle
            // It ceases to exist

            return 0; // No returned triangles are valid
        }

        if (nInsidePointCount == 3)
        {
            // All points lie on the inside of plane, so do nothing
            // and allow the triangle to simply pass through
            out_tri1 = in_tri;

            return 1; // Just the one returned original triangle is valid
        }

        if (nInsidePointCount == 1 && nOutsidePointCount == 2)
        {
            // triangle should be clipped. As two points lie outside
            // the plane, the triangle simply becomes a smaller triangle

            // Copy appearance info to new triangle
            out_tri1.texture =  in_tri.texture;
            out_tri1.face = in_tri.face;

            // The inside point is valid, so keep that...
            out_tri1.p[0] = *inside_points[0];
            out_tri1.t[0] = *inside_tex[0];

            // but the two new points are at the locations where the 
            // original sides of the triangle (lines) intersect with the plane
            float t;
            out_tri1.p[1] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
            out_tri1.t[1].u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
            out_tri1.t[1].v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
            out_tri1.t[1].w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

            out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1], t);
            out_tri1.t[2].u = t * (outside_tex[1]->u - inside_tex[0]->u) + inside_tex[0]->u;
            out_tri1.t[2].v = t * (outside_tex[1]->v - inside_tex[0]->v) + inside_tex[0]->v;
            out_tri1.t[2].w = t * (outside_tex[1]->w - inside_tex[0]->w) + inside_tex[0]->w;

            return 1; // Return the newly formed single triangle
        }

        if (nInsidePointCount == 2 && nOutsidePointCount == 1)
        {
            // triangle should be clipped. As two points lie inside the plane,
            // the clipped triangle becomes a "quad". Fortunately, we can
            // represent a quad with two new triangles

            // Copy appearance info to new triangles
            out_tri1.texture =  in_tri.texture; 
            out_tri1.face =  in_tri.face;

            out_tri2.texture =  in_tri.texture; 
            out_tri2.face =  in_tri.face;
            // The first triangle consists of the two inside points and a new
            // point determined by the location where one side of the triangle
            // intersects with the plane
            out_tri1.p[0] = *inside_points[0];
            out_tri1.p[1] = *inside_points[1];
            out_tri1.t[0] = *inside_tex[0];
            out_tri1.t[1] = *inside_tex[1];

            float t;
            out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
            out_tri1.t[2].u = t * (outside_tex[0]->u - inside_tex[0]->u) + inside_tex[0]->u;
            out_tri1.t[2].v = t * (outside_tex[0]->v - inside_tex[0]->v) + inside_tex[0]->v;
            out_tri1.t[2].w = t * (outside_tex[0]->w - inside_tex[0]->w) + inside_tex[0]->w;

            // The second triangle is composed of one of he inside points, a
            // new point determined by the intersection of the other side of the 
            // triangle and the plane, and the newly created point above
            out_tri2.p[0] = *inside_points[1];
            out_tri2.t[0] = *inside_tex[1];
            out_tri2.p[1] = out_tri1.p[2];
            out_tri2.t[1] = out_tri1.t[2];
            out_tri2.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0], t);
            out_tri2.t[2].u = t * (outside_tex[0]->u - inside_tex[1]->u) + inside_tex[1]->u;
            out_tri2.t[2].v = t * (outside_tex[0]->v - inside_tex[1]->v) + inside_tex[1]->v;
            out_tri2.t[2].w = t * (outside_tex[0]->w - inside_tex[1]->w) + inside_tex[1]->w;
            return 2; // Return two newly formed triangles which form a quad
        }
        return 1;
    }
            
    std::vector<vec3d> cubePoints(Faces& faces) {
        std::vector<vec3d> points;

        points.push_back({(0 + faces.east), (0 + faces.bottom), (0 + faces.north), 1});		//[0] 0, 0, 0
        points.push_back({(1 - faces.west), (0 + faces.bottom), (0 + faces.north), 1});		//[1] 1, 0, 0
        points.push_back({(0 + faces.east), (0 + faces.bottom), (1 - faces.south), 1});		//[2] 0, 0, 1
        points.push_back({(1 - faces.west), (0 + faces.bottom), (1 - faces.south), 1});		//[3] 1, 0, 1

        points.push_back({(0 + faces.east), (1 - faces.top), (0 + faces.north), 1});		//[4] 0, 1, 0
        points.push_back({(1 - faces.west), (1 - faces.top), (0 + faces.north), 1});		//[5] 1, 1, 0
        points.push_back({(0 + faces.east), (1 - faces.top), (1 - faces.south), 1});		//[6] 0, 1, 1
        points.push_back({(1 - faces.west), (1 - faces.top), (1 - faces.south), 1});		//[7] 1, 1, 1

        return points;
    }

    uint16_t getIndicePosition(vec3d position) {
        return ( (uint16_t)(position.y * CHUNK_DEPTH) + (uint16_t)(position.x * CHUNK_HEIGHT * CHUNK_DEPTH) + (uint16_t)position.z );
    }

    int getPositionInArray(vec3d position) {
        // x + n * (y + n * z);
        return ( (int)(position.y * CHUNK_WIDTH) + (int)(position.x * CHUNK_WIDTH * CHUNK_WIDTH) + (int)position.z );
    }

    void getUVCoordinates(uint8_t t_indice, uint8_t triangle, int sections, vec2d* t) {
        int row = t_indice % sections;
        int collumn = t_indice / sections;
        
        float step = 1.0f/sections;
        
        float x0 = row * step;
        float y0 = collumn * step;
        
        float x1 = x0 + 0;
        float y1 = y0 + step;
        
        float x2 = x0 + step;
        float y2 = y0 + step;
        
        float x3 = x0 + step;
        float y3 = y0 + 0;

        if (triangle == 0){
            t[0] = {x0, y0, 1};
            t[1] = {x1, y1, 1};
            t[2] = {x2, y2, 1};
        } else if (triangle == 1){
            t[0] = {x0, y0, 1};
            t[1] = {x2, y2, 1};
            t[2] = {x3, y3, 1};
        } else if (triangle == 3){
            t[0] = {x0, y0, 1};
            t[1] = {x1, y1, 1};
            t[2] = {x2, y2, 1};
            t[3] = {x3, y3, 1};
        }
    }

    vec3d getPositionOfIndex(int index) {
        vec3d position;
        position.z = float(index % CHUNK_DEPTH);
        index /= CHUNK_DEPTH;
        position.y = float(index % CHUNK_HEIGHT);
        index /= CHUNK_HEIGHT;
        position.x = float(index);
        return position;
    }

    void translateMesh(vector<triangle>& triangles, vec3d translation) {
        for (auto& triangle : triangles) {
            for (int i = 0; i < 3; ++i) {
                triangle.p[i] += translation;
            }
        }
    }

    void rotateMesh(vector<triangle>& triangles, vec3d position, vec3d rotate, vec3d local_center){
        translateMesh(triangles, -local_center);
        for (auto& triangle : triangles) {
            for (auto& vertex : triangle.p){
                mat4x4 rotX = Matrix_MakeRotationX(rotate.x);
                mat4x4 rotY = Matrix_MakeRotationY(rotate.y);
                mat4x4 rotZ = Matrix_MakeRotationZ(rotate.z);
                vertex = vertex * rotX;
                vertex = vertex * rotY;
                vertex = vertex * rotZ;
            }
        }
        translateMesh(triangles, local_center);
        translateMesh(triangles, position);
    }

    void createMesh(set<triangle>& triangles, vector<Indices>& indices, vector<vec3d> points, vec3d position, vec3d rotate, vec3d local_center){
        vector<triangle> tris;
        for (auto& indice : indices){
            triangle t1;
            t1.p[0] = points[indice.p_indices[0]];
            t1.p[1] = points[indice.p_indices[1]];
            t1.p[2] = points[indice.p_indices[2]];
            t1.culling = true;
            t1.face = indice.t_indice;
            t1.texture = indice.type;
            getUVCoordinates(indice.t_indice, 0, 1, t1.t);
            
            triangle t2;
            t2.p[0] = points[indice.p_indices[0]];
            t2.p[1] = points[indice.p_indices[2]];
            t2.p[2] = points[indice.p_indices[3]];
            t2.culling = true;
            t2.face = indice.t_indice;
            t2.texture = indice.type;
            getUVCoordinates(indice.t_indice, 1, 1, t2.t);
        
            tris.push_back(t1);
            tris.push_back(t2);
        }

        rotateMesh(tris, position, rotate, local_center);

        for (auto& triangle : tris)
            triangles.insert(triangle);

    }

    bool cubeIntersection(const Ray& ray, const vec3d minPoint, const vec3d maxPoint, float& tMin, float& tMax, VoxelData* voxels, int& pos) {
        float tMinX = (minPoint.x - ray.origin.x) / ray.direction.x;
        float tMaxX = (maxPoint.x - ray.origin.x) / ray.direction.x;

        if (tMinX > tMaxX) std::swap(tMinX, tMaxX);

        float tMinY = (minPoint.y - ray.origin.y) / ray.direction.y;
        float tMaxY = (maxPoint.y - ray.origin.y) / ray.direction.y;

        if (tMinY > tMaxY) std::swap(tMinY, tMaxY);

        float tMinZ = (minPoint.z - ray.origin.z) / ray.direction.z;
        float tMaxZ = (maxPoint.z - ray.origin.z) / ray.direction.z;

        if (tMinZ > tMaxZ) std::swap(tMinZ, tMaxZ);

        tMin = std::max({tMinX, tMinY, tMinZ});
        tMax = std::min({tMaxX, tMaxY, tMaxZ});

        if (!(tMin <= tMax))
            return false;

        // return true;

        vec3d intersection = ray.origin + (ray.direction * tMin);

        vec3d intersectionFixed = {
            trunc(intersection.x),
            trunc(intersection.y),
            trunc(intersection.z)
        };

        int indice = getPositionInArray(intersectionFixed);
        if (indice >= 0 && indice < (CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH) && intersectionFixed >= 0.0f){
            if (voxels[indice].type != 0){
                pos = indice;
                return true;
            }
        }

        return false;
    }

    vector<vec3d> generateSphere(vec3d pos, int radius, int size){
        vector<vec3d> points;
        vec3d truncPos = {trunc(pos.x), trunc(pos.y), trunc(pos.z)};

        for (int x = 0; x < size; x++){
            for (int y = 0; y < size; y++){
                for (int z = 0; z < size; z++){
                    int dx = x - size/2;
                    int dy = y - size/2;
                    int dz = z - size/2;

                    vec3d p = {(float)x, (float)y, (float)z};
                    p = (p - (float)size/2) + truncPos;
                    
                    if (dx * dx + dy * dy + dz * dz <= radius * radius)
                        points.push_back(p);
                }
            }
        }

        return points;
    }

    vector<Indices> generateRegularVoxelEntitie(uint8_t type){
        vector<Indices> meshs;
        // TOP
        meshs.push_back({type, 2u, 4u, 6u, 7u, 5u});

        // BOTTOM
        meshs.push_back({type, 0u, 3u, 2u, 0u, 1u});
        
        // NORTH
        meshs.push_back({type, 1u, 3u, 7u, 6u, 2u});

        // SOUTH
        meshs.push_back({type, 1u, 0u, 4u, 5u, 1u});

        // EAST
        meshs.push_back({type, 1u, 1u, 5u, 7u, 3u});

        // WEST
        meshs.push_back({type, 1u, 2u, 6u, 4u, 0u});

        return meshs;
    }

    VoxelData* getVoxelFace(ChunkMap& currChunk, const int x, const int y, const int z, const uint8_t side) {
        uint16_t indice = getIndicePosition({(float)x, (float)y, (float)z});
        VoxelData* voxelFace = &currChunk.voxels[indice];
        voxelFace->side = side;
        return voxelFace;
    }

    void quad( 
                    vec3d bottomLeft, 
                    vec3d topLeft, 
                    vec3d topRight, 
                    vec3d bottomRight,
                    VoxelData voxel,
                    bool backFace, set<triangle>& tris) {
        
        int t_indices1[3];
        int t_indices2[3];
        vec2d uv1 = {1, 1, 1};
        vec2d uv2 = {1, 1, 1};

        vec3d vertices[4];

        vertices[2] = topLeft * 1;
        vertices[3] = topRight * 1;
        vertices[0] = bottomLeft * 1;
        vertices[1] = bottomRight * 1;

        uint8_t face = 1;

        if (voxel.side == _NORTH){
            t_indices1[0] = 0;
            t_indices1[1] = 1;
            t_indices1[2] = 2;
            uv1.u = floor(Vector_distance(vertices[0], vertices[2])*2);
            uv1.v = floor(Vector_distance(vertices[0], vertices[1])*2);

            t_indices2[0] = 1;
            t_indices2[1] = 2;
            t_indices2[2] = 0;
            uv2.u = floor(Vector_distance(vertices[0], vertices[2])*2);
            uv2.v = floor(Vector_distance(vertices[0], vertices[1])*2);
        }

        if (voxel.side == _SOUTH){
            t_indices1[0] = 0;
            t_indices1[1] = 2;
            t_indices1[2] = 1;
            uv1.u = floor(Vector_distance(vertices[0], vertices[2])*2);
            uv1.v = floor(Vector_distance(vertices[0], vertices[1])*2);

            t_indices2[0] = 2;
            t_indices2[1] = 1;
            t_indices2[2] = 0;
            uv2.u = floor(Vector_distance(vertices[0], vertices[2])*2);
            uv2.v = floor(Vector_distance(vertices[0], vertices[1])*2);
        }

        if (voxel.side == _WEST){
            t_indices1[0] = 1;
            t_indices1[1] = 2;
            t_indices1[2] = 0;
            uv1.u = floor(Vector_distance(vertices[0], vertices[1])*2);
            uv1.v = floor(Vector_distance(vertices[0], vertices[2])*2);

            t_indices2[0] = 0;
            t_indices2[1] = 1;
            t_indices2[2] = 2;
            uv2.u = floor(Vector_distance(vertices[0], vertices[1])*2);
            uv2.v = floor(Vector_distance(vertices[0], vertices[2])*2);
        }

        if (voxel.side == _EAST){
            t_indices1[0] = 2;
            t_indices1[1] = 1;
            t_indices1[2] = 0;
            uv1.u = floor(Vector_distance(vertices[0], vertices[1])*2);
            uv1.v = floor(Vector_distance(vertices[0], vertices[2])*2);

            t_indices2[0] = 0;
            t_indices2[1] = 2;
            t_indices2[2] = 1;
            uv2.u = floor(Vector_distance(vertices[0], vertices[1])*2);
            uv2.v = floor(Vector_distance(vertices[0], vertices[2])*2);
        }

        if (voxel.side == _TOP){
            t_indices1[0] = 0;
            t_indices1[1] = 2;
            t_indices1[2] = 1;
            uv1.u = floor(Vector_distance(vertices[0], vertices[1])*2);
            uv1.v = floor(Vector_distance(vertices[0], vertices[2])*2);


            t_indices2[0] = 2;
            t_indices2[1] = 1;
            t_indices2[2] = 0;
            uv2.u = floor(Vector_distance(vertices[0], vertices[1])*2);
            uv2.v = floor(Vector_distance(vertices[0], vertices[2])*2);

            face = 2;
        }

        if (voxel.side == _BOTTOM){
            t_indices1[0] = 0;
            t_indices1[1] = 2;
            t_indices1[2] = 1;
            uv1.u = floor(Vector_distance(vertices[0], vertices[2])*2);
            uv1.v = floor(Vector_distance(vertices[0], vertices[1])*2);

            t_indices2[0] = 2;
            t_indices2[1] = 1;
            t_indices2[2] = 0;
            uv2.u = floor(Vector_distance(vertices[0], vertices[2])*2);
            uv2.v = floor(Vector_distance(vertices[0], vertices[1])*2);

            face = 0;
        }

        vec2d t_t1[3];
        getUVCoordinates(1, 1, 2, t_t1);
        vec2d t_t2[3];
        getUVCoordinates(1, 0, 2, t_t2);

        // int* indexes = backFace ? new int[6]{2,0,1, 1,3,2} : new int[6]{ 2,3,1, 1,0,2 };
        
        triangle t1;
        if (!backFace){
            t1.p[0] = vertices[2];
            t1.p[1] = vertices[3];
            t1.p[2] = vertices[1];
        }else{
            t1.p[0] = vertices[2];
            t1.p[1] = vertices[0];
            t1.p[2] = vertices[1];
        }
        t1.texture = voxel.type;
        t1.face = face;
        t1.culling = true;
        
        if (voxel.side == _EAST || voxel.side == _NORTH){
            t1.t[0] = t_t2[t_indices1[0]]; 
            t1.t[1] = t_t2[t_indices1[1]]; 
            t1.t[2] = t_t2[t_indices1[2]];
        }else{
            t1.t[0] = t_t1[t_indices1[0]]; 
            t1.t[1] = t_t1[t_indices1[1]]; 
            t1.t[2] = t_t1[t_indices1[2]];
        }
        

        t1.t[0] *= uv1;
        t1.t[1] *= uv1;
        t1.t[2] *= uv1;
        auto triInsert1 = tris.insert(t1);
        if ( !triInsert1.second ) {
            auto it = tris.find(t1);
            if (it != tris.end())
                tris.erase(it);
        }
        
        triangle t2;
        if (!backFace){
            t2.p[0] = vertices[1];
            t2.p[1] = vertices[0];
            t2.p[2] = vertices[2];
        }else{
            t2.p[0] = vertices[1];
            t2.p[1] = vertices[3];
            t2.p[2] = vertices[2];
        }

        t2.texture = voxel.type;
        t2.face = face;
        t2.culling = true;
        
        if (voxel.side == _EAST || voxel.side == _NORTH){
            t2.t[0] = t_t1[t_indices2[0]]; 
            t2.t[1] = t_t1[t_indices2[1]]; 
            t2.t[2] = t_t1[t_indices2[2]];
        }else{
            t2.t[0] = t_t2[t_indices2[0]]; 
            t2.t[1] = t_t2[t_indices2[1]]; 
            t2.t[2] = t_t2[t_indices2[2]];
        }

        t2.t[0] *= uv2;
        t2.t[1] *= uv2;
        t2.t[2] *= uv2;
        auto trisInsert2 = tris.insert(t2);
        if ( !trisInsert2.second ) {
            auto it = tris.find(t2);
            if (it != tris.end())
                tris.erase(it);
        }
        
    }

    void greedy(ChunkMap& currChunk, set<triangle>& tris) {

        /*
            * These are just working variables for the algorithm - almost all taken 
            * directly from Mikola Lysenko's javascript implementation.
            */
        int i = 0, j = 0, k = 0, l = 0, w = 0, h = 0, u = 0, v = 0, n = 0, side = 0;
        
        int  x[3] = {0,0,0};
        int  q[3] = {0,0,0};
        int du[3] = {0,0,0}; 
        int dv[3] = {0,0,0};         
        
        /*
            * We create a mask - this will contain the groups of matching voxel faces 
            * as we proceed through the chunk in 6 directions - once for each face.
            */
        VoxelData* mask[CHUNK_WIDTH * CHUNK_WIDTH] = {nullptr};
        
        /*
            * These are just working variables to hold two faces during comparison.
            */
        VoxelData* voxelFace;
        VoxelData* voxelFace1;

        /**
         * We start with the lesser-spotted boolean for-loop (also known as the old flippy floppy). 
         * 
         * The variable backFace will be TRUE on the first iteration and FALSE on the second - this allows 
         * us to track which direction the indices should run during creation of the quad.
         * 
         * This loop runs twice, and the inner loop 3 times - totally 6 iterations - one for each 
         * voxel face.
         */
        for (bool backFace = true, b = false; b != backFace; backFace = backFace && b, b = !b) { 

            /*
                * We sweep over the 3 dimensions - most of what follows is well described by Mikola Lysenko 
                * in his post - and is ported from his Javascript implementation.  Where this implementation 
                * diverges, I've added commentary.
                */
            for(int d = 0; d < 3; d++) {

                u = (d + 1) % 3; 
                v = (d + 2) % 3;

                x[0] = 0;
                x[1] = 0;
                x[2] = 0;

                q[0] = 0;
                q[1] = 0;
                q[2] = 0;
                q[d] = 1;

                /*
                    * Here we're keeping track of the side that we're meshing.
                    */
                if (d == 0)      { side = backFace ? _WEST   : _EAST;  }
                else if (d == 1) { side = backFace ? _BOTTOM : _TOP;   }
                else if (d == 2) { side = backFace ? _SOUTH  : _NORTH; }                

                /*
                    * We move through the dimension from front to back
                    */            
                for(x[d] = -1; x[d] < CHUNK_WIDTH;) {

                    /*
                        * -------------------------------------------------------------------
                        *   We compute the mask
                        * -------------------------------------------------------------------
                        */
                    n = 0;

                    for(x[v] = 0; x[v] < CHUNK_WIDTH; x[v]++) {

                        for(x[u] = 0; x[u] < CHUNK_WIDTH; x[u]++) {

                            /*
                                * Here we retrieve two voxel faces for comparison.
                                */
                            voxelFace  = (x[d] >= 0 )             ? getVoxelFace(currChunk, x[0], x[1], x[2], side)                      : nullptr;
                            voxelFace1 = (x[d] < CHUNK_WIDTH - 1) ? getVoxelFace(currChunk, x[0] + q[0], x[1] + q[1], x[2] + q[2], side) : nullptr;

                            /*
                                * Note that we're using the equals function in the voxel face class here, which lets the faces 
                                * be compared based on any number of attributes.
                                * 
                                * Also, we choose the face to add to the mask depending on whether we're moving through on a backface or not.
                                */
                            mask[n++] = ((voxelFace != nullptr && voxelFace1 != nullptr && *voxelFace == *voxelFace1)) 
                                        ? nullptr 
                                        : backFace ? voxelFace1 : voxelFace;
                        }
                    }

                    x[d]++;

                    /*
                        * Now we generate the mesh for the mask
                        */
                    n = 0;

                    for(j = 0; j < CHUNK_WIDTH; j++) {

                        for(i = 0; i < CHUNK_WIDTH;) {

                            if(mask[n] != nullptr) {

                                /*
                                    * We compute the width
                                    */
                                for(w = 1; (i + w < CHUNK_WIDTH && mask[n + w] != nullptr) && *mask[n + w] == *mask[n]; w++) {}

                                /*
                                    * Then we compute height
                                    */
                                bool done = false;

                                for(h = 1; j + h < CHUNK_WIDTH; h++) {

                                    for(k = 0; k < w; k++) {

                                        if(mask[n + k + h * CHUNK_WIDTH] == nullptr || !(*mask[n + k + h * CHUNK_WIDTH] == *mask[n])) { done = true; break; }
                                    }

                                    if(done) { break; }
                                }

                                /*
                                    * Here we check the "transparent" attribute in the VoxelFace class to ensure that we don't mesh 
                                    * any culled faces.
                                    */
                                if (!(mask[n]->type == 0)) {
                                    /*
                                        * Add quad
                                        */
                                    x[u] = i;  
                                    x[v] = j;

                                    du[0] = 0;
                                    du[1] = 0;
                                    du[2] = 0;
                                    du[u] = w;

                                    dv[0] = 0;
                                    dv[1] = 0;
                                    dv[2] = 0;
                                    dv[v] = h;

                                    /*
                                        * And here we call the quad function in order to render a merged quad in the scene.
                                        * 
                                        * We pass mask[n] to the function, which is an instance of the VoxelFace class containing 
                                        * all the attributes of the face - which allows for variables to be passed to shaders - for 
                                        * example lighting values used to create ambient occlusion.
                                        */
                                    quad({(float)(x[0]					), (float)(                x[1]), (float)(                  x[2])}, 
                                            {(float)(x[0] + du[0]			), (float)(        x[1] + du[1]), (float)(          x[2] + du[2])}, 
                                            {(float)(x[0] + du[0] + dv[0]	), (float)(x[1] + du[1] + dv[1]), (float)(  x[2] + du[2] + dv[2])}, 
                                            {(float)(x[0] + dv[0]			), (float)(        x[1] + dv[1]), (float)(          x[2] + dv[2])}, 
                                            *mask[n],
                                            backFace, tris);
                                }

                                /*
                                    * We zero out the mask
                                    */
                                for(l = 0; l < h; ++l) {

                                    for(k = 0; k < w; ++k) { mask[n + k + l * CHUNK_WIDTH] = nullptr; }
                                }

                                /*
                                    * And then finally increment the counters and continue
                                    */
                                i += w; 
                                n += w;

                            } else {

                                i++;
                                n++;
                            }
                        }
                    } 
                }
            }        
        }
    }

    void initVoxels(ChunkMap& currChunk){

        for (int x = 0; x < CHUNK_WIDTH; x++) {
            
            for (int y = 0; y < CHUNK_WIDTH; y++) {
            
                for (int z = 0; z < CHUNK_WIDTH; z++) {
                    VoxelData* face = new VoxelData();
                    face->type = 2;

                    if (y == CHUNK_WIDTH - 1)
                        face->type = 1;

                    currChunk.voxels.push_back(*face);
                }
            }            
        }
    }
};