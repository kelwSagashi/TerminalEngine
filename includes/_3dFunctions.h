#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <cwchar>
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <array>
#include <set>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <vector>
#include <strstream>
#include <fstream>

#define maxAB(a, b) ((a) > (b) ? (a) : (b))

using namespace std;

namespace Math3d
{
    constexpr int CHUNK_WIDTH =  16;
    constexpr int CHUNK_HEIGHT = 16;
    constexpr int CHUNK_DEPTH =  16;
    constexpr uint8_t _SOUTH  = 0;
    constexpr uint8_t _NORTH  = 1;
    constexpr uint8_t _EAST 	 = 2;
    constexpr uint8_t _WEST 	 = 3;
    constexpr uint8_t _TOP 	 = 4;
    constexpr uint8_t _BOTTOM = 5;

	struct Faces{
		float top;
		float bottom; 
		float east; 
		float west;
		float north; 
		float south;

		Faces(float value) : top(value), bottom(value), east(value), west(value), north(value), south(value){}
	};

    struct vec2d
    {
        float u = 0;
        float v = 0;
        float w = 1;

		vec2d operator*(const vec2d& other) const {return {u * other.u, v * other.v, w}; }
		vec2d& operator*=(const vec2d& other)  {
			u *= other.u;
			v *= other.v;
			return *this;
		}
    };

	struct vec2
	{
		float x, y;
		vec2(float value) : x(value), y(value){}
		vec2(float _x, float _y) : x(_x), y(_y){}

		vec2 operator+(vec2 const& other){return vec2(x + other.x, y + other.y);}
		vec2 operator-(vec2 const& other){return vec2(x - other.x, y - other.y);}
		vec2 operator*(vec2 const& other){return vec2(x * other.x, y * other.y);}
		vec2 operator/(vec2 const& other){return vec2(x / other.x, y / other.y);}
		vec2 operator-() {return vec2(-x, -y);}
};
	
	struct mat4x4
    {
        float m[4][4] = { 0 };
    };

    struct vec3d
    {
        float x = 0;
        float y = 0;
        float z = 0;
        float w = 1; // Need a 4th term to perform sensible matrix vector multiplication

		vec3d operator-(const vec3d& other) const {return {x - other.x, y - other.y, z - other.z, w}; }
		vec3d operator+(const vec3d& other) const {return {x + other.x, y + other.y, z + other.z, w}; }
		vec3d operator*(const vec3d& other) const {return {x * other.x, y * other.y, z * other.z, w}; }
		vec3d operator*(const float& other) const {return {x * other, y * other, z * other, w}; }
		vec3d operator+(const float& other) const {return {x + other, y + other, z + other, w}; }
		vec3d operator-(const float& other) const {return {x - other, y - other, z - other, w}; }
		vec3d operator/(const vec3d& other) const {return {x / other.x, y / other.y, z / other.z, w}; }
		vec3d operator/(const float& other) const {return {x / other, y / other, z / other, w}; }
		vec3d operator-() const { return {-x, -y, -z, w}; }

		bool operator==(const vec3d& other) const {return tie(x, y, z) == tie(other.x, other.y, other.z);}
		bool operator<(const vec3d& other)  const {return tie(x, y, z) <  tie(other.x, other.y, other.z);}
		bool operator>=(const vec3d& other) const {return tie(x, y, z) >= tie(other.x, other.y, other.z);}
		bool operator<=(const vec3d& other) const {return tie(x, y, z) <= tie(other.x, other.y, other.z);}
		bool operator!=(const vec3d& other) const {return tie(x, y, z) != tie(other.x, other.y, other.z);}

		bool operator>=(const float& other) const { return x >= other && y >= other && z >= other; }
		
		vec3d& operator+=(const vec3d& other) {
			x += other.x; 
			y += other.y; 
			z += other.z;
			return *this;
		}

		vec3d& operator-=(const vec3d& other) {
			x -= other.x; 
			y -= other.y; 
			z -= other.z;
			return *this;
		}

		vec3d& operator*=(const vec3d& other) {
			x *= other.x; 
			y *= other.y; 
			z *= other.z;
			return *this;
		}

		vec3d& operator/=(const vec3d& other) {
			x /= other.x; 
			y /= other.y; 
			z /= other.z;
			return *this;
		}

