#include "../includes/Renderer.h"

void Renderer::ClipLine(vec2& A, vec2& B){
	int xMax = ScreenWidth() - 1;
	int xMin = 0;
	int yMax = ScreenHeight() - 1;
	int yMin = 0;
	vec2 C = {A.x, B.y};
	
	vec2 D = vec2(xMax, B.y);
	float AC = sqrt(((C.x - A.x) * (C.x - A.x)) + ((C.y - A.y) * (C.y - A.y)));
	float CB = sqrt(((B.x - C.x) * (B.x - C.x)) + ((B.y - C.y) * (B.y - C.y)));
	float DB = sqrt(((B.x - D.x) * (B.x - D.x)) + ((B.y - D.y) * (B.y - D.y)));

	if (B.x > xMax){
		B.x = xMax;
		float DE = ((AC/CB) * DB);
		if (B.y > A.y)
			B.y = C.y - DE;
		if (B.y < A.y)
			B.y = C.y + DE;
	}

	D = vec2(xMin, B.y);
	AC = sqrt(((C.x - A.x) * (C.x - A.x)) + ((C.y - A.y) * (C.y - A.y)));
	CB = sqrt(((B.x - C.x) * (B.x - C.x)) + ((B.y - C.y) * (B.y - C.y)));
	DB = sqrt(((B.x - D.x) * (B.x - D.x)) + ((B.y - D.y) * (B.y - D.y)));

	if (B.x < xMin){
		B.x = xMin;
		float DE = ((AC/CB) * DB);
		if (B.y > A.y)
			B.y = C.y - DE;
		if (B.y < A.y)
			B.y = C.y + DE;
	}

	C = vec2(B.x, A.y);
	D = vec2(B.x, yMax);
	AC = sqrt(((C.x - A.x) * (C.x - A.x)) + ((C.y - A.y) * (C.y - A.y)));
	CB = sqrt(((B.x - C.x) * (B.x - C.x)) + ((B.y - C.y) * (B.y - C.y)));
	DB = sqrt(((D.x - B.x) * (D.x - B.x)) + ((D.y - B.y) * (D.y - B.y)));

	if (B.y > yMax){	
		B.y = yMax;
		float DE = ((AC/CB) * DB);
		if (B.x > A.x)
			B.x = C.x - DE;
		if (B.x < A.x)
			B.x = C.x + DE;
	}

	D = vec2(B.x, yMin);
	AC = sqrt(((C.x - A.x) * (C.x - A.x)) + ((C.y - A.y) * (C.y - A.y)));
	CB = sqrt(((B.x - C.x) * (B.x - C.x)) + ((B.y - C.y) * (B.y - C.y)));
	DB = sqrt(((D.x - B.x) * (D.x - B.x)) + ((D.y - B.y) * (D.y - B.y)));

	if (B.y < yMin){	
		B.y = yMin;
		float DE = ((AC/CB) * DB);
		if (B.x > A.x)
			B.x = C.x - DE;
		if (B.x < A.x)
			B.x = C.x + DE;
	}
}

