#include "state.h"

using namespace burbokop;

//2d
Vector *State::position = new Vector();
double State::ticks = 0;
bool State::mousePressed = false;
double State::scale = 10;
double State::rotate = 0;

//palyer
bool *State::keyPressed = new bool[16];
Vector3d *State::camera = new Vector3d();
Vector3d *State::cameraView = new Vector3d(0, 1, 0);
double State::horizontalAngle = SPM_PI / 2;
double State::verticalAngle = 0;

void State::handleCollision(Vector3d *position, RawMap *map)
{
    double level = map->getLevel(position->getIntX(), position->getIntY()) * map->getMaxLevel() + 2;
    if(position->getDoubleZ() <= level) {
        position->setZ(level);
    }
}

//world
bool State::gravity = false;
bool State::collision = false;
float *State::lightPosition = new float[4];

//console
bool State::consoleOpen = false;
std::string State::currentCommand = "";
std::queue<std::string> *State::commandQueue = new std::queue<std::string>();
std::vector<std::string> *State::consoleOut = new std::vector<std::string>();
Timer *State::existenceTimer = new Timer(2000);

//objects
std::vector<Model*> *State::objects = new std::vector<Model*>();
//map
RawMap *State::map;
GLuint *State::mapTexture;


//util
std::string State::stateText = "none";
