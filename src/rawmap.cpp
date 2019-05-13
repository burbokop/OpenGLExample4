#include "rawmap.h"

#include <iostream>
#include <GL/gl.h>
#include <3te/vector3d.h>


using namespace burbokop::utilities3d;

RawMap::RawMap(const char *path, int width, int height)
{
    this->width = width;
    this->height = height;

    this->interval = 16;
    this->maxLevel = 0.2;

    this->data = new char[width * height];

    FILE *pFile = nullptr;
    pFile = fopen(path, "rb");

    if (pFile == nullptr) {
        std::cout << "Can't Find The Height Map!\n";
        return;
    }

    fread(this->data, 1, width * height, pFile);

    int result = ferror(pFile);
    if (result) {
        std::cout << "Failed To Get Data!\n";
    }
    fclose(pFile);
}

RawMap::RawMap(SDL_Surface *map)
{
    this->width = map->w;
    this->height = map->h;

    this->interval = 2;
    this->maxLevel = 32;

    int arraySize = this->width * this->height;
    this->data = new char[arraySize];
    if(SDL_MUSTLOCK(map))
    {
        SDL_LockSurface(map);
    }


    if(map->format->BytesPerPixel == 1){
        printf("map convertion 8 bit\n");
        Uint8 *pixels = (Uint8*)map->pixels;
        for (int i = 0; i < arraySize; i++) {
            Uint8 pixel = pixels[i];
            data[i] = pixel;
        }
    }
    else if(map->format->BytesPerPixel == 2) {
        printf("map convertion 16 bit\n");
        Uint16 *pixels = (Uint16*)map->pixels;
        for (int i = 0; i < arraySize; i++) {
            Uint16 pixel = pixels[i];
            data[i] = pixel;
        }
    }
    else if(map->format->BytesPerPixel == 4) {
        printf("map convertion 32 bit\n");
        Uint32 *pixels = (Uint32*)map->pixels;
        for (int i = 0; i < arraySize; i++) {
            Uint32 pixel = pixels[i];
            data[i] = pixel;
        }
    }
    else {
        printf("map convertion (format: %d)\n", map->format->BytesPerPixel);
    }

    if(SDL_MUSTLOCK(map))
    {
        SDL_UnlockSurface(map);
    }
}


float RawMap::getLevel(int x, int y) {
    float result = this->data[x + this->width * y];
    result = result / 256;
    return result;
}

void RawMap::renderHeightMap()
{
    glBegin(GL_QUADS);

    for (int y = 0; y < this->height; y += this->interval) {
        for (int x = 0; x < this->width; x += this->interval) {
            float z = 0.0f;

            //float trim = 0.95;
            float trim = 1;

            /*
            Vector3d *poligon_v1 = new Vector3d((float)x, (float)y, this->getLevel(x, y));
            Vector3d *poligon_v2 = new Vector3d((float)(x + this->interval), (float)y, this->getLevel(x + this->interval, y));
            Vector3d *poligon_v3 = new Vector3d((float)(x + this->interval), (float)(y + this->interval), this->getLevel(x + this->interval, y + this->interval));

            Vector3d *normal = poligon_v1->subtract(poligon_v2)->multiply(poligon_v2->subtract(poligon_v3))->normalized()->multiply(-1);
            */

            //glNormal3d(normal->getDoubleX(), normal->getDoubleY(), normal->getDoubleZ());

            z = this->getLevel(x, y);
            glColor3f(z, 0.1f, 0.1f);
            glTexCoord2d(0, 0);
            z = z * this->maxLevel;
            glVertex3d(x, y, z);

            z = this->getLevel(x + this->interval, y);
            glColor3f(z, 0.1f, 0.1f);
            glTexCoord2d(1, 0);
            z = z * this->maxLevel;
            glVertex3d(x + this->interval * trim, y, z);

            z = this->getLevel(x + this->interval, y + this->interval);
            glColor3f(z, 0.1f, 0.1f);
            glTexCoord2d(1, 1);
            z = z * this->maxLevel;
            glVertex3d(x + this->interval * trim, y + this->interval * trim, z);

            z = this->getLevel(x, y + this->interval);
            glColor3f(z, 0.1f, 0.1f);
            glTexCoord2d(0, 1);
            z = z * this->maxLevel;
            glVertex3d(x, y + this->interval * trim, z);
        }
    }
    glEnd();
    glFlush();
}




/*
char *State::heightMap = new char[MAP_SIZE * MAP_SIZE];
void State::LoadRawFile(const char *strName, int nSize, char *pHeightMap)
{
    FILE *pFile = NULL;
    pFile = fopen( strName, "rb" );

    if ( pFile == NULL ) {
        std::cout << "Can't Find The Height Map!\n";
        return;
    }

    fread( pHeightMap, 1, nSize, pFile);

    int result = ferror(pFile);
    if (result) {
        std::cout << "Failed To Get Data!\n";
    }
    fclose(pFile);
}

void State::convertToRaw(SDL_Surface *surface, int nSize, char *pHeightMap)
{
    int max = 0;
    if(surface->w * surface->h >= nSize) max = nSize;
    else max = surface->w * surface->h;

    printf("%d . %d . %d\n", nSize, surface->w * surface->h, max);

    Uint32 *pixels = (Uint32*)(surface->pixels);
    for (int i = 0; i < max; i++) {
        int val = 0;
        val = pixels[i];
        printf("%x . %d\n", val, i);
        //std::cout << val << " | " << i << "\n";
        pHeightMap[i] = val;
    }
}


int State::Height(char *pHeightMap, int X, int Y) {
    //int x = X % MAP_SIZE;
    //int y = Y % MAP_SIZE;

    int x = X;
    int y = Y;

    if(!pHeightMap) return 0;
    return pHeightMap[x + (y * MAP_SIZE)];
}


void State::SetVertexColor(char *pHeightMap, int x, int y) {
    if(!pHeightMap) return;
    float fColor = -0.15f + (Height(pHeightMap, x, y ) / 256.0f);

    glColor3f(fColor, 0.05f, fColor);
}


void State::RenderHeightMap(char *pHeightMap) {
    int X = 0, Y = 0;
    int x, y, z;

    if(!pHeightMap) return;

    bool bRender = true;
    if(bRender)
        glBegin(GL_QUADS);
    else
        glBegin(GL_LINES);

    for (X = 0; X < MAP_SIZE; X += STEP_SIZE) {
        for (Y = 0; Y < MAP_SIZE; Y += STEP_SIZE) {
            x = X;
            y = Y;
            z = Height(pHeightMap, X, Y) * HEIGHT_RATIO;

            SetVertexColor(pHeightMap, x, y);
            glVertex3i(x, y, z);


            x = X;
            y = Y + STEP_SIZE ;
            z = Height(pHeightMap, X, Y + STEP_SIZE ) * HEIGHT_RATIO;

            SetVertexColor(pHeightMap, x, y);
            glVertex3i(x, y, z);


            x = X + STEP_SIZE;
            y = Y + STEP_SIZE ;
            z = Height(pHeightMap, X + STEP_SIZE, Y + STEP_SIZE ) * HEIGHT_RATIO;

            SetVertexColor(pHeightMap, x, y);
            glVertex3i(x, y, z);


            x = X + STEP_SIZE;
            y = Y;
            z = Height(pHeightMap, X + STEP_SIZE, Y ) * HEIGHT_RATIO;


            SetVertexColor(pHeightMap, x, y);
            glVertex3i(x, y, z);
        }
    }

    glEnd();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

*/

char *RawMap::getData()
{
    return data;
}

float RawMap::getMaxLevel()
{
    return maxLevel;
}
