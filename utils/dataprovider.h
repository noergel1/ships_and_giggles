#pragma once
#include <vector>
#include "definitions.h"
#include "helpers.h"
#include "../models/model_data.h"


class DataProvider {
public:
    // functions return the vertices & indices defined in the corresponding .cpp
    static const std::vector<VertexData> getCubeVertices();
    static const std::vector<unsigned int> getCubeIndices();
    static const unsigned int getCubeIndiceSize();

    // functions return the vertices & indices defined in the corresponding .cpp
    static const std::vector<VertexData> getPlaneVertices();
    static const std::vector<unsigned int> getPlaneIndices();
    static const unsigned int getPlaneIndiceSize();

    static const std::vector<VertexData> generatePlaneVertices( glm::vec3 _v0, glm::vec3 _v1, glm::vec3 _v2, glm::vec3 _v3, unsigned int _div );
    static const std::vector<unsigned int> generatePlaneIndices( unsigned int _div );

    // vertices for a quad that fille the entire screen in Normalized Device Coordinates
    static const std::vector<float> getQuadVertices();
    static const std::vector<unsigned int> getQuadIndices();
    static const unsigned int getQuadIndiceSize();

    static const std::string getShipObjPath();

    // kernels for postprocessing
    static std::vector<float> noPostProcessing();
    static std::vector<float> getSharpenKernel();
    static std::vector<float> getBlurKernel();
    static std::vector<float> getEdgeDetectionKernel();



private:
    static const std::vector<VertexData> cubeVertices;
    static const std::vector<unsigned int> cubeIndices;

    static const std::vector<VertexData> planeVertices;
    static const std::vector<unsigned int> planeIndices;
};