// Função para desenhar uma linha entre dois pontos no espaço 3D e projetar no espaço 2D
void Renderer::Draw3DLine(char pixel, Math3d::vec3d& pointA, Math3d::vec3d& pointB, Camera& camera) {
// Matriz de Mundo (World Matrix)
    Math3d::mat4x4 matRotZ, matRotX;
	matRotZ = Math3d::Matrix_MakeRotationZ(0);
	matRotX = Math3d::Matrix_MakeRotationX(0);

	Math3d::mat4x4 matTrans;
	matTrans = Math3d::Matrix_MakeTranslation(0.0f, 0.0f, 0.0f);

	Math3d::mat4x4 matWorld;
	matWorld = Math3d::Matrix_MakeIdentity();	// Form World Matrix
	matWorld = Math3d::Matrix_MultiplyMatrix(matRotZ, matRotX); // Transform by rotation
	matWorld = Math3d::Matrix_MultiplyMatrix(matWorld, matTrans); // Transform by translation

	// World Matrix Transform
	Math3d::vec3d pointA_Transformed = pointA * matWorld;
	Math3d::vec3d pointB_Transformed = pointB * matWorld;

	// Matriz de Visão (View Matrix)
	Math3d::mat4x4 matView = camera.view();

	// Transforma os pontos do espaço 3D para o espaço da câmera (View Space)
	Math3d::vec3d viewPointA = pointA_Transformed * matView;
	Math3d::vec3d viewPointB = pointB_Transformed * matView;

	Math3d::Line_ClipAgainstPlane(viewPointA, viewPointB, { 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f });

	// Projeta os pontos do espaço de visão para o espaço de tela (espaço 2D)
	Math3d::vec3d projectedPointA = viewPointA * camera.matProj;
	Math3d::vec3d projectedPointB = viewPointB * camera.matProj;

	// Normaliza os pontos projetados em coordenadas de tela
	if (projectedPointA.w != 0)
		projectedPointA = projectedPointA / projectedPointA.w;
	if (projectedPointB.w != 0)
		projectedPointB = projectedPointB / projectedPointB.w;

	// X/Y are inverted so put them back
	projectedPointA.x *= -1.0f;
	projectedPointB.x *= -1.0f;
	projectedPointA.y *= -1.0f;
	projectedPointB.y *= -1.0f;

	// Offset verts into visible normalised space
	Math3d::vec3d vOffsetView = { 1,1,0 };
	projectedPointA = projectedPointA + vOffsetView;
	projectedPointB = projectedPointB + vOffsetView;

	// Mapeia os pontos para as dimensões da tela
	projectedPointA.x *= 0.5f * ((float)ScreenWidth() - 1);
	projectedPointA.y *= 0.5f * ((float)ScreenHeight() - 1);

	projectedPointB.x *= 0.5f * ((float)ScreenWidth() - 1);
	projectedPointB.y *= 0.5f * ((float)ScreenHeight() - 1);

	// Desenha a linha entre os pontos projetados na tela
	Math3d::Line_ClipAgainstPlane(projectedPointA, projectedPointB, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
	Math3d::Line_ClipAgainstPlane(projectedPointA, projectedPointB, { 0.0f, (float)ScreenHeight() - 1, 0.0f }, { 0.0f, -1.0f, 0.0f });
	Math3d::Line_ClipAgainstPlane(projectedPointA, projectedPointB, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f });
	Math3d::Line_ClipAgainstPlane(projectedPointA, projectedPointB, { (float)ScreenWidth() - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f });

	vec2 A(projectedPointA.x, projectedPointA.y);
	vec2 B(projectedPointB.x, projectedPointB.y);

	ClipLine(A, B);
	ClipLine(B, A);

	bool AisInside = (A.x > 0 && A.x < ScreenWidth() - 1 && A.y > 0 && A.y < ScreenHeight() - 1);
	bool BisInside = (B.x > 0 && B.x < ScreenWidth() - 1 && B.y > 0 && B.y < ScreenHeight() - 1);

	if (AisInside || BisInside){
		DrawLine(pixel, A.x, A.y, B.x, B.y);
	}
}

