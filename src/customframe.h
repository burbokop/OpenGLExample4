#ifndef CUSTOMFRAME_H
#define CUSTOMFRAME_H

#include <3te/frame.h>
#include <3te/timer.h>
#include <3te/vector.h>


#include <SDL2/SDL_ttf.h>


class CustomFrame : public burbokop::tte::Frame
{
public:
    CustomFrame();

protected:

    virtual void gameLoop();
    virtual void initGame();
    virtual void initGraphics();
    virtual void render();
    virtual void handleEvent(SDL_Event *event);
};

#endif // CUSTOMFRAME_H
