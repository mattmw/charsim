/*
 * BaseMaterial.h
 *
 * A material providing basic shader-loading, but no texture support.
 */
#ifndef BASEMATERIAL_H_
#define BASEMATERIAL_H_

#include "Material.h"

class BaseMaterial : public Material
{
public:
    BaseMaterial(String materialId, String vertexShader, String fragmentShader);

    BaseMaterial(String materialId,
            String vertexShader,
            String geometryShader,
            String fragmentShader);

    virtual ~BaseMaterial();

    /* Access to program for Renderer. */
    const GLuint& getProgram() const;

private:
    /*
     * createShader, createProgram and initProgram are based
     * on the OpenGL Utility Library's program compilation/linking code:
     * http://glsdk.sourceforge.net/docs/html/group__module__glutil__program.html
     * See LICENSE.txt
     */
    GLuint createShader(GLenum eShaderType, std::string &shaderFile);
    GLuint createProgram(std::vector<GLuint> &shaderList);
    void initProgram();
};

#endif /* BASEMATERIAL_H_ */
