#include <glad/glad.h>
#include <stdio.h>
#include "stb_image.h"
#include "texture.h"

unsigned int generateTexture2D(char *texturePath, int TextureWrapS,
                               int TextureWrapT, int TextureFilterMin,
                               int TextureFilterMag) {

  stbi_set_flip_vertically_on_load(1);
  int width, height, nrChannels;
  unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
  if (!data) {
    printf("Unabel to gen tex");
    return 0;
  }

  GLenum format = GL_RGB;
  if (nrChannels == 1) {
    format = GL_RED;
  }
  if (nrChannels == 3) {
    format = GL_RGB;
  }
  if (nrChannels == 4) {
    format = GL_RGBA;
  }

  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  // set params
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TextureWrapS);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TextureWrapT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TextureFilterMin);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureFilterMag);

  stbi_image_free(data);
  return texture;
}
