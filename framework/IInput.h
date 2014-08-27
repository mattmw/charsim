/*
 * IInput.h
 *
 *      Author: MMW
 */
#ifndef IINPUT_H_
#define IINPUT_H_

#include "framework/Input.h"

class IInput
{
public:
    IInput(Input* input)
            : _input{input} {};

protected:
    Input* _input;

};

#endif /* IINPUT_H_ */
