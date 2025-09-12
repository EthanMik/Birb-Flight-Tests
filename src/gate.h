#pragma once

#include "globals.h"

class Gate 
{
public:
    Gate(int x, int y, int w, int h, Texture2D* texture);
    void Draw();
    void Open();
};