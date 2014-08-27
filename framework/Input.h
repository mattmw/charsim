/*
 * Input.h
 *
 * Handles keyboard and mouse states.
 * GameEnvironment keeps a copy for the current state to be set by GameLoop.
 * GameEnvironment distributes reference to interactive scenegraph nodes.
 * Objects access reference and perform their own handling.
 *
 *      Author: MMW
 */

#ifndef INPUT_H_
#define INPUT_H_

#define KEY 0
#define SCANCODE 1
#define ACTION 2
#define MODS 3

struct Input
{
    /* Keyboard inputs. */
    int key = -1;
    int scancode = -1;
    int action = -1;
    int mods = -1;

    /* Mouse inputs. */
    bool isEntered;

    /* World-space coordinates. */
    float x;
    float y;
};

#endif /* INPUT_H_ */