		vec3d operator*(const mat4x4& m) const { return {
				x * m.m[0][0] + y * m.m[1][0] + z * m.m[2][0] + w * m.m[3][0],
				x * m.m[0][1] + y * m.m[1][1] + z * m.m[2][1] + w * m.m[3][1],
				x * m.m[0][2] + y * m.m[1][2] + z * m.m[2][2] + w * m.m[3][2],
				x * m.m[0][3] + y * m.m[1][3] + z * m.m[2][3] + w * m.m[3][3]
			};
		}
    };

	struct Ray {
		vec3d origin;
		vec3d direction;
	};

	struct Line{
		vec3d start;
		vec3d end;
	};

    struct triangle
    {
        vec3d p[3];
        vec2d t[3]; // added a texture coord per vertex 
		uint8_t texture;
		uint8_t face;
		bool culling;

		bool operator==(const triangle& other) const {
			float x = (p[0].x + p[1].x + p[2].x) / 3.0f;
			float y = (p[0].y + p[1].y + p[2].y) / 3.0f;
			float z = (p[0].z + p[1].z + p[2].z) / 3.0f;

			float other_x = (other.p[0].x + other.p[1].x + other.p[2].x) / 3.0f;
			float other_y = (other.p[0].y + other.p[1].y + other.p[2].y) / 3.0f;
			float other_z = (other.p[0].z + other.p[1].z + other.p[2].z) / 3.0f;

			vec3d center = {x, y, z, 1};
			vec3d other_center = {other_x, other_y, other_z, 1};

			return center == other_center;
		}

		bool operator<(const triangle& other) const {
			float x = (p[0].x + p[1].x + p[2].x) / 3.0f;
			float y = (p[0].y + p[1].y + p[2].y) / 3.0f;
			float z = (p[0].z + p[1].z + p[2].z) / 3.0f;

			float other_x = (other.p[0].x + other.p[1].x + other.p[2].x) / 3.0f;
			float other_y = (other.p[0].y + other.p[1].y + other.p[2].y) / 3.0f;
			float other_z = (other.p[0].z + other.p[1].z + other.p[2].z) / 3.0f;

			vec3d center = {x, y, z, 1};
			vec3d other_center = {other_x, other_y, other_z, 1};

			return center < other_center;
		}
    };

	struct VoxelData{
		uint8_t type;
		uint8_t side;

		bool operator==(const VoxelData& other) const {
			return type == other.type;
		}	
	};

	struct ChunkMap{
		vector<VoxelData> voxels;
	};

	struct Indices{
		uint8_t type;
		uint8_t t_indice;
		uint8_t p_indices[4];
	}; 

	struct MeshObj
	{
		set<triangle> tris;

