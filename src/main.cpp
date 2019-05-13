
#include "customframe.h"
#include "3te/debug.h"
#include "state.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL_image.h>
#include <iostream>

using namespace std;
using namespace burbokop::tte;


int main() {
    //Debug::enableLog();
    Frame *frame = new CustomFrame();
    frame->startAll();
    return 0;
}
