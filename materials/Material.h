/*
 * Material.h
 *
 * Abstract class for materials.
 *
 *      Author: MMW
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "framework/core.h"

class Material
{
public:
    Material(String materialId, String vertexShader, String fragmentShader)
            : _materialId{materialId},
              _vertexShader{vertexShader},
              _fragmentShader{fragmentShader} {};

    Material(String materialId,
            String vertexShader,
            String geometryShader,
            String fragmentShader)
            : _materialId{materialId},
              _vertexShader{vertexShader},
              _geometryShader{geometryShader},
              _fragmentShader{fragmentShader} {};

    virtual ~Material() {};

    /* Called by model nodes to get program for the Renderer. */
    virtual const GLuint& getProgram() const = 0;

protected:
    String _materialId;

    String _vertexShader;
    String _geometryShader;
    String _fragmentShader;

    /* Array of shaders for compiling into program. */
    std::vector<GLuint> _shaderList;
    /* Shader program. */
    GLuint _program = 0;

    virtual GLuint createShader(GLenum eShaderType, String &shaderFile) = 0;
    virtual GLuint createProgram(std::vector<GLuint> &shaderList) = 0;
    virtual void initProgram() = 0;
};

#endif /* MATERIAL_H_ */