void Renderer::rasterTriangles(Camera& camera, Math3d::mat4x4 matWorld, vector<Math3d::triangle>& vecTrianglesToRaster, set<Math3d::triangle>& tris){
	for (auto tri : tris)
	{
		if (tri.culling){
			
			Math3d::triangle triProjected, triTransformed, triViewed;

			// World Matrix Transform
			triTransformed.p[0] = tri.p[0] * matWorld;
			triTransformed.p[1] = tri.p[1] * matWorld;
			triTransformed.p[2] = tri.p[2] * matWorld;
			triTransformed.t[0] = tri.t[0];
			triTransformed.t[1] = tri.t[1];
			triTransformed.t[2] = tri.t[2];
			triTransformed.texture = tri.texture;
			triTransformed.face = tri.face;

			// Calculate triangle Normal
			Math3d::vec3d normal, line1, line2;

			// Get lines either side of triangle
			line1 = triTransformed.p[1] - triTransformed.p[0];
			line2 = triTransformed.p[2] - triTransformed.p[0];

			// Take cross product of lines to get normal to triangle surface
			normal = Math3d::Vector_CrossProduct(line1, line2);

			// Math3d::vec3d n = normal;
			// You normally need to normalise a normal!
			normal = Math3d::Vector_Normalise(normal);
			
			// Get Ray from triangle to camera
			Math3d::vec3d vCameraRay = triTransformed.p[0] - camera.vCamera;
			
			// If ray is aligned with normal, then triangle is visible
			if (Math3d::Vector_DotProduct(normal, vCameraRay) < 0.0f)
			{
			#if LOADOBJ > 0
				vec3d light_direction = { 0.0f, 1.0f, -1.0f };
				light_direction = Vector_Normalise(light_direction);
				float dp = maxAB(0.1f, Vector_DotProduct(light_direction, normal));
			#endif
				// Convert World Space --> View Space
                Math3d::mat4x4 matView = camera.view();
				triViewed.p[0] = triTransformed.p[0] * matView;
				triViewed.p[1] = triTransformed.p[1] * matView;
				triViewed.p[2] = triTransformed.p[2] * matView;
				triViewed.texture = triTransformed.texture;
			#if LOADOBJ > 0
				triViewed.face = static_cast<uint8_t>(dp * 94);
			#else	
				triViewed.face = triTransformed.face;
			#endif
				triViewed.t[0] = triTransformed.t[0];
				triViewed.t[1] = triTransformed.t[1];
				triViewed.t[2] = triTransformed.t[2];

				// Clip Viewed Triangle against near plane, this could form two additional
				// additional triangles. 
				int nClippedTriangles = 0;
				Math3d::triangle clipped[2];
				nClippedTriangles = Math3d::Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

				// We may end up with multiple triangles form the clip, so project as
				// required
				for (int n = 0; n < nClippedTriangles; n++)
				{
					// Project triangles from 3D --> 2D
					triProjected.p[0] = clipped[n].p[0] * camera.matProj;
					triProjected.p[1] = clipped[n].p[1] * camera.matProj;
					triProjected.p[2] = clipped[n].p[2] * camera.matProj;
					triProjected.texture = clipped[n].texture;
					triProjected.face = clipped[n].face;
					triProjected.t[0] = clipped[n].t[0];
					triProjected.t[1] = clipped[n].t[1];
					triProjected.t[2] = clipped[n].t[2];


					triProjected.t[0].u = triProjected.t[0].u / triProjected.p[0].w;
					triProjected.t[1].u = triProjected.t[1].u / triProjected.p[1].w;
					triProjected.t[2].u = triProjected.t[2].u / triProjected.p[2].w;

					triProjected.t[0].v = triProjected.t[0].v / triProjected.p[0].w;
					triProjected.t[1].v = triProjected.t[1].v / triProjected.p[1].w;
					triProjected.t[2].v = triProjected.t[2].v / triProjected.p[2].w;

					triProjected.t[0].w = 1.0f / triProjected.p[0].w;
					triProjected.t[1].w = 1.0f / triProjected.p[1].w;
					triProjected.t[2].w = 1.0f / triProjected.p[2].w;


					// Scale into view, we moved the normalising into cartesian space
					// out of the matrix.vector function from the previous videos, so
					// do this manually
					triProjected.p[0] = triProjected.p[0] / triProjected.p[0].w;
					triProjected.p[1] = triProjected.p[1] / triProjected.p[1].w;
					triProjected.p[2] = triProjected.p[2] / triProjected.p[2].w;

					// X/Y are inverted so put them back
					triProjected.p[0].x *= -1.0f;
					triProjected.p[1].x *= -1.0f;
					triProjected.p[2].x *= -1.0f;
					triProjected.p[0].y *= -1.0f;
					triProjected.p[1].y *= -1.0f;
					triProjected.p[2].y *= -1.0f;

					// Offset verts into visible normalised space
					Math3d::vec3d vOffsetView = { 1,1,0 };
					triProjected.p[0] = triProjected.p[0] + vOffsetView;
					triProjected.p[1] = triProjected.p[1] + vOffsetView;
					triProjected.p[2] = triProjected.p[2] + vOffsetView;
					triProjected.p[0].x *= 0.5f * (float)ScreenWidth();
					triProjected.p[0].y *= 0.5f * (float)ScreenHeight();
					triProjected.p[1].x *= 0.5f * (float)ScreenWidth();
					triProjected.p[1].y *= 0.5f * (float)ScreenHeight();
					triProjected.p[2].x *= 0.5f * (float)ScreenWidth();
					triProjected.p[2].y *= 0.5f * (float)ScreenHeight();

					// Store triangle for sorting
					
					vecTrianglesToRaster.push_back(triProjected);
				}			
			}
		}
	}
}

