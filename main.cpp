#include "engine.h"

int main()
{
    const int emuScreenWidth = 400;
    const int emuScreenHeight = 300;

    int screenWidth = emuScreenWidth;
    int screenHeight = emuScreenHeight;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE); 

    InitWindow(screenWidth, screenHeight, "DMM");
    // MaximizeWindow();

    SetTargetFPS(60);

    // Initialize the camera
    Camera camera = { { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

    float sensitivity = 1;

    // Init
    Hud character;

    // Model scene = LoadModel("assets/models/scene0.obj");
    Model scene = LoadModel("assets/models/sceneBSP1.obj");

    Model cube = LoadModelFromMesh(GenMeshCube(0.5,0.5,0.5));

    Model cone = LoadModelFromMesh(GenMeshCone(0.1,1,10));

    Model sphere = LoadModelFromMesh(GenMeshSphere(0.2f,10,10));

    Vector3 playerPos = {0,0.51,0};

    float playerRadius = 0.5;

    float playerSpeed = 0.1;

    Color playerColor = GREEN;

    Object player({{0,1,0},{0,0,1,0},{1,1,1}});
    Object cameraVSupport({{0,0,3},{0,0,1,0},{1,1,1}});
    player.addChild(&cameraVSupport);

    Vector2 angle = {0,0};

    camera.position = {0,3,-3};
    camera.target = {0,0,0};

    // bsp test
    std::vector<Vector3> vertices;
    for(int i = 0; i < scene.meshes[0].vertexCount; i++)
        vertices.push_back({
            scene.meshes[0].vertices[i*3],
            scene.meshes[0].vertices[i*3+1],
            scene.meshes[0].vertices[i*3+2]});

    std::cout << "Creating BSP tree from " << vertices.size() << " vertices..." << std::endl;

    BSPNode * tree = BuildBSPTree(vertices);

    std::cout << "BSP tree generated" << std::endl;

    // Player physics

    float playerVerticalSpeed = 0;

    float gravity = 0.003f;

    while (!(WindowShouldClose()&&!IsKeyPressed(KEY_ESCAPE))) // Main loop
    {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        // Update camera
        if(IsCursorOnScreen()&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT)&&!IsCursorHidden()) DisableCursor();

        camera.target = cameraVSupport.getGlobalPosition();
        camera.position = player.getGlobalPosition();

        if(IsCursorHidden())
        {
            angle.x+=(float)GetMouseDelta().x/screenWidth*sensitivity;
            float temp = angle.y - ((float)GetMouseDelta().y/screenWidth*sensitivity);
            angle.y = (temp > -3/2 && temp < 3/2) ? temp : angle.y; // Limits the vertical view

            if(IsKeyPressed(KEY_ESCAPE)) EnableCursor();
        }


        player.transform.rotation = QuaternionProduct(VectorAngleQuaternion({1,0,0},angle.y),VectorAngleQuaternion({0,1,0},angle.x));

        // Movement

        Vector3 localHorizontalMovement = {0,0,0};
        Vector3 movement = {0,0,0};

        if(IsKeyDown(KEY_W)) localHorizontalMovement.x ++;
        if(IsKeyDown(KEY_S)) localHorizontalMovement.x --;
        if(IsKeyDown(KEY_A)) localHorizontalMovement.y ++;
        if(IsKeyDown(KEY_D)) localHorizontalMovement.y --;

        float rot = angle.x + atan2(localHorizontalMovement.x, localHorizontalMovement.y);
        Vector2 hMovement = {cosf(rot)*playerSpeed,sinf(rot)*playerSpeed};

        movement = {hMovement.x,0,hMovement.y};

        Vector3 playerMove = player.transform.translation;
        Vector3 playerVelocity = {0,0,0};

        if(localHorizontalMovement.x!=0||localHorizontalMovement.y!=0)
        {
            playerMove = {
                player.transform.translation.x + hMovement.x,
                player.transform.translation.y,
                player.transform.translation.z + hMovement.y};

            playerVelocity = {hMovement.x,0,hMovement.y};
        }

        if(IsModelReady(scene))
        {

            Vector3 shiftDelta = {0,0,0};

            Ray rayDown = {{playerMove.x,playerMove.y+0.6,playerMove.z},{0,-1,0}};

            RayCollision rayDownCollision = GetRayCollisionMesh(rayDown,scene.meshes[0],scene.transform);

            if(rayDownCollision.distance<0.1&&rayDownCollision.hit)
            {
                if(playerVerticalSpeed<=0)
                {
                    if(IsKeyDown(KEY_SPACE)) playerVerticalSpeed = 0.06;
                }

                playerVerticalSpeed = playerVerticalSpeed < 0 ? 0 : playerVerticalSpeed;
            }
            // else
            //     playerVerticalSpeed -= gravity;
                
            // playerMove.y += playerVerticalSpeed;

            // playerVelocity.y+=playerVerticalSpeed;

            // bool coll = CheckCollisionSphereMesh(playerMove,playerRadius,scene.meshes[0],&shiftDelta);
            // float distance = INFINITY;
            // int bspIterations = 0;
            // shiftDelta = {0,0,0};
            // while(SphereBSPCollision({playerMove.x,playerMove.y,playerMove.z},tree,&shiftDelta,&distance,0))
            // {
            //     playerMove = {playerMove.x+shiftDelta.x,playerMove.y+shiftDelta.y,playerMove.z+shiftDelta.z};
            //     std::cout << "DELTA " << Vector3String(shiftDelta) << std::endl;
            //     shiftDelta = {0,0,0};
            //     bspIterations++;
            // }
            // std::cout << "BSP " << bspIterations << std::endl;
            
            // player.transform.translation = {playerMove.x+shiftDelta.x,playerMove.y+shiftDelta.y,playerMove.z+shiftDelta.z};

            // playerVelocity = Vector3Subtract(playerVelocity,Vector3ScalarProduct(shiftDelta,Vector3DotProduct(playerVelocity,shiftDelta)));

            // player.transform.translation = { 
            //     player.transform.translation.x+playerVelocity.x, 
            //     player.transform.translation.y+playerVelocity.y,
            //     player.transform.translation.z+playerVelocity.z};

            // PointBSPCollision({playerMove.x,playerMove.y,playerMove.z},tree);

            Vector3 intersection = playerMove;
            Vector3 intersectionNormal;

            if(BSPRay(tree,player.transform.translation,playerMove,&intersection,&intersectionNormal))
            {
                std::cout << "intersection " << Vector3String(intersection) << std::endl;
                std::cout << "normal " << Vector3String(intersectionNormal) << std::endl;
                std::cout << "projection " << Vector3String(Vector3NormalProject(Vector3Subtract(playerMove,intersection),intersectionNormal)) << std::endl;
                Vector3 finalMovement =  Vector3Sum(Vector3NormalProject(Vector3Subtract(playerMove,intersection),intersectionNormal),intersection);
                std::cout << "move " << Vector3String(finalMovement) << std::endl;
                player.transform.translation = finalMovement;
                // std::cout << "normal " << Vector3String(Vector3NormalProject(Vector3Subtract(playerMove,intersection),intersectionNormal)) << std::endl;
                // player.transform.translation = intersection;
            }
                
            // else
            //     player.transform.translation = playerMove;

            if(!PointBSPCollision(player.transform.translation,tree))
                playerColor = GREEN;  
            else 
                playerColor = RED;
        }

        // Other
            

        ClearBackground(BLACK);
        
        BeginMode3D(camera);

        player.update();

        DrawModel(sphere,{
            player.getGlobalPosition().x,
            player.getGlobalPosition().y+0.0f,
            player.getGlobalPosition().z},1,playerColor);

        DrawModel(scene,Vector3{0,0,0},1.0f,WHITE);

        DrawModelWires(scene,Vector3{0,0,0},1.0f,RED);


        // DrawSphereWires(player.getGlobalPosition(),playerRadius,10,10,BLUE);

        // DrawGrid(10, 1.0f);

        EndMode3D();

        BeginDrawing();

        // Hud

        // character.hudPos = {(float)((short)(character.hudPos.x + 10) % screenWidth), (float)((short)(character.hudPos.y + 10) % screenHeight)};
        // character.size = { (float)32.0f*(screenWidth/(float)emuScreenWidth),(float)32.0f*(screenWidth/(float)emuScreenWidth)};

        

        // character.draw({((float)screenWidth/emuScreenWidth),((float)screenWidth/emuScreenWidth)},{0,(float)screenHeight});
        // character.draw({((float)screenWidth/emuScreenWidth),((float)screenWidth/emuScreenWidth)},{0,(float)screenHeight});

        EndDrawing();
    }

    CloseWindow();

    // system("pause");

    return 0;
}