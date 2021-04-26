#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <cstdlib>
#include <stb\stb_image.h>
#include "gl.h"

class Texture
{
public:
    unsigned int texture_unit;
    int texture_uniform;

    Texture() {}
    Texture(const char* image_filename, unsigned int texture_unit);
};

#endif // _TEXTURE_H