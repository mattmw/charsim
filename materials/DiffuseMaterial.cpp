/*
 * DiffuseMaterial.cpp
 */
#include "DiffuseMaterial.h"

DiffuseMaterial::DiffuseMaterial(String materialId,
        String vertexShader,
        String fragmentShader,
        String diffuseTexturePath)
        : BaseMaterial(materialId, vertexShader, fragmentShader),
          _diffuseTexturePath{diffuseTexturePath},
          _diffuseTextureBuffer{0}
{
}

DiffuseMaterial::~DiffuseMaterial()
{
    std::cout << "\nDestructing DiffuseMaterial: \"" << _materialId << "\"" << std::endl;
}

#define FOURCC_DXT1 0x31545844
#define FOURCC_DXT3 0x33545844
void DiffuseMaterial::loadTextureBuffers()
{
    /*
     * loadTextureBuffers is based on OpenGL-tutorial's DDS loading code:
     * http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/#Compressed_Textures
     * See LICENSE.txt
     */

    unsigned char header[124];

    FILE *fp;

    const char* imagepath = _diffuseTexturePath.c_str();

    /* try to open the file */
    fp = fopen(imagepath, "rb");
    if (fp == NULL)
    {
        printf("%s could not be opened. Are you in the right directory?\n", imagepath);
        getchar();
    }

    /* verify the type of file */
    char filecode[4];
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "DDS ", 4) != 0)
    {
        fclose(fp);
        cout<<"Error: Image is not DDS."<<endl;
    }

    /* get the surface desc */
    fread(&header, 124, 1, fp);

    unsigned int height      = *(unsigned int*)&(header[8 ]);
    unsigned int width       = *(unsigned int*)&(header[12]);
    unsigned int linearSize  = *(unsigned int*)&(header[16]);
    unsigned int mipMapCount = *(unsigned int*)&(header[24]);
    unsigned int fourCC      = *(unsigned int*)&(header[80]);


    unsigned char * buffer;
    unsigned int bufsize;
    /* how big is it going to be including all mipmaps? */
    bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
    fread(buffer, 1, bufsize, fp);
    /* close the file pointer */
    fclose(fp);

    unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
    unsigned int format;
    switch(fourCC)
    {
        case FOURCC_DXT1:
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            cout<<"DXT1 discovered"<<endl;
            break;
        case FOURCC_DXT3:
            format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            break;
        default:
            free(buffer);
    }

    /* Create one OpenGL texture */
    GLuint textureID;
    glGenTextures(1, &textureID);

    /* "Bind" the newly created texture : all future texture functions will modify this texture */
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;

    /* load the mipmaps */
    for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
    {
        unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
                0, size, buffer + offset);

        offset += size;
        width  /= 2;
        height /= 2;

        /* Deal with Non-Power-Of-Two textures. */
        if(width < 1) width = 1;
        if(height < 1) height = 1;

    }

    free(buffer);

    _diffuseTextureBuffer = textureID;
}

const GLuint& DiffuseMaterial::getDiffuseTextureBuffer() const
{
    return _diffuseTextureBuffer;
}
