#ifndef PADDLE_H
#define PADDLE_H

#include <qgl.h>

class paddle
{
public:
    paddle(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height, GLfloat stepsize);
    void moveUp();
    void moveDown();
    void setColor(float r, float g, float b);
    void setPosition(GLfloat x, GLfloat y, GLfloat z);
    void draw();

    GLfloat getX();
    GLfloat getY();
    GLfloat getZ();
    GLfloat getWidth();
    GLfloat getHeight();

private:
    GLfloat m_position[3];      // position of the paddle lower left point
    GLfloat m_width;            // width of a player paddle
    GLfloat m_height;           // height of a player paddle
    GLfloat m_stepSize;         // move width of a paddle
    float m_colorRGB[3];        // paddle RGB color values
};

#endif // PADDLE_H
