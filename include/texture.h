#ifndef TEXTURE_H
#define TEXTURE_H
#include <stdint.h>

unsigned int generateTexture2D(char *texturePath, int TextureWrapS,
                               int TextureWrapT, int TextureFilterMin,
                               int TextureFilterMag);
#endif // TEXTURE_H
