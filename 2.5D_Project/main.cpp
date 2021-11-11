#include<Windows.h>
#include <iostream>
#include<string>
#include<map>
#define GLFW_STATIC
#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include"CWindow.h"
#include"Shader.h"


#include"Camera.h"
#include"Game.h"
#include"Resource.h"
#include"Floor.h"
#include"UI.h"
#include"TextRender.h"
CWindow Wnd;
HWND hwnd;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

vector<std::string> faces
{
    "Texture/right.jpg",
    "Texture/left.jpg",
    "Texture/top.jpg",
    "Texture/bottom.jpg",
    "Texture/front.jpg",
    "Texture/back.jpg",
};

Camera camera(glm::vec3(0, 8.0f, 8.0f), glm::radians(-45.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));//鼠标控制玩家转向
Camera playercamera(glm::vec3(0, 8.0f, 8.0f), glm::radians(-45.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));
Camera Gamecamera(glm::vec3(0, 8.0f, 8.0f), glm::radians(-45.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));

float x = 0.0f;
float y = 0.0f;
float lastx;
float lasty;
bool firstMouse = true;

double mouse_xpos;   //鼠标位置
double mouse_ypos;

bool cameraposX_add = false;    //判断摄像机是否需要向左移动
float CposX_add = 0;            //记录摄像机需要向左移动多少
bool cameraposX_reduce = false;
float CposX_reduce = 0;
bool cameraposZ_add = false;    //判断摄像机是否需要向外移动
float CposZ_add = 0;
bool cameraposZ_reduce = false;
float CposZ_reduce = 0;

bool UIbag = false;     //判断是否弹出背包UI
unsigned int uibag = 0;    //按下一次按键UI图片一直存在，再次按下取消(单数存在，双数取消)


unsigned int loadCubemap(vector<std::string> faces)     //加载天空盒
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void processInput(GLFWwindow* window)
{
    if (Wnd.stop)
        return;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstMouse = true;
        Wnd.Stop();
    }
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
   // if (Wnd.stop)
   //     return;
    if (firstMouse == true) {
        lastx = xPos;
        lasty = yPos;
        camera.mouse_posx = xPos;
        camera.mouse_posy = yPos;
        firstMouse = false;
    }
    float deltax, deltay;
    deltax = xPos - lastx;
    deltay = yPos - lasty;
    lastx = xPos;
    lasty = yPos;
    camera.mouse_posx = xPos;
    camera.mouse_posy = yPos;
    camera.ProcessMouseMovement(deltax, deltay); //传参deltax为正时，鼠标向左移，摄像机视角向左移
}

void mouse_button(GLFWwindow* window, int button, int action, int mode) {
    if (action == GLFW_PRESS) {
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:

            //getting cursor position 
            glfwGetCursorPos(window, &mouse_xpos, &mouse_ypos);
            // if (xpos >= 300 && xpos <= 495 && ypos >= 420 && ypos <= 455) {
            //     
            // }
            // if (xpos >= 250 && xpos <= 585 && ypos >= 190 && ypos <= 250) {
            //   
            // }
            if (mouse_xpos >= 1321 && mouse_xpos <= 1425 && mouse_ypos >= 260 && mouse_ypos <= 345) {
                UIbag = false;
                uibag = 2;
            }
            //cout << "Cursor Position at " << xpos << " : " << ypos << endl;
            break;
        default:
            break;
        }
    }
}

void ExampleGLFWkeyfun(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        switch (key)
        {
        case GLFW_KEY_I:
            UIbag = true;
            uibag++;
            if (uibag % 2 != 0)
            {
               // Wnd.stop = true;
            }
            else
            {
                //Wnd.stop = false;
            }
            break;
        case GLFW_KEY_ESCAPE:
        
            break;
        default:
            break;
        }
    }
}

