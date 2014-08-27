/*
 * Exceptions.h
 *
 *      Author: MMW
 */
#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <exception>
#include <string>
#include <sstream>

class InitException : public std::exception
{
public:
    InitException(std::string msg);
    InitException(const char* msg);

    virtual const char* what() const throw();
private:
    std::string _msg = "Initialisation Error: ";
};

class FileException : public std::exception
{
public:
    /* TODO: Duplication. Inherit from BaseException. */
    FileException(std::string msg, std::string path);

    virtual const char* what() const throw();
private:
    std::string _msg = "File IO Error: ";
};

class ShaderException : public std::exception
{
public:
    ShaderException(const char* shaderType, std::string infoLog);
    virtual const char* what() const throw();
private:
    std::string _msg = "Shader compilation error: ";

};

#endif /* EXCEPTIONS_H_ */


