#ifndef STATE_H
#define STATE_H

#include <rawmap.h>

#include <3te/3te_main_header.h>
#include <3te/mesh.h>
#include <3te/model.h>
#include <vector>
#include <queue>

using namespace burbokop;
using namespace burbokop::tte;
using namespace burbokop::utilities3d;

class State
{
public:
    //2d
    static Vector *position;
    static double ticks;
    static bool mousePressed;
    static double scale;
    static double rotate;


    //player
    static bool *keyPressed;
    static Vector3d *camera;
    static Vector3d *cameraView;
    static double horizontalAngle;
    static double verticalAngle;
    static void handleCollision(Vector3d *position, RawMap *map);

    //world
    static bool gravity;
    static bool collision;
    static float *lightPosition;

    //console
    static bool consoleOpen;
    static std::string currentCommand;
    static std::queue<std::string> *commandQueue;
    static std::vector<std::string> *consoleOut;
    static Timer *existenceTimer;

    //objects
    static std::vector<Model*> *objects;
    //map
    static RawMap *map;
    static GLuint *mapTexture;

    //util
    static std::string stateText;
};

#endif // STATE_H
