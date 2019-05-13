#include "customframe.h"

#include "state.h"

#include <spm.h>
#include <iostream>
#include <fstream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <3te/debug.h>

using namespace burbokop::tte;



CustomFrame::CustomFrame() : Frame() {
    this->resolution = new Vector(800, 600);
}

void CustomFrame::initGame() {
    State::camera = new Vector3d(6, 37, 64);
}

void CustomFrame::gameLoop() {
    while (!State::commandQueue->empty())
    {
        std::string command = State::commandQueue->front();
        State::commandQueue->pop();

        std::cout << command << "\n";
        if(command == "/rv") {
            State::horizontalAngle = 0;
            State::verticalAngle = 0;
        }
        else if(command == "/gon") {
            State::gravity = true;
            State::consoleOut->push_back(" gravity enabled");
        }
        else if(command == "/goff") {
            State::gravity = false;
            State::consoleOut->push_back(" gravity disabled");
        }
        else if(command == "/lon") {
            glEnable(GL_LIGHTING);
            State::consoleOut->push_back(" lights enabled");
        }
        else if(command == "/loff") {
            glDisable(GL_LIGHTING);
            State::consoleOut->push_back(" lights disabled");
        }
        else if(command == "/help") {
            State::consoleOut->push_back(" /rv - reset view");
            State::consoleOut->push_back(" /gon - gravity on");
            State::consoleOut->push_back(" /goff - gravity off");
            State::consoleOut->push_back(" /lon - light on");
            State::consoleOut->push_back(" /loff - light off");
        }
        else {
            State::consoleOut->push_back(" unknown command");
        }
        //State::consoleOut->push_back(command);
        State::existenceTimer->reset();
    }
    if(State::existenceTimer->count() && State::consoleOut->size() > 10) State::consoleOut->erase(State::consoleOut->begin());

    State::ticks += 0.3;
    State::position = Vector::createByAngle(20, State::ticks);

    Vector3d *translation = Vector3d::createByArray(State::keyPressed, State::horizontalAngle);
    if(State::keyPressed[6]) translation = translation->multiply(16);

    if(State::gravity) translation->setZ(-2);
    State::camera = State::camera->add(translation);
    if(State::map && State::collision) State::handleCollision(State::camera, State::map);
    delete translation;
    State::cameraView = new Vector3d(State::camera->getDoubleX() + std::cos(State::horizontalAngle), State::camera->getDoubleY() + std::sin(State::horizontalAngle), State::camera->getDoubleZ() + std::tan(State::verticalAngle));

    //std::cout << "exit flag: " << this->exitFlag << "\n";
}

void CustomFrame::initGraphics() {
    //SDL init
    SDL_Init(SDL_INIT_VIDEO);

    // for 64 bit os
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // -------------

    this->window = SDL_CreateWindow("Custom Frame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->resolution->getIntX(), this->resolution->getIntY(), SDL_WINDOW_OPENGL);
    SDL_GL_CreateContext(this->window);
    SDL_ShowCursor(false);

    //screen
    this->screen = SDL_CreateRGBSurface(0, this->resolution->getIntX(), this->resolution->getIntY(), 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

    //ttf init
    TTF_Init();
    this->fonts->insert(std::pair<std::string, TTF_Font*>("mainfont", TTF_OpenFont("data/fonts/OpenSans-Bold.ttf", 12)));
    this->fonts->insert(std::pair<std::string, TTF_Font*>("smallfont", TTF_OpenFont("data/fonts/OpenSans-Bold.ttf", 7)));

    //GL init
    SDL_GL_SetSwapInterval(1);
    glClearColor(0.5, 0.5, 0.5, 1);

    //init depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glClearDepth(1);

    //init Lighting
    GLfloat LightAmbient[]= {0.9f, 0.9f, 0.9f, 1.0f}; // Значения фонового света ( НОВОЕ )
    GLfloat LightDiffuse[]= {1.0f, 1.0f, 1.0f, 1.0f}; // Значения диффузного света ( НОВОЕ )
    GLfloat LightPosition[]= {0.0f, -5.0f, 0, 1.0f};     // Позиция света ( НОВОЕ )

    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);    // Установка Фонового Света
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);    // Установка Диффузного Света
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);   // Позиция света
    glEnable(GL_LIGHT1);
    if(true) {
        glEnable(GL_LIGHTING);

        float ambient[4] = {0.5, 0.5, 0.5, 1};
        //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
        //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);


        GLfloat *lightAmbient = new GLfloat[3] {0.1f, 0.1f, 0.1f};
        GLfloat *lightDiffuse = new GLfloat[3] {1.0f, 1.0f, 1.0f};
        GLfloat *lightSpecular = new GLfloat[3] {0.5f, 0.5f, 0.5f};
        GLfloat *lightPosition = new GLfloat[4] {14.0f, -12.0f, 6.0f, 1.0f};

        State::lightPosition[0] = 14.0f;
        State::lightPosition[1] = -12.0f;
        State::lightPosition[2] = 6.0f;
        State::lightPosition[3] = 1.0f;

        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
        //glLightfv(GL_LIGHT0, GL_POSITION, State::lightPosition);
    }


    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);




    //GLfloat front_color[] = {1, 1, 1, 1};
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, front_color);



    //error-point

    Model *cube = Model::loadModel("data/models/cube_small.obj", "data/textures/cube.png");

    cube->getMesh()->setParameter(TTE_MESH_OPTION_RESET_COLOR, false);
    State::objects->push_back(cube);
    Model *pistol = Model::loadModel("data/models/m9.2.obj", "data/textures/m9.png");
    pistol->getMesh()->setParameter(TTE_MESH_OPTION_RESET_COLOR, false);
    State::objects->push_back(pistol);


    //load map
    State::map = new RawMap(IMG_Load("data/raw/m3.png"));
    State::mapTexture = Model::loadTexture("data/textures/t4.png");
}

