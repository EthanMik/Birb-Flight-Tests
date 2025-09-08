#pragma once

#include "globals.h"

class Map 
{
public:
    Map();
    ~Map();

    void Generate();
    void Update();
    void Draw();

private:
    std::vector<Animal> animals;
};