/*
 * BaseMaterial.cpp
 */
#include "BaseMaterial.h"

BaseMaterial::BaseMaterial(String materialId,
        String vertexShader,
        String fragmentShader)
        : Material(materialId, vertexShader, fragmentShader)
{
    initProgram();
}

BaseMaterial::BaseMaterial(String materialId,
        String vertexShader,
        String geometryShader,
        String fragmentShader)
        : Material(materialId, vertexShader, geometryShader, fragmentShader)
{
    initProgram();
}

BaseMaterial::~BaseMaterial()
{
    std::cout << "\nDestructing BaseMaterial: \"" << _materialId << "\"" << std::endl;
}

GLuint BaseMaterial::createShader(GLenum eShaderType, String &shaderFile)
{
    const GLuint shader = glCreateShader(eShaderType);
    const char *shaderFileData = shaderFile.c_str();
    glShaderSource(shader, 1, &shaderFileData, NULL);

    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *infoLog = new GLchar[infoLogLength + 1];

        glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);

        String strInfoLog = infoLog;
        delete[] infoLog;

        const char *shaderType = NULL;
        switch(eShaderType)
        {
            case GL_VERTEX_SHADER: shaderType = "vertex"; break;
            case GL_GEOMETRY_SHADER: shaderType = "geometry"; break;
            case GL_FRAGMENT_SHADER: shaderType = "fragment"; break;
        }

        throw ShaderException(shaderType, strInfoLog);
    }

    return shader;
}

GLuint BaseMaterial::createProgram(std::vector<GLuint> &shaderList)
{
    const GLuint program = glCreateProgram();

    for (uint i = 0; i < shaderList.size(); i++)
        glAttachShader(program, shaderList[i]);

    glLinkProgram(program);

    GLint status;
    glGetProgramiv (program, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *infoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
        String strInfoLog = infoLog;
        delete[] infoLog;

        throw InitException(strInfoLog);
    }

    for(uint i = 0; i < shaderList.size(); i++)
        glDetachShader(program, shaderList[i]);

    return program;
}

void BaseMaterial::initProgram()
{
    if (!_vertexShader.empty())
        _shaderList.push_back(createShader(GL_VERTEX_SHADER, _vertexShader));
    if (!_geometryShader.empty())
        _shaderList.push_back(createShader(GL_GEOMETRY_SHADER, _geometryShader));
    if (!_fragmentShader.empty())
        _shaderList.push_back(createShader(GL_FRAGMENT_SHADER, _fragmentShader));

    _program = createProgram(_shaderList);

    std::for_each(_shaderList.begin(), _shaderList.end(), glDeleteShader);
}

const GLuint& BaseMaterial::getProgram() const
{
    return _program;
}