void CustomFrame::render() {
    SDL_FillRect(this->screen, nullptr, 0x7f000000);

    SDL_LockSurface(this->screen);
    SPM_Circle(this->screen, 300, 60, 20, 0xfff86000);
    SPM_Line(this->screen, 300, 60, 300 + State::position->getIntX(), 60 + State::position->getIntY(), 0xfff86000);
    SDL_UnlockSurface(this->screen);


    SPM_TTF_BlendedText(this->screen, "mwl tps    : " + std::to_string(this->mwltps), this->fonts->at("mainfont"), 20, 40, {0, 220, 86, 128});
    SPM_TTF_BlendedText(this->screen, "gwl tps    : " + std::to_string(this->gwltps), this->fonts->at("mainfont"), 20, 60, {0, 220, 86, 128});
    SPM_TTF_BlendedText(this->screen, "ewl tps    : " + std::to_string(this->ewltps), this->fonts->at("mainfont"), 20, 80, {0, 220, 86, 128});

    SPM_TTF_BlendedText(this->screen, "exit flag  : " + std::to_string(this->exitFlag), this->fonts->at("mainfont"), 20, 120, {100, 220, 86, 100});
    SPM_TTF_BlendedText(this->screen, "pause flag : " + std::to_string(this->pauseFlag), this->fonts->at("mainfont"), 20, 140, {100, 220, 86, 100});

    SPM_TTF_BlendedText(this->screen, "horizontal : " + std::to_string(State::horizontalAngle), this->fonts->at("mainfont"), 20, 180, {240, 220, 86, 100});
    SPM_TTF_BlendedText(this->screen, "vertical   : " + std::to_string(State::verticalAngle), this->fonts->at("mainfont"), 20, 200, {240, 220, 86, 100});

    SPM_TTF_BlendedText(this->screen, "cx         : " + std::to_string(State::camera->getDoubleX()), this->fonts->at("mainfont"), 20, 220, {240, 220, 86, 100});
    SPM_TTF_BlendedText(this->screen, "cy         : " + std::to_string(State::camera->getDoubleY()), this->fonts->at("mainfont"), 20, 240, {240, 220, 86, 100});
    SPM_TTF_BlendedText(this->screen, "cz         : " + std::to_string(State::camera->getDoubleZ()), this->fonts->at("mainfont"), 20, 260, {240, 220, 86, 100});

    SPM_TTF_BlendedText(this->screen, "keys       : " + std::to_string(State::keyPressed[4]) + " " + std::to_string(State::keyPressed[5]), this->fonts->at("mainfont"), 20, 280, {240, 220, 86, 100});    
    SPM_TTF_BlendedText(this->screen, "derivetive : " + std::to_string(State::keyPressed[4]) + " " + std::to_string(State::keyPressed[5]), this->fonts->at("mainfont"), 20, 300, {240, 220, 86, 100});


    int consoleY = 300;
    SPM_TTF_BlendedText(this->screen, "c " + std::to_string(State::consoleOpen) + " : " + State::currentCommand, this->fonts->at("mainfont"), 20, consoleY, {240, 220, 86, 100});
    consoleY += 20;
    for(int i = State::consoleOut->size() - 1; i >= 0; i--)
    {
        std::string line = State::consoleOut->at(i);
        SPM_TTF_BlendedText(this->screen, line, this->fonts->at("mainfont"), 20, consoleY, {240, 220, 86, 100});
        consoleY += 16;
    }

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, this->resolution->getDoubleX() / this->resolution->getDoubleY(), 1, 3600);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); //reset translation maxrix (pohodu)

    gluLookAt(State::camera->getDoubleX(), State::camera->getDoubleY(), State::camera->getDoubleZ(), State::cameraView->getDoubleX(), State::cameraView->getDoubleY(), State::cameraView->getDoubleZ(), 0, 0, 1);

    glLightfv(GL_LIGHT0, GL_POSITION, State::lightPosition);


    //visualize model viev matrix
    GLfloat *modelViewMatrix = new GLfloat[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
    float *a;
    a = modelViewMatrix;
    SPM_TTF_BlendedText(this->screen, std::to_string(a[0]) + " " + std::to_string(a[1]) + " " + std::to_string(a[2]) + " " + std::to_string(a[3]), this->fonts->at("smallfont"), this->resolution->getIntX() - 200, 10, {255, 0, 255, 0});
    SPM_TTF_BlendedText(this->screen, std::to_string(a[4]) + " " + std::to_string(a[5]) + " " + std::to_string(a[6]) + " " + std::to_string(a[7]), this->fonts->at("smallfont"), this->resolution->getIntX() - 200, 20, {255, 0, 255, 0});
    SPM_TTF_BlendedText(this->screen, std::to_string(a[8]) + " " + std::to_string(a[9]) + " " + std::to_string(a[10]) + " " + std::to_string(a[11]), this->fonts->at("smallfont"), this->resolution->getIntX() - 200, 30, {255, 0, 255, 0});
    SPM_TTF_BlendedText(this->screen, std::to_string(a[12]) + " " + std::to_string(a[13]) + " " + std::to_string(a[14]) + " " + std::to_string(a[15]), this->fonts->at("smallfont"), this->resolution->getIntX() - 200, 40, {255, 0, 255, 0});


    //----------------------------

    /*
    float tmp_vector_x = State::lightPosition[0] * modelViewMatrix[0] + State::lightPosition[1] * modelViewMatrix[1] + State::lightPosition[2] * modelViewMatrix[2] + State::lightPosition[3] * modelViewMatrix[3];
    float tmp_vector_y = State::lightPosition[0] * modelViewMatrix[4] + State::lightPosition[1] * modelViewMatrix[5] + State::lightPosition[2] * modelViewMatrix[6] + State::lightPosition[3] * modelViewMatrix[7];
    float tmp_vector_z = State::lightPosition[0] * modelViewMatrix[8] + State::lightPosition[1] * modelViewMatrix[9] + State::lightPosition[2] * modelViewMatrix[10] + State::lightPosition[3] * modelViewMatrix[11];
    float tmp_vector_w = State::lightPosition[0] * modelViewMatrix[12] + State::lightPosition[1] * modelViewMatrix[13] + State::lightPosition[2] * modelViewMatrix[14] + State::lightPosition[3] * modelViewMatrix[15];

    State::lightPosition[0] = tmp_vector_x;
    State::lightPosition[1] = tmp_vector_y;
    State::lightPosition[2] = tmp_vector_z;
    State::lightPosition[3] = tmp_vector_w;

    //------------------------------

*/


    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 0.51f);
    for (Model *model : *State::objects) {
       if(model != nullptr) model->render();
    }



    glTranslatef(28.0f, 93.0f, 15.0f);
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0, 2.0f, 1);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(1.2f, 2.0f, -1);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-1.2f, 2.0f, -1);
    glEnd();
    glTranslatef(-28.0f, -93.0f, -15.0f);





    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, *State::mapTexture);
    glColor3f(1.0f, 1.0f, 1.0f);
    if(State::map) State::map->renderHeightMap();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, this->resolution->getIntX(), 0, this->resolution->getIntY(), -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRasterPos2i(0, this->resolution->getIntY());
    glPixelZoom(1.0f, -1.0f);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);
    glDrawPixels(this->screen->w, this->screen->h, GL_RGBA, GL_UNSIGNED_BYTE, this->screen->pixels);
    glEnable(GL_DEPTH_TEST);


    SDL_GL_SwapWindow(window);

    //glReadBuffer(GL_FRONT);
    //glAccum(GL_MULT, 1.1f);
    //glDrawBuffer(GL_FRONT);
    //Debug::log("render ended");
}

