#include "paddle.h"

paddle::paddle(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height, GLfloat stepSize)
{
    this->m_position[0] = x;
    this->m_position[1] = y;
    this->m_position[2] = z;

    this->m_width = width;
    this->m_height = height;

    this->m_stepSize = stepSize;

    /* Default player color - blue */
    this->m_colorRGB[0] = 0;
    this->m_colorRGB[1] = 0;
    this->m_colorRGB[2] = 1;
}

void paddle::moveUp()
{
    this->m_position[1] += this->m_stepSize;
}

void paddle::moveDown()
{
    this->m_position[1] -= this->m_stepSize;
}

void paddle::setColor(float r, float g, float b)
{
    this->m_colorRGB[0] = r;
    this->m_colorRGB[1] = g;
    this->m_colorRGB[2] = b;
}

void paddle::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
    this->m_position[0] = x;
    this->m_position[1] = y;
    this->m_position[2] = z;
}

void paddle::draw()
{
    glBegin(GL_QUADS);
        glColor3f(this->m_colorRGB[0], this->m_colorRGB[1], this->m_colorRGB[2]);
        glVertex3f(this->m_position[0],               this->m_position[1],                this->m_position[2]);
        glVertex3f(this->m_position[0]+this->m_width, this->m_position[1],                this->m_position[2]);
        glVertex3f(this->m_position[0]+this->m_width, this->m_position[1]+this->m_height, this->m_position[2]);
        glVertex3f(this->m_position[0],               this->m_position[1]+this->m_height, this->m_position[2]);
    glEnd();
}

GLfloat paddle::getX()
{
    return this->m_position[0];
}

GLfloat paddle::getY()
{
    return this->m_position[1];
}

GLfloat paddle::getZ()
{
    return this->m_position[2];
}

GLfloat paddle::getWidth()
{
    return this->m_width;
}

GLfloat paddle::getHeight()
{
    return this->m_height;
}
