#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <image_loader/stb_image.h>
#include <iostream>

class Texture{
    public:
        unsigned int ID;
        int width, height, nrChannels;

        Texture(const char* texturePath, GLint type, GLint wrapS = GL_REPEAT, GLint wrapT = GL_REPEAT, 
                GLint magFilter = GL_LINEAR, GLint minFilter = GL_LINEAR_MIPMAP_LINEAR){

            glGenTextures(1, &ID);
            glBindTexture(GL_TEXTURE_2D, ID);

            glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
            glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
            glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
            glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
            
            stbi_set_flip_vertically_on_load(true);

            unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
            if(data){
                glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D); 
            }else{
                std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD" << std::endl;
            }
            stbi_image_free(data);
        }

        void use(GLint location = GL_TEXTURE0){
            glActiveTexture(location);
            glBindTexture(GL_TEXTURE_2D, ID);
        }
};


#endif