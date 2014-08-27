/*
 * DiffuseMaterial.h
 *
 * A material with support for one diffuse texture.
 */
#ifndef DIFFUSEMATERIAL_H_
#define DIFFUSEMATERIAL_H_

#include "BaseMaterial.h"

class DiffuseMaterial : public BaseMaterial
{
public:
    DiffuseMaterial(String materialId,
            String vertexShader,
            String fragmentShader,
            String diffuseTexturePath);

    virtual ~DiffuseMaterial();

    /* Texture loading. */
    void loadTextureBuffers();
    const GLuint& getDiffuseTextureBuffer() const;

private:
    String _diffuseTexturePath;

    /*
     * Image data to be loaded into texture object when material is constructed.
     * Texture objects will be loaded and cached by ResMan, but as we're using
     * GLFW to directly load the image texture, this won't be needed for now.
     */
    // std::shared_ptr<Texture> tex_diffuse;

    /* Diffuse texture buffer object. */
    GLuint _diffuseTextureBuffer;
};

#endif /* DIFFUSEMATERIAL_H_ */