		bool LoadFromObjectFile(string sFilename, bool bHasTexture = false)
		{
			ifstream f(sFilename);
			if (!f.is_open())
				return false;

			// Local cache of verts
			vector<vec3d> verts;
			vector<vec2d> texs;

			while (!f.eof())
			{
				char line[128];
				f.getline(line, 128);

				strstream s;
				s << line;

				char junk;

				if (line[0] == 'v')
				{
					if (line[1] == 't')
					{
						vec2d v;
						s >> junk >> junk >> v.u >> v.v;
						// A little hack for the spyro texture
						//v.u = 1.0f - v.u;
						//v.v = 1.0f - v.v;
						texs.push_back(v);
					}
					else
					{
						vec3d v;
						s >> junk >> v.x >> v.y >> v.z;
						verts.push_back(v);
					}
				}

				if (!bHasTexture)
				{
					if (line[0] == 'f')
					{
						int f[3];
						s >> junk >> f[0] >> f[1] >> f[2];
						triangle t = { verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] };
						t.culling = true;
						t.face = 1U;
						t.texture = 1U;
						tris.insert(t);
					}
				}
				else
				{
					if (line[0] == 'f')
					{
						s >> junk;

						string tokens[6];
						int nTokenCount = -1;


						while (!s.eof())
						{
							char c = s.get();
							if (c == ' ' || c == '/')
								nTokenCount++;
							else
								tokens[nTokenCount].append(1, c);
						}

						tokens[nTokenCount].pop_back();

						triangle t = { verts[stoi(tokens[0]) - 1], verts[stoi(tokens[2]) - 1], verts[stoi(tokens[4]) - 1],
							texs[stoi(tokens[1]) - 1], texs[stoi(tokens[3]) - 1], texs[stoi(tokens[5]) - 1] };
						t.culling = true;
						t.face = 1U;
						t.texture = 1U;
						tris.insert(t);

					}

				}
			}
			return true;
		}
	};

    float clamp1(float valor, float min, float max);

	vec2d repeatSample(const vec2d v, const vec2d min, const vec2d max);

	float map(float value, float fromMinValue, float fromMaxValue, float toMinValue, float toMaxValue);

	float radians(float degrees);

    float Vector_Magnitude(const vec3d& vec);

    mat4x4 Matrix_MakeIdentity();

    mat4x4 Matrix_Scale(vec3d& v);

    mat4x4 Matrix_MakeRotationX(float fAngleRad);

	mat4x4 Matrix_MakeRotationY(float fAngleRad);

    mat4x4 Matrix_MakeRotationZ(float fAngleRad);

    mat4x4 Matrix_MakeTranslation(float x, float y, float z);

    mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);

    mat4x4 Matrix_MultiplyMatrix(mat4x4 &m1, mat4x4 &m2);

    mat4x4 Matrix_AddMatrix(mat4x4 &m1, mat4x4 &m2);

    mat4x4 Matrix_SubMatrix(mat4x4 &m1, mat4x4 &m2);

    mat4x4 Matrix_PointAt(vec3d &pos, vec3d &target, vec3d &up);

    mat4x4 Matrix_QuickInverse(mat4x4 &m);

    float Vector_DotProduct(vec3d &v1, vec3d &v2);

	float Vector_distance(vec3d& v1, vec3d& v2);

    float Vector_Length(vec3d &v);

    vec3d Vector_Normalise(vec3d &v);

    vec3d Vector_CrossProduct(vec3d &v1, vec3d &v2);

    vec3d Vector_IntersectPlane(vec3d &plane_p, vec3d &plane_n, vec3d &lineStart, vec3d &lineEnd, float &t);

    vec3d triangleCenterPoint(triangle t);

    vec3d getNormal(triangle t);

    vec3d getPointFromNormal(vec3d startPoint, vec3d normalVector, float distance);

    void Line_ClipAgainstPlane(vec3d& pointA, vec3d& pointB, vec3d planePoint, vec3d planeNormal);

    int Triangle_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle &in_tri, triangle &out_tri1, triangle &out_tri2);

    bool loadObj(const std::string& filename, std::vector<triangle>& triangles);

    void SaveTrianglesToFile(std::vector<triangle>& triangles, std::string& filename);

    vector<vec3d> cubePoints(Faces& faces);

    uint16_t getIndicePosition(vec3d position);

    int getPositionInArray(vec3d position);

    void getUVCoordinates(uint8_t t_indice, uint8_t triangle, int sections, vec2d* t);

    vec3d getPositionOfIndex(int index);

    void translateMesh(vector<triangle>& triangles, vec3d translation);

    void rotateMesh(vector<triangle>& triangles, vec3d position, vec3d rotate, vec3d local_center);

    void createMesh(set<triangle>& triangles, vector<Indices>& indices, vector<vec3d> points, vec3d position, vec3d rotate, vec3d local_center);
    
    bool cubeIntersection(const Ray& ray, const vec3d minPoint, const vec3d maxPoint, float& tMin, float& tMax, VoxelData* voxels, int& pos);

    vector<vec3d> generateSphere(vec3d pos, int radius, int size);

    vector<Indices> generateRegularVoxelEntitie(uint8_t type);

    VoxelData* getVoxelFace(ChunkMap& currChunk, const int x, const int y, const int z, const uint8_t side);
    void quad( 
    vec3d bottomLeft, 
    vec3d topLeft, 
    vec3d topRight, 
    vec3d bottomRight,
    VoxelData voxel,
    bool backFace, set<triangle>& tris);

    void greedy(ChunkMap& currChunk, set<triangle>& tris);

	void initVoxels(ChunkMap& currChunk);
};