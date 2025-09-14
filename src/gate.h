#pragma once

#include "globals.h"

class Gate 
{
public:
    Gate(float x, float y, Texture2D* texture);

    void Draw();

    void Open();
    void Close();

private:
    float x;
    float y; 
    Texture2D* texture;

    // Timer timer;
    int alpha{255};

    bool opened;
};