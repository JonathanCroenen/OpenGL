#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{
    public:
        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath){
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vShaderFile(vertexPath);
            std::ifstream fShaderFile(fragmentPath);

            vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
            
            try
            {
                vertexCode.assign( (std::istreambuf_iterator<char>(vShaderFile) ),
                       (std::istreambuf_iterator<char>()    ) );

                fragmentCode.assign( (std::istreambuf_iterator<char>(fShaderFile) ),
                       (std::istreambuf_iterator<char>()    ) );
            }
            catch(std::ifstream::failure e)
            {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
            }
            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();

            unsigned int vertex, fragment;
            int success;
            char infoLog[512];

            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);

            checkCompileErrors(vertex, "VERTEX");

            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);

            checkCompileErrors(fragment, "FRAGMENT");

            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);

            checkCompileErrors(ID, "PROGRAM");
            
            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        void use(){
            glUseProgram(ID);
        }

        void setBool(const char* name, bool value) const{
            glUniform1i(glGetUniformLocation(ID, name), (int)value);
        }
        void setInt(const char* name, int value) const{
            glUniform1i(glGetUniformLocation(ID, name), value);
        }
        void setFloat(const char* name, float value) const{
            glUniform1f(glGetUniformLocation(ID, name), value);
        }


        void setVec2(const char* name, glm::vec2 value) const{
            glUniform2fv(glGetUniformLocation(ID, name), 1, &(value.x));
        }
        void setVec2(const char* name, float x, float y) const{
            glUniform2f(glGetUniformLocation(ID, name), x, y);
        }

        void setVec3(const char* name, glm::vec3 value) const{
            glUniform3fv(glGetUniformLocation(ID, name), 1, &(value.x));
        }
        void setVec3(const char* name, float x, float y, float z) const{
            glUniform3f(glGetUniformLocation(ID, name), x, y, z);
        }

        void setVec4(const char* name, glm::vec4 value) const{
            glUniform4fv(glGetUniformLocation(ID, name), 1, &(value.x));
        }
        void setVec4(const char* name, float x, float y, float z, float w) const{
            glUniform4f(glGetUniformLocation(ID, name), x, y, z, w);
        }


        void setMat2(const char* name, glm::mat2 value) const{
            glUniformMatrix2fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &(value[0].x));
        }
        void setMat3(const char* name, glm::mat3 value) const{
            glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &(value[0].x));
        }
        void setMat4(const char* name, glm::mat4 value) const{
            glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &(value[0].x));
        }
    private:
        void checkCompileErrors(GLuint shader, std::string type){
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM"){
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success){
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::" << type << "::SHADER::COMPILATION_ERROR" << "\n" << infoLog << "\n --------------------------------------------------------- " << std::endl;
            }
        }else{
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success){
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER::" << type << "::LINKING_ERROR"<< "\n" << infoLog << "\n --------------------------------------------------------- " << std::endl;
            }
        }
    }
};

#endif
