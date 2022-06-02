#include "model_data.h"


ModelData::ModelData(unsigned int _vao, unsigned int _indiceCount, ShaderReference _shader, std::vector<Texture> _textures, float _shininess)
    : m_VAO(_vao)
    , m_indiceCount(_indiceCount)
    , m_Shader(_shader)
    , m_textures(_textures)
    , m_shininess(_shininess)
{
}


std::vector<Texture> ModelData::getTextures()
{
    return m_textures;
}

float ModelData::getShininess()
{
    return m_shininess;
}
