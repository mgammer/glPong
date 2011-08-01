#ifndef PONGBALL_H
#define PONGBALL_H

#include <qgl.h>
#include <math.h>
#include <phonon/MediaObject>
#include <phonon/AudioOutput>
#include <QApplication>

#define BALLRADIUS 2

class pongBall
{
public:
    pongBall(GLfloat x, GLfloat y, GLfloat z, int speed, int angle);
    ~pongBall();

    void draw();

    int collusionPaddle(GLfloat x, GLfloat y, GLfloat width, GLfloat height);
    int collusionWall(GLfloat whereY);
    int goal(GLfloat goalposition);
    void move();

    void setColor(float r, float g, float b);
    void setPosition(GLfloat x, GLfloat y, GLfloat z);
    void setAngle(int angle);
    void setSpeed(int speed);
    void setMoveThroughWall(bool b);

    GLfloat getX();
    GLfloat getY();
    GLfloat getZ();

private:
    GLfloat m_position[3];          // ball position in kartesian-coordinates -> x, y, z
    GLfloat m_oldPosition[3];       // ball position from last movement in kartesian-coordinates -> x, y, z
    int m_ballMotionPolar[2];       // ball motion described in polar-coordinates -> ballspeed and angle
    float m_color[3];               // ball RGB Color values

    bool m_collideAgainstWall;      // ignore walls?

   Phonon::MediaObject *m_mediaObject;  // mediaObject which plays given sound files sounds NOTE: due to a Qt bug, mediaObject can not play soundfiles from Qt resource-files .qrc
   Phonon::AudioOutput *m_audioOutput;  // audioOutput outputs audio from mediaObject to soundcard
   Phonon::Path path;                   // creates connection between mediaObject and audioOutput
};

#endif // PONGBALL_H