void CustomFrame::handleEvent(SDL_Event *event)
{
    std::cout << "begin\n";
    State::stateText = "";
    if(event->type == SDL_KEYDOWN) {

        std::cout << "  key d " << event->key.keysym.scancode << "\n";

        State::stateText += "[key down] ";
        if(!State::consoleOpen) {
            if(event->key.keysym.scancode == SDL_SCANCODE_W) State::keyPressed[0] = true;
            if(event->key.keysym.scancode == SDL_SCANCODE_S) State::keyPressed[1] = true;
            if(event->key.keysym.scancode == SDL_SCANCODE_A)  State::keyPressed[2] = true;
            if(event->key.keysym.scancode == SDL_SCANCODE_D)  State::keyPressed[3] = true;
            if(event->key.keysym.scancode == SDL_SCANCODE_SPACE)  State::keyPressed[4] = true;
            if(event->key.keysym.scancode == SDL_SCANCODE_C)  State::keyPressed[5] = true;
            if(event->key.keysym.scancode == SDL_SCANCODE_LSHIFT)  State::keyPressed[6] = true;
        }
        else {
            if(event->key.keysym.scancode == SDL_SCANCODE_RETURN) {
                if(State::currentCommand != ""){
                    State::commandQueue->push(State::currentCommand);
                    State::currentCommand = "";
                }
            }
            else if (event->key.keysym.scancode != SDL_SCANCODE_GRAVE){
                State::currentCommand += (char)event->key.keysym.sym;
            }
        }
    }
    else if(event->type == SDL_KEYUP) {
        std::cout << "  key u\n";
        State::stateText += "[key up] ";
        if(!State::consoleOpen) {
            if(event->key.keysym.scancode == SDL_SCANCODE_W) State::keyPressed[0] = false;
            if(event->key.keysym.scancode == SDL_SCANCODE_S) State::keyPressed[1] = false;
            if(event->key.keysym.scancode == SDL_SCANCODE_A)  State::keyPressed[2] = false;
            if(event->key.keysym.scancode == SDL_SCANCODE_D)  State::keyPressed[3] = false;
            if(event->key.keysym.scancode == SDL_SCANCODE_SPACE)  State::keyPressed[4] = false;
            if(event->key.keysym.scancode == SDL_SCANCODE_C)  State::keyPressed[5] = false;
            if(event->key.keysym.scancode == SDL_SCANCODE_LSHIFT)  State::keyPressed[6] = false;
        }
        if(event->key.keysym.scancode == SDL_SCANCODE_GRAVE) State::consoleOpen = !State::consoleOpen;
    }
    else if(event->type == SDL_MOUSEMOTION && !State::consoleOpen) {
        std::cout << "  mouse\n";
        State::stateText += "[mouse] ";

        Vector *mouseOffset = this->mouse->subtract(this->resolution->multiply(0.5))->multiply(-0.001);
        SDL_WarpMouseInWindow(this->window, this->resolution->getIntX() / 2, this->resolution->getIntY() / 2);

        State::horizontalAngle += mouseOffset->getDoubleX();
        State::verticalAngle += mouseOffset->getDoubleY();

        const float trim = 0.000000001f;
        if(State::verticalAngle >= SPM_PI * 3 / 2) {State::verticalAngle = SPM_PI * 3 / 2;}
        else if(State::verticalAngle <= SPM_PI / 2 + trim) {State::verticalAngle = SPM_PI / 2 + trim;}
    }
    std::cout << "end\n";
    if(this->screen != nullptr)
        SPM_TTF_BlendedText(this->screen, State::stateText, this->fonts->at("mainfont"), this->resolution->getIntX() - 300, 80, {255, 0, 255, 0});

}
