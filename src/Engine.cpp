#include "../includes/Renderer.h"

bool Engine::onUserCreate() {
    textures.push_back(new Sprite("res/textures/none.txt", 2,true));
    textures.push_back(new Sprite("res/textures/grass.txt", 2,true));
    textures.push_back(new Sprite("res/textures/stone.txt", 2,true));
    textMap = new Sprite("res/textures/font.txt", 1, false);
    light_direction = { 0.0f, 2.0f, 1.0f };
    light_direction = Math3d::Vector_Normalise(light_direction);
    camera.init(90.0f, pixelAspect, aspect, false, WindowWidth(), WindowHeight());
#if LOADOBJ == 1
	mesh.LoadFromObjectFile("res/obj/Monkey.obj", false);
#elif LOADOBJ == 2
    mesh.LoadFromObjectFile("res/obj/Axis.obj", false);
#elif LOADOBJ == 3
    mesh.LoadFromObjectFile("res/obj/Block.obj", false);
#elif LOADOBJ == 4
    mesh.LoadFromObjectFile("res/obj/Icosphere.obj", false);
#elif LOADOBJ == 5
    mesh.LoadFromObjectFile("res/obj/mineterrain.obj", false);
#elif LOADOBJ == 6
    mesh.LoadFromObjectFile("res/obj/Montain.obj", false);
#endif
#if LOADOBJ == 0
    // voxels = new Math3d::VoxelFace[Math3d::CHUNK_WIDTH * Math3d::CHUNK_WIDTH * Math3d::CHUNK_WIDTH];
    Math3d::initVoxels(chunk);
    Math3d::Faces cubeFaces(0);
    meshVertexs.push_back(Math3d::cubePoints(cubeFaces));
    meshIndices.push_back(Math3d::generateRegularVoxelEntitie(1));
#endif
    return true;
}

void Engine::DebugPlay(){
    ostringstream oss; 
    oss << fixed << setprecision(1) << camera.vCamera.x << ", " << camera.vCamera.y << ", " << camera.vCamera.z;
    DrawText('\xDB', oss.str(), 0, 8, 0, textMap);
    ostringstream oss2; 
    oss2 << fixed << setprecision(1) << camera.vLookDir.x << ", " << camera.vLookDir.y << ", " << camera.vLookDir.z;
    DrawText('\xDB', oss2.str(), 0, 16, 0, textMap);

    int minute = static_cast<int>(deltaTime) / 60;
    int second = static_cast<int>(deltaTime) % 60;
    int milissecond = static_cast<int>(std::round((deltaTime - static_cast<float>(minute * 60 + second)) * 1000));
    ostringstream ossFPS;
    ossFPS << fixed << setprecision(1) << "FPS: " << frames << " | " << minute << "m " << second << "s " << milissecond << "ms";
    DrawText('\xDB', ossFPS.str(), 0, 0, 0, textMap);

	DrawText('\xDB', ossCustom.str(), 0, 3*8, 0, textMap);

    ostringstream ossCamera;
    ossCamera << fixed << setprecision(1) << "yaw: " << camera.fYaw << " pitch: " << camera.fPitch;
    DrawText('\xDB', ossCamera.str(), 0, 4*8, 0, textMap);

}

bool Engine::update() {
    
    keyInput.recieveText(currentCommand);
    
    if (currentCommand.find("/wire") == 0)
        wireframe = !wireframe;
    if (currentCommand.find("/solid") == 0)
        solid = !solid;
    if (currentCommand.find("/look") == 0)
        camera.freeLook = !camera.freeLook;
    if (currentCommand.find("/remove") == 0){
        currentCommand.erase(0, 7);
        istringstream iss(currentCommand);
        int _x, _y, _z;
        if (iss >> _x >> _y >> _z) {
            uint16_t indice = getIndicePosition({(float)_x, (float)_y, (float)_z});
        
            VoxelData* voxel = indice >= 0 && indice < chunk.voxels.size() ? 
                               &chunk.voxels[indice] : nullptr;
            if (voxel != nullptr) voxel->type = 0;
        }
    }

    if (currentCommand.find("/add") == 0){
        size_t addPos = currentCommand.find("/add");
        if (addPos != std::string::npos) {
            // Encontrou "/add", agora vamos analisar o restante do comando
            std::istringstream iss(currentCommand.substr(addPos + 4)); // +4 para pular "/add"
            float _x, _y, _z;
            int typeVoxel = 0;
            if (iss >> typeVoxel >> _x >> _y >> _z) {
                uint16_t indice = getIndicePosition({_x, _y, _z});
                VoxelData* voxel = indice >= 0 && indice < chunk.voxels.size() ? 
                                &chunk.voxels[indice] : nullptr;
                if (voxel != nullptr) {
                    voxel->type = (uint8_t)typeVoxel&0xFF;
                }
            }
        }
    }

    camera.configureProjection();

    if (!keyInput.chat){
        camera.rotate();
        camera.move();
    }

#if LOADOBJ > 0
    render3d({nullptr, &mesh.tris, camera, textures, fTheta});
#else
    render3d({&chunk, nullptr, camera, textures, fTheta});
#endif

    if (keyInput.chat){
        DrawText('\xDB', "CHAT: ", 0, ScreenHeight() - 16, 0, textMap);
        DrawText('\xDB', keyInput.getText(), 7*8, ScreenHeight() - 16, 0, textMap);
    }

    if (keyInput.debug)
        DebugPlay();
    
    ossCustom.str("");
    return true;
}