void Renderer::render3d(Render3dParams params){
	Math3d::mat4x4 matView = params.camera.view();

	// Set up "World Tranmsform" though not updating theta 
	// makes this a bit redundant
	params.fTheta = deltaTime;
	Math3d::mat4x4 matRotZ, matRotX;
	matRotZ = Math3d::Matrix_MakeRotationZ(params.fTheta * 0);
	matRotX = Math3d::Matrix_MakeRotationX(params.fTheta * 0);

	Math3d::mat4x4 matTrans;
	matTrans = Math3d::Matrix_MakeTranslation(0.0f, 0.0f, 0.0f);

	Math3d::mat4x4 matWorld;
	matWorld = Math3d::Matrix_MakeIdentity();	// Form World Matrix
	matWorld = Math3d::Matrix_MultiplyMatrix(matRotZ, matRotX); // Transform by rotation
	matWorld = Math3d::Matrix_MultiplyMatrix(matWorld, matTrans); // Transform by translation

	// Store triagles for rastering later
	vector<Math3d::triangle> vecTrianglesToRaster;
#if LOADOBJ > 0
	rasterTriangles(params.camera, matWorld, vecTrianglesToRaster, *params.meshLoadedTris);

	sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](triangle &t1, triangle &t2)
	{
		float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
		float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
		return z1 > z2;
	});
#else
	set<Math3d::triangle> tris;

	Math3d::greedy(*params.chunk, tris);

	// Math3d::createMesh(tris, meshIndices);

	rasterTriangles(params.camera, matWorld, vecTrianglesToRaster, tris);