int WinMain(HINSTANCE, HINSTANCE, PSTR, int) {

    SetProcessDPIAware();
    int cx = GetSystemMetrics(SM_CXSCREEN);
    int cy = GetSystemMetrics(SM_CYSCREEN);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(cx, cy, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);                 //创建窗口上下文
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetKeyCallback(window, ExampleGLFWkeyfun);
    glfwSetMouseButtonCallback(window, mouse_button);   //鼠标按钮回调函数
    glfwSetCursorPosCallback(window, mouse_callback);//鼠标回调函数

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    glViewport(0, 0, cx, cy);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    hwnd = GetActiveWindow();
    TCHAR Buffer[128];
    ShowWindow(hwnd, SW_MAXIMIZE);
    Wnd.SetWindow(hwnd);


    Shader* FBXShader = new Shader("Shader/FBXanimationVertex.vert", "Shader/FBXanimationFragment.frag");
    //Shader* FBXShader2 = new Shader("FBXanimationVertex.vert", "FBXanimationFragment.frag");
    Shader* OBJShader = new Shader("Shader/vertexSource.vert", "Shader/fragmentSource.frag");
    Shader* floorShader = new Shader("Shader/FloorVertex.vert", "Shader/FloorFragment.frag");
    Shader* wallShader = new Shader("Shader/WallVertex.vert", "Shader/WallFragment.frag");
    Shader* enemyShader = new Shader("Shader/WallVertex.vert", "Shader/WallFragment.frag");
    Shader* Textshader = new Shader("Shader/Text_vertex.vert", "Shader/Text_fragment.frag");
    Shader* UIshader = new Shader("Shader/UiVertex.vert", "Shader/UiFragment.frag");
    Shader* UIequipshader = new Shader("Shader/UiVertex.vert", "Shader/UiFragment.frag");
    Shader* skyboxshader = new Shader("Shader/Sky_vertex.vert", "Shader/Sky_fragment.frag");

    //加载天空盒贴图
    unsigned int cubemapTexture = loadCubemap(faces);

    UI* GameUI = new UI(UIshader, UIequipshader);

    Model* player = new Model("model/Fast Run.fbx");
    // Model* enemy = new Model(exepath.substr(0, exepath.find_last_of('\\')) + "\\model\\enemy_Situps .fbx");
    ObjModel* npc = new ObjModel( "model/ChocoSantaClaus06.obj");
    ObjModel* wall = new ObjModel("model/Mu_Box.obj");
   
    ObjModel* obstacles = new ObjModel("model/Plantform.obj");
    ObjModel* melee_weapons = new ObjModel("model/soldier_78_gun.obj");
    ObjModel* drop = new ObjModel("model/ChocoSantaClaus06.obj");
    //  ObjModel* butter = new ObjModel(exepath.substr(0, exepath.find_last_of('\\')) + "\\model\\sphere.obj");

      //cout << exepath.substr(0, exepath.find_last_of('\\')) << endl;

    Game* game = new Game(FBXShader, Gamecamera,Wnd,glm::vec3(0, 0, 1.0f), glm::vec3(0.36f, 0.34f, 16.0f));
    Floor* floor = new Floor(floorShader);

    //skyboxVAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    skyboxshader->use();
    skyboxshader->setuniform1i("skybox", 0);

    Textshader->use();
    glm::mat4 Textprojection = glm::ortho(0.0f, 1200.0f, 0.0f, 800.0f);
    glUniformMatrix4fv(glGetUniformLocation(Textshader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(Textprojection));

    //墙体实例化VAO绑定
    game->WallInit(wall);
    game->obstaclesInit(obstacles);


    game->EnemyInit();
    //绑定地板VAO
    floor->TransDoorInit();
    floor->Init();
    floor->NoiseInit();
    //绑定UI界面VAO
    GameUI->GameStartTextureInit();
    GameUI->GetInit();
    //文字绑定
    TextInit();

    glm::mat4 viewMat;
    glm::mat4 GameviewMat;
    glm::mat4 playerviewMat;

    glm::mat4 projMat;
    glm::mat4 UIprojMat;
    projMat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    //UIprojMat= glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1.0f);
   // projMat = glm::ortho(0.0f, 600.0f, 0.0f, 800.0f);

    image("Texture/box.jpg", GL_RGB, GL_RGB, 1);
    image("Texture/floor.jpg", GL_RGB, GL_RGB, 2);
    image("Texture/UIbag.png", GL_RGBA, GL_RGBA, 3);
    
    image("Texture/Nnn.png", GL_RGBA, GL_RGBA, 5);
    image("Texture/qiang.jpg", GL_RGB, GL_RGB, 6);
    image("Texture/awesomeface.png", GL_RGBA, GL_RGBA, 7);
    image("Texture/1.png", GL_RGBA, GL_RGBA, 8);
    image("Texture/yellow.jpg", GL_RGB, GL_RGB, 9);
    image("Texture/GameStartTexture.png", GL_RGBA, GL_RGBA, 10);

    std::string p;
    const char* pn;
    for (unsigned int i = 0; i < 123; i++)  //加载PNG序列
    {
        if (i >= 0 && i < 10)
        {
            p = "Magic/Magic_circle0000" + std::to_string(i) + ".png";
        }
        else if (i < 100)
        {
            p = "Magic/Magic_circle000" + std::to_string(i) + ".png";
        }
        else
        {
            p = "Magic/Magic_circle00" + std::to_string(i) + ".png";
        }
        pn = p.c_str();
        image(pn, GL_RGBA, GL_RGBA, i + 11);
    }       //11到133

    for (int i = 2; i < 55; i++)
    {
        if (i >= 0 && i < 10)
        {
            p = "DianJi/DianJi_0000" + std::to_string(i) + ".png";
        }
        else if (i < 100)
        {
            p = "DianJi/DianJi_000" + std::to_string(i) + ".png";
        }

        pn = p.c_str();
        image(pn, GL_RGBA, GL_RGBA, i + 132);
    }       //134-134+59

    //GameUI->DrawGameStartTexture();
   // Wnd.getmenu();
   // wsprintf(Buffer, TEXT("攻击力:%.2f", game->Butter[0]->butterAttack));
    
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //SetTimer(hwnd, 1, 100, NULL);
        if (!Wnd.start)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            

            Wnd.Menuclear();//清除上次状态的菜单
            Wnd.getmenu();  //绘制新状态的菜单
            Wnd.Stop();     //游戏未开始状态

            UIshader->use();
            GameUI->DrawGameStartTexture(UIshader);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        else
        {
            Wnd.Menuclear();

            processInput(window);
            game->processInput(window);
            //当人物进入下一个房间时控制摄像机视角移动
        /*    if (game->pos.x > game->Gamecamera.Position.x + 5.0f)         //右
            {
                cameraposX_add = true;
                CposX_add = game->Gamecamera.Position.x + 8.0f;
            }
            if (cameraposX_add)
            {
                game->Gamecamera.Position.x += 0.5f;
                if (game->Gamecamera.Position.x   > CposX_add)
                {
                    cameraposX_add = false;
                }
            }



            if (game->pos.x < game->Gamecamera.Position.x - 5.0f) //左
            {
                cameraposX_reduce = true;
                CposX_reduce = game->Gamecamera.Position.x - 8.0f;
            }
            if (cameraposX_reduce)
            {
                game->Gamecamera.Position.x -= 0.5f;

                if (game->Gamecamera.Position.x < CposX_reduce)
                {
                    cameraposX_reduce = false;
                }
            }

              //上
            if (game->pos.z < game->Gamecamera.Position.z - 13.0f)  //摄像机Z的初始位置为8；与game模型对齐的话就要先减8
            {
                cameraposZ_reduce = true;
                CposZ_reduce = game->Gamecamera.Position.z - 5.0f;
            }
            if (cameraposZ_reduce)
            {
                game->Gamecamera.Position.z -= 0.5f;

                if (game->Gamecamera.Position.z < CposZ_reduce)
                {
                    cameraposZ_reduce = false;
                }
            }

            if (game->pos.z > game->Gamecamera.Position.z - 3.0f)
            {
                //game->Gamecamera.Position.z += 5.0f;
                cameraposZ_add = true;
                CposZ_add = game->Gamecamera.Position.z + 5.0f;
            }
            if (cameraposZ_add)
            {
                game->Gamecamera.Position.z += 0.5f;
                if (game->Gamecamera.Position.z > CposZ_add)
                {
                    cameraposZ_add = false;
                }
            }*/
            /*///////////////////////*/
            

            Wnd.clear();
            wsprintf(Buffer, TEXT("生命值: %d"), game->PlayLife);
            Wnd.Textout(Buffer, 0, 200, 30);
            wsprintf(Buffer, TEXT("攻击力:5"));
            Wnd.Textout(Buffer, 0, 240, 30);

            viewMat = camera.GetViewMatrix();
            GameviewMat = game->Gamecamera.GetViewMatrix();

            //UI渲染
            UIshader->use();
            //GameUI->GetInit();

            if (UIbag && uibag % 2 != 0)
            {
                glfwGetCursorPos(window, &mouse_xpos, &mouse_ypos);     //获取鼠标位置
                if (mouse_xpos > 642 && mouse_xpos < 736 && mouse_ypos >= 358 && mouse_ypos <= 410 && UIbag)
                {
                    wsprintf(Buffer, TEXT("装备"));
                    Wnd.EquipmentTextout(Buffer, 600, 400, 20);
                }
                else
                {
                    Wnd.Equipmentclear();
                }
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                GameUI->GetGoings();
                GameUI->GetBagUI();

                // GameUI->GetInit();
            }
            else
            {
                UIbag = false;
                Wnd.Equipmentclear();
            }

            //渲染地板
            floorShader->use();
            glUniformMatrix4fv(glGetUniformLocation(floorShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(GameviewMat));
            glUniformMatrix4fv(glGetUniformLocation(floorShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));

            floor->draw();
            floor->drawTransDoor();
            floor->drawNoise();
           

            //渲染墙体
            wallShader->use();
            glUniformMatrix4fv(glGetUniformLocation(wallShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(GameviewMat));
            glUniformMatrix4fv(glGetUniformLocation(wallShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
            game->WallDraw(wallShader);
            game->ObstaclesDraw(wallShader);

            OBJShader->use();
            glUniformMatrix4fv(glGetUniformLocation(OBJShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(GameviewMat));
            glUniformMatrix4fv(glGetUniformLocation(OBJShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));

            game->mouse_gap -= 0.5f;//鼠标点击频率

            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && game->mouse_gap <= 0) {
                game->mouse_gap = 5.0f;//鼠标点击间隔
                game->Melee_weapons->melee_weapons_trans = 90.0f;
                game->bo = true;
                game->Butter.push_back(new ObjModel("model/sphere.obj", game->pos));

                //子弹每次加载时，如果模型带有贴图每次都会用stbi_image库加载贴图。会卡
                // 解决---在代码内给子弹模型加上贴图
            }
            //判断玩家子弹是否存在
            if (game->bo)
            {
                game->ButterInit(camera, OBJShader);
                game->buttercheck();
                game->melee_weapons_check();
            }

            //枪类武器
            game->Melee_weaponsInit(melee_weapons, OBJShader);

            // GameviewMat = game->Gamecamera.GetViewMatrix();
            FBXShader->use();
            glUniformMatrix4fv(glGetUniformLocation(FBXShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(GameviewMat));
            glUniformMatrix4fv(glGetUniformLocation(FBXShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));

            
            //渲染文字
            std::stringstream life;
            life << game->PlayLife;

            Textshader->use();
            RenderText(Textshader, "BuiBuiBui!!!:" + life.str(), 0, 750.0f, 1.0f, glm::vec3(0.5f, 0.8f, 0.0f));

            //NPC
            game->NPCInit(npc, OBJShader);

            if (game->pos.x <= game->NPC->NPCpos.x + 0.3f && game->pos.x >= game->NPC->NPCpos.x - 0.3f && game->pos.z <= game->NPC->NPCpos.z + 0.5f && game->pos.z >= game->NPC->NPCpos.z - 0.5f)
            {
                GameUI->NPCconmmunication();
            }
            
            float elapsedTime = (float)glfwGetTime() * 20;
            elapsedTime = (int)elapsedTime % (480 / 30);
            elapsedTime = elapsedTime < 1.0f ? 1.0f : elapsedTime;
            // printf("%.2f\n", elapsedTime);

            player->Action(elapsedTime);
            //enemy->Action(elapsedTime);

            
            game->EnemyDraw(FBXShader);//渲染敌人
            game->PlayInit(player, camera);//渲染玩家

            //渲染掉落物
            if (game->dropAmount > game->dropCreate)
            {
                game->dropCreate = game->dropAmount;
                game->GetDropModel(drop);
                //game->Drop.push_back(new ObjModel(exepath.substr(0, exepath.find_last_of('\\')) + "\\model\\ChocoSantaClaus06.obj"));
            }
            if (game->dropCreate>0)
            {
                for (unsigned int i = 0; i < game->dropCreate; i++)
                {
                    wsprintf(Buffer, TEXT("AAAAAA"));
                    //Wnd.Textout(Buffer, 30 * game->Drop[i]->dropPos.x - 5 * (8 - game->gameCameraPos.x + game->gameCameraPos.z), 30 * game->Drop[i]->dropPos.z + 5 * (8 - game->gameCameraPos.z), 30);
                    Wnd.Textout(Buffer, 300 + 50*i, 500, 30);
                }
            }
            //wsprintf(Buffer, TEXT("AAAAAA"));
            //Wnd.Textout(Buffer, 300, 500, 30);
            game->DropInit(OBJShader);
            //玩家到达一定位置敌人生成子弹
            game->Play_Enemy_gap();

            for (unsigned int i = 0; i < game->ALL_enemy.size(); i++)
            {
                if (game->ALL_enemy[i].enemy->enemy_shootButter)  //若玩家到达敌人检测范围则enemy_shootButter为真
                {
                    game->ALL_enemy[i].enemy->enemybutterRate -= 0.1f;
                    if (game->ALL_enemy[i].enemy->enemybutterRate <= 0)
                    {
                        game->enemybo = true;
                        game->ALL_enemy[i].enemy->enemybutterRate = 5.0f;
                        game->EnemyButter.push_back(new ObjModel("model/sphere.obj", game->ALL_enemy[i].enemy->enemyPos));
                    }
                }
            }

            //检测敌人是否生成子弹并发射
            if (game->enemybo)
            {
                game->EnemyButterInit(OBJShader);
            }

            game->enemyButtercheck();//子弹的碰撞检测

            //玩家踩到陷阱扣除生命值
            if (game->Player->pos.x >= floor->NoisePos.x - 0.6f && game->Player->pos.x <= floor->NoisePos.x + 0.6f && game->Player->pos.z > floor->NoisePos.z - 0.6f && game->Player->pos.z < floor->NoisePos.z + 0.6f)
            {
                game->Play_injured_interval--;  //受伤间隔

                if (game->Play_injured_interval <= 0)
                {
                    game->Play_injured_interval = 30;
                    game->PlayLife--;
                    //printf("%.2lf\n", game->pos.x);
                }
            }
            else         //当玩家不在陷阱内，受伤间隔变为零。保证下一次踩到陷阱时会立即扣血
            {
                game->Play_injured_interval = 1;
            }
            
            
            //检测玩家进入传送门
            if (game->Player->pos.x >= floor->TransDoorPos.x - 0.6f && game->Player->pos.x <= floor->TransDoorPos.x + 0.6f && game->Player->pos.z > floor->TransDoorPos.z - 0.6f && game->Player->pos.z < floor->TransDoorPos.z + 0.6f)
            {
                //wall->Leve = 2;
                floor->TransTime--;
                floor->drawDianJi();
                if (floor->TransTime<=0)
                {
                    floor->TransTime = 100;
                    game->pos.x = 0;    //玩家的位置是由Game类中pos变量控制的
                    game->pos.z = 1.0f;
                    game->Gamecamera.Position = glm::vec3(0, 8.0f, 8.0f);
                }
            }
            else        //不在传送门中，传导时间恢复
            {
                floor->TransTime = 100;
                floor->DianJiTextureGap = 2.0f;     //电击贴图恢复初始状态
                floor->DianjiTextureID = 134;
                floor->DianJiTransPos.y = 0.01f;
            }

            //skyBos;
            glm::mat4 SkymodelMat;
            SkymodelMat = glm::rotate(SkymodelMat, glm::radians(40.0f), glm::vec3(1.0f, 0, 0));
            GameviewMat = glm::mat4(glm::mat3(game->Gamecamera.GetViewMatrix()));
            glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
            skyboxshader->use();
            glUniformMatrix4fv(glGetUniformLocation(skyboxshader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(SkymodelMat));
            glUniformMatrix4fv(glGetUniformLocation(skyboxshader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(GameviewMat));
            glUniformMatrix4fv(glGetUniformLocation(skyboxshader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
            glBindVertexArray(skyboxVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS);

            glfwSwapBuffers(window);
            while (Wnd.stop)  //暂停
            {

                //glfwSwapBuffers(window);
                processInput(window);
                glfwPollEvents();
            }
            glfwPollEvents();
            game->Gamecamera.updatecameraPos();
        }
    }
    glfwTerminate();

	//return (int)msg.wParam;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}