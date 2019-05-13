#ifndef RAWMAP_H
#define RAWMAP_H

#include <SDL2/SDL.h>

class RawMap
{
private:
    char *data;

    int width;
    int height;

    int interval;
    float maxLevel;

public:
    RawMap(const char *path, int width, int height);
    RawMap(SDL_Surface *map);

    //void LoadRawFile(const char *strName, int nSize, char *pHeightMap);
    //void convertToRaw(SDL_Surface *surface, int nSize, char *pHeightMap);
    //int Height(char *pHeightMap, int X, int Y);
    //void SetVertexColor(char *pHeightMap, int x, int y);
    float getLevel(int x, int y);
    void renderHeightMap();
    char *getData();
    float getMaxLevel();
};

#endif // RAWMAP_H