#endif
	// Clear Screen
	CleanPixels(' ');

	// Clear Depth Buffer
	for (int i = 0; i < ScreenWidth()*ScreenHeight(); i++)
		pDepthBuffer[i] = keyInput.depthBufferValue;

	// Loop through all transformed, viewed, projected, and sorted triangles
	int dp = 0;
	for (auto &triToRaster : vecTrianglesToRaster)
	{
		// Clip triangles against all four screen edges, this could yield
		// a bunch of triangles, so create a queue that we traverse to 
		//  ensure we only test new triangles generated against planes
		Math3d::triangle clipped[2];
		list<Math3d::triangle> listTriangles;
		vector<Math3d::vec3d> normalTriangles;
		vector<Math3d::vec3d> trianglePoints;

		// Add initial triangle
		listTriangles.push_back(triToRaster);

		normalTriangles.push_back(Math3d::getNormal(triToRaster));

		int nNewTriangles = 1;

		for (int p = 0; p < 4; p++)
		{
			int nTrisToAdd = 0;
			while (nNewTriangles > 0)
			{
				// Take triangle from front of queue
				Math3d::triangle test = listTriangles.front();
				listTriangles.pop_front();
				nNewTriangles--;

				// Clip it against a plane. We only need to test each 
				// subsequent plane, against subsequent new triangles
				// as all triangles after a plane clip are guaranteed
				// to lie on the inside of the plane. I like how this
				// comment is almost completely and utterly justified
				switch (p)
				{
				case 0:	nTrisToAdd = Math3d::Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 1:	nTrisToAdd = Math3d::Triangle_ClipAgainstPlane({ 0.0f, (float)ScreenHeight() - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 2:	nTrisToAdd = Math3d::Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				case 3:	nTrisToAdd = Math3d::Triangle_ClipAgainstPlane({ (float)ScreenWidth() - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
				}

				// Clipping may yield a variable number of triangles, so
				// add these new ones to the back of the queue for subsequent
				// clipping against next planes
				for (int w = 0; w < nTrisToAdd; w++){
					listTriangles.push_back(clipped[w]);
					normalTriangles.push_back(Math3d::getNormal(clipped[w]));
				}
			}
			nNewTriangles = listTriangles.size();
		}

		
		// Draw the transformed, viewed, clipped, projected, sorted, clipped triangles
		for (auto &t : listTriangles)
		{
			if (solid){
			#if LOADOBJ > 0
				FillTriangle(gradient[t.face], t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y);
			#else
				TexturedTriangle(t.p[0].x, t.p[0].y, t.t[0].u, t.t[0].v, t.t[0].w,
					t.p[1].x, t.p[1].y, t.t[1].u, t.t[1].v, t.t[1].w,
					t.p[2].x, t.p[2].y, t.t[2].u, t.t[2].v, t.t[2].w, params.textures[t.texture], t.face);
			#endif
			}
			if (wireframe){
				DrawTriangle('\xB2', t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y);
				// DrawLine('\xF1', t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y);
			}
		}
	}
}

void Renderer::TexturedTriangle(	int x1, int y1, float u1, float v1, float w1,
							int x2, int y2, float u2, float v2, float w2,
							int x3, int y3, float u3, float v3, float w3,
		Sprite *tex, uint8_t face)
	{
	if (y2 < y1)
	{
		swap(y1, y2);
		swap(x1, x2);
		swap(u1, u2);
		swap(v1, v2);
		swap(w1, w2);
	}

	if (y3 < y1)
	{
		swap(y1, y3);
		swap(x1, x3);
		swap(u1, u3);
		swap(v1, v3);
		swap(w1, w3);
	}

	if (y3 < y2)
	{
		swap(y2, y3);
		swap(x2, x3);
		swap(u2, u3);
		swap(v2, v3);
		swap(w2, w3);
	}

	int dy1 = y2 - y1;
	int dx1 = x2 - x1;
	float dv1 = v2 - v1;
	float du1 = u2 - u1;
	float dw1 = w2 - w1;

	int dy2 = y3 - y1;
	int dx2 = x3 - x1;
	float dv2 = v3 - v1;
	float du2 = u3 - u1;
	float dw2 = w3 - w1;

	float tex_u, tex_v, tex_w;

	float dax_step = 0, dbx_step = 0,
		du1_step = 0, dv1_step = 0,
		du2_step = 0, dv2_step = 0,
		dw1_step=0, dw2_step=0;

	if (dy1) dax_step = dx1 / (float)abs(dy1);
	if (dy2) dbx_step = dx2 / (float)abs(dy2);

	if (dy1) du1_step = du1 / (float)abs(dy1);
	if (dy1) dv1_step = dv1 / (float)abs(dy1);
	if (dy1) dw1_step = dw1 / (float)abs(dy1);

	if (dy2) du2_step = du2 / (float)abs(dy2);
	if (dy2) dv2_step = dv2 / (float)abs(dy2);
	if (dy2) dw2_step = dw2 / (float)abs(dy2);

	if (dy1)
	{
		for (int i = y1; i <= y2; i++)
		{
			int ax = x1 + (float)(i - y1) * dax_step;
			int bx = x1 + (float)(i - y1) * dbx_step;

			float tex_su = u1 + (float)(i - y1) * du1_step;
			float tex_sv = v1 + (float)(i - y1) * dv1_step;
			float tex_sw = w1 + (float)(i - y1) * dw1_step;

			float tex_eu = u1 + (float)(i - y1) * du2_step;
			float tex_ev = v1 + (float)(i - y1) * dv2_step;
			float tex_ew = w1 + (float)(i - y1) * dw2_step;

			if (ax > bx)
			{
				swap(ax, bx);
				swap(tex_su, tex_eu);
				swap(tex_sv, tex_ev);
				swap(tex_sw, tex_ew);
			}

			tex_u = tex_su;
			tex_v = tex_sv;
			tex_w = tex_sw;

			float tstep = 1.0f / ((float)(bx - ax));
			float t = 0.0f;

			for (int j = ax; j < bx; j++)
			{
				tex_u = (1.0f - t) * tex_su + t * tex_eu;
				tex_v = (1.0f - t) * tex_sv + t * tex_ev;
				tex_w = (1.0f - t) * tex_sw + t * tex_ew;
				if (tex_w > pDepthBuffer[i*ScreenWidth() + j])
				{
					Draw(j, i, tex->SampleGlyph(tex_u / tex_w, tex_v / tex_w, face));
					pDepthBuffer[i*ScreenWidth() + j] = tex_w;
				}
				t += tstep;
			}

		}
	}

	dy1 = y3 - y2;
	dx1 = x3 - x2;
	dv1 = v3 - v2;
	du1 = u3 - u2;
	dw1 = w3 - w2;

	if (dy1) dax_step = dx1 / (float)abs(dy1);
	if (dy2) dbx_step = dx2 / (float)abs(dy2);

	du1_step = 0, dv1_step = 0;
	if (dy1) du1_step = du1 / (float)abs(dy1);
	if (dy1) dv1_step = dv1 / (float)abs(dy1);
	if (dy1) dw1_step = dw1 / (float)abs(dy1);

	if (dy1)
	{
		for (int i = y2; i <= y3; i++)
		{
			int ax = x2 + (float)(i - y2) * dax_step;
			int bx = x1 + (float)(i - y1) * dbx_step;

			float tex_su = u2 + (float)(i - y2) * du1_step;
			float tex_sv = v2 + (float)(i - y2) * dv1_step;
			float tex_sw = w2 + (float)(i - y2) * dw1_step;

			float tex_eu = u1 + (float)(i - y1) * du2_step;
			float tex_ev = v1 + (float)(i - y1) * dv2_step;
			float tex_ew = w1 + (float)(i - y1) * dw2_step;

			if (ax > bx)
			{
				swap(ax, bx);
				swap(tex_su, tex_eu);
				swap(tex_sv, tex_ev);
				swap(tex_sw, tex_ew);
			}

			tex_u = tex_su;
			tex_v = tex_sv;
			tex_w = tex_sw;

			float tstep = 1.0f / ((float)(bx - ax));
			float t = 0.0f;

			for (int j = ax; j < bx; j++)
			{
				tex_u = (1.0f - t) * tex_su + t * tex_eu;
				tex_v = (1.0f - t) * tex_sv + t * tex_ev;
				tex_w = (1.0f - t) * tex_sw + t * tex_ew;

				if (tex_w > pDepthBuffer[i*ScreenWidth() + j])
				{
					Draw(j, i, tex->SampleGlyph(tex_u / tex_w, tex_v / tex_w, face));
					pDepthBuffer[i*ScreenWidth() + j] = tex_w;
				}
				t += tstep;
			}
		}	
	}		
}