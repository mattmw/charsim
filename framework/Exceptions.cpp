/*
 * Exceptions.cpp
 *
 *      Author: MMW
 */

#include "Exceptions.h"

/* TODO: Could use templating here. */

InitException::InitException(std::string msg)
{
    _msg += msg;
}

InitException::InitException(const char* msg)
{
    _msg += msg;
}

const char* InitException::what() const throw()
{
    return _msg.c_str();
}

FileException::FileException(std::string msg, std::string path)
{
    _msg += msg;
    _msg += " (";
    _msg += path;
    _msg += ")";
}

const char* FileException::what() const throw()
{
    return _msg.c_str();
}

ShaderException::ShaderException(const char* shaderType, std::string infoLog)
{
    std::ostringstream msg;
    msg << shaderType << infoLog;
    _msg += msg.str();
}

const char* ShaderException::what() const throw()
{
    return _msg.c_str();
}
