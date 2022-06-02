#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h>
#include <glm/ext.hpp>
#include "glm/gtc/matrix_transform.hpp"


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    static unsigned int createShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
    {
        unsigned int shaderId = 0;

        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try 
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();		
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();			
            // if geometry shader path is present, also load a geometry shader
            if(geometryPath != nullptr)
            {
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // if geometry shader is given, compile geometry shader
        unsigned int geometry;
        if(geometryPath != nullptr)
        {
            const char * gShaderCode = geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            checkCompileErrors(geometry, "GEOMETRY");
        }
        // shader Program
        shaderId = glCreateProgram();
        glAttachShader(shaderId, vertex);
        glAttachShader(shaderId, fragment);
        if(geometryPath != nullptr)
            glAttachShader(shaderId, geometry);
        glLinkProgram(shaderId);
        checkCompileErrors(shaderId, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if(geometryPath != nullptr)
            glDeleteShader(geometry);

        return shaderId;
    }
    // activate the shader
    // ------------------------------------------------------------------------
    static bool useShader(const unsigned int _id) 
    { 
        glUseProgram(_id); 

        return true;
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    static void setBool(const unsigned _id, const std::string& name, bool value)
    {
        unsigned int uniformId = glGetUniformLocation(_id, name.c_str());
        if (uniformId == -1)
            std::cout << "Couldnt find boolean Uniform " + name + "!\n";
        else
            glUniform1i(uniformId, (int)value);
    }

    static void setInt(const unsigned int _id, const std::string& name, int value)
    {
        unsigned int uniformId = glGetUniformLocation(_id, name.c_str());
        if (uniformId == -1)
            std::cout << "Couldnt find int Uniform " + name + "!\n";
        else
            glUniform1i(uniformId, value);
    }

    static void setFloat(const unsigned int _id, const std::string& name, float value)
    {
        unsigned int uniformId = glGetUniformLocation(_id, name.c_str());
        if (uniformId == -1)
            std::cout << "Couldnt find float Uniform " + name + "!\n";
        else
            glUniform1f(uniformId, value);
    }

    static void setVec3(const unsigned int _id, const std::string& name, glm::vec3 vector)
    {
        unsigned int uniformId = glGetUniformLocation(_id, name.c_str());

        if (uniformId == -1)
            std::cout << "Couldnt find vec3 Uniform " + name + "!\n";
        else
            glUniform3f(uniformId, vector.x, vector.y, vector.z);

    }

    static void setVec3(const unsigned int _id, const std::string& name, float x, float y, float z)
    {
        unsigned int uniformId = glGetUniformLocation(_id, name.c_str());
        glm::vec3 vector = glm::vec3(x, y, z);
        if (uniformId == -1)
            std::cout << "Couldnt find vec3 Uniform " + name + "!\n";
        else
        {
            glUniform3f(uniformId, vector.x, vector.y, vector.z);
        }
    }

    static void setVec4(const unsigned int _id, const std::string& name, float x, float y, float z, float w)
    {
        unsigned int uniformId = glGetUniformLocation(_id, name.c_str());
        if (uniformId == -1)
            std::cout << "Couldnt find vec4 Uniform " + name + "!\n";
        else
            glUniform4f(uniformId, x, y, z, w);

    }

    static void setMat4(const unsigned int _id, const std::string& name, glm::mat4 matrix)
    {
        unsigned int uniformId = glGetUniformLocation(_id, name.c_str());
        if (uniformId == -1)
            std::cout << "Couldnt find mat4 Uniform " + name + "!\n";
        else
            glUniformMatrix4fv(uniformId, 1, GL_FALSE, glm::value_ptr(matrix));
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    static void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
#endif