/*
 * ITime.h
 *
 *  Interface for nodes which utilise time-related data, such as the elapsed
 *  game time and frame time delta (the time taken to draw the previous frame).
 *
 *      Author: MMW
 */
#ifndef ITIME_H_
#define ITIME_H_

class ITime
{
public:
    ITime(float* tGame, float* tDelta)
            : _tGame{tGame}, _tDelta{tDelta} {};

protected:
    /* Elapsed game time (time elapsed since first game loop). */
    float* _tGame;
    /* Frame time delta (time taken to draw the previous frame). */
    float* _tDelta;

};

#endif /* ITIME_H_ */
