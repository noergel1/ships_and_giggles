#pragma once
#include <vector>
#include "definitions.h"


class DataProvider {
public:
    // functions return the vertices & indices defined in the corresponding .cpp
    static const std::vector<VertexData> getCubeVertices();
    static const std::vector<unsigned int> getCubeIndices();
    static const unsigned int getCubeIndiceSize();



    static const std::string getShipObjPath();


private:
    static const std::vector<VertexData> cubeVertices;
    static const std::vector<unsigned int> cubeIndices;
};