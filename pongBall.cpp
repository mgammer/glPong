#include "pongBall.h"

pongBall::pongBall(GLfloat x, GLfloat y, GLfloat z, int speed, int angle)
{
    // setup ball environment
    this->m_position[0] = x;
    this->m_position[1] = y;
    this->m_position[2] = z;

    this->m_oldPosition[0] = x;
    this->m_oldPosition[1] = y;
    this->m_oldPosition[2] = z;

    this->m_ballMotionPolar[0] = speed;
    this->m_ballMotionPolar[1] = angle;

    this->m_collideAgainstWall = true;  // default: we can hit walls :)

    // setup sound
    this->m_mediaObject = new Phonon::MediaObject();
    this->m_audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory);
    path = Phonon::createPath(this->m_mediaObject, this->m_audioOutput);
}

pongBall::~pongBall()
{
    delete this->m_mediaObject;
    delete this->m_audioOutput;
}

void pongBall::draw()
{
    glTranslatef( this->m_position[0], this->m_position[1], this->m_position[2]-BALLRADIUS );       // translate to the ball origin
    glColor3f(this->m_color[0], this->m_color[1], this->m_color[2]);                                // apply the ball color
    gluSphere(gluNewQuadric(), BALLRADIUS, 32, 16);                                                 // draw the ball
    glLoadIdentity();                                                                               // undo translation
}

void pongBall::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
    this->m_position[0] = x;
    this->m_position[1] = y;
    this->m_position[2] = z;

    this->m_oldPosition[0] = x;
    this->m_oldPosition[1] = y;
    this->m_oldPosition[2] = z;
}

void pongBall::setAngle(int angle)
{
    this->m_ballMotionPolar[1] = angle;
}

void pongBall::setColor(float r, float g, float b)
{
    this->m_color[0] = r;
    this->m_color[1] = g;
    this->m_color[2] = b;
}

void pongBall::setSpeed(int speed)
{
    this->m_ballMotionPolar[0] = speed;
}

void pongBall::setMoveThroughWall(bool b)
{
    this->m_collideAgainstWall = !b;
}

int pongBall::collusionPaddle(GLfloat x, GLfloat y, GLfloat width, GLfloat height)
{
    /*
     * Known Bug: if ball moves faster than player is thick, ball can move through player into the goal
     * Solution: need to calculate if ball moved through player from his last position to his actual position
     *            if thou, reset ball to a 'move'-position in front of the player to do collusion detection
     * Status: Corrected
     */
    if(abs(this->m_position[0] - this->m_oldPosition[0]) >= this->m_ballMotionPolar[0]/*+BALLRADIUS*2*/) {          // settings are given for a jump -> check if jumped
        if(this->m_position[1]-BALLRADIUS <= y+height && this->m_position[1]+BALLRADIUS >= y) {                     // jump does not matter if paddle is not in ball y-axis
            if(this->m_oldPosition[0]-BALLRADIUS > x+width && this->m_position[0]+BALLRADIUS < x) {                 // we jumped over the player1 paddle without being detected
                this->m_position[0] = this->m_oldPosition[0];                                                       // reset position to old position
                this->m_position[1] = this->m_oldPosition[1];
                while(this->m_position[0]-BALLRADIUS > x+width) {                                                   // calculate new ball position that will hit the paddle
                    this->m_position[0] += cos(m_ballMotionPolar[1]*M_PI/180) * width;                              // we use the player width as speed, this way the ball can not jump through the player
                    this->m_position[1] += sin(m_ballMotionPolar[1]*M_PI/180) * width;
                }
            }
            else if(this->m_oldPosition[0]+BALLRADIUS < x && this->m_position[0]-BALLRADIUS > x+width) {            // we jumped over the player2 paddle without being detected
                this->m_position[0] = this->m_oldPosition[0];                                                       // reset position to old position
                this->m_position[1] = this->m_oldPosition[1];
                while(this->m_position[0]+BALLRADIUS < x) {                                                         // calculate new ball position that will hit the paddle
                    this->m_position[0] += cos(this->m_ballMotionPolar[1]*M_PI/180) * width;                        // we use the player width as speed, this way the ball can not jump through the player
                    this->m_position[1] += sin(this->m_ballMotionPolar[1]*M_PI/180) * width;
                }
            }
        }
    }

    /* ball m_ballMotionPolar[1] can be negative. This makes sure our augle is between 0 and 360 degree */
    while(this->m_ballMotionPolar[1] < 0)
        this->m_ballMotionPolar[1] += 360;

    /*
     * The actual collusion detection
     * Procedure: check if ball (with ballradius) is in paddle rectangle, if thou, check where ball hits the paddle and which way the ball goes (depend on the angle)
     *              and make some angle-effects on the ball. We do not turn the balls direction.
     */
    if((this->m_position[0]+BALLRADIUS >= x && this->m_position[0]+BALLRADIUS <= x+width)
            || (this->m_position[0]-BALLRADIUS >= x && this->m_position[0]-BALLRADIUS <= x+width)) {    // x-coordinate match
        if(this->m_position[1]-BALLRADIUS <= y+height && this->m_position[1]+BALLRADIUS >= y) {         // y-coordinate match
            // collusion with player - detect where to apply a usefull angle

            // player paddle split into 5 sections: from player paddle top to bottum
            if(this->m_position[1] >= y+(height/5)*4) {
                if(abs(m_ballMotionPolar[1]) == 0)
                    m_ballMotionPolar[1] -= 10;
                else if(abs(m_ballMotionPolar[1]) == 180)
                    m_ballMotionPolar[1] += 10;

                if(m_ballMotionPolar[1] > 0 && m_ballMotionPolar[1] < 90)               // first quadrant
                    m_ballMotionPolar[1] = (m_ballMotionPolar[1]+10)*-1;
                else if(m_ballMotionPolar[1] > 90 && m_ballMotionPolar[1] < 180)        // second quadrant
                    m_ballMotionPolar[1] = (m_ballMotionPolar[1]-10)*-1;
                else if(m_ballMotionPolar[1] > 180 && m_ballMotionPolar[1] < 270) {}    // third quadrant - do nothing
                else if(m_ballMotionPolar[1] > 270 && m_ballMotionPolar[1] < 360) {}    // fourth quadrant - do nothing

            }
            else if(this->m_position[1] <= y+(height/5)*4 && this->m_position[1] >= y+(height/5)*3) {
                if(abs(m_ballMotionPolar[1]) == 0)
                    m_ballMotionPolar[1] -= 5;
                else if(abs(m_ballMotionPolar[1]) == 180)
                    m_ballMotionPolar[1] += 5;

                if(m_ballMotionPolar[1] > 0 && m_ballMotionPolar[1] < 90)               // first quadrant
                    m_ballMotionPolar[1] = (m_ballMotionPolar[1]+5)*-1;
                else if(m_ballMotionPolar[1] > 90 && m_ballMotionPolar[1] < 180)        // second quadrant
                    m_ballMotionPolar[1] = (m_ballMotionPolar[1]-5)*-1;
                else if(m_ballMotionPolar[1] > 180 && m_ballMotionPolar[1] < 270)       // third quadrant - lower angle
                    m_ballMotionPolar[1] = (m_ballMotionPolar[1]-10)*-1;
                else if(m_ballMotionPolar[1] > 270 && m_ballMotionPolar[1] < 360)       // fourth quadrant - lower angle
                    m_ballMotionPolar[1] = (m_ballMotionPolar[1]+10)*-1;

            }
            else if(this->m_position[1] <= y+(height/5)*3 && this->m_position[1] >= y+(height/5)*2) {
                // hitting paddel middle does change the direction
                m_ballMotionPolar[1] *=-1;
            }
            else if(this->m_position[1] <= y+(height/5)*2 && this->m_position[1] >= y+(height/5)*1) {
                if(abs(m_ballMotionPolar[1]) == 0)
                    m_ballMotionPolar[1] += 5;
                else if(abs(m_ballMotionPolar[1]) == 180)
                    m_ballMotionPolar[1] -= 5;

                if(m_ballMotionPolar[1] > 0 && m_ballMotionPolar[1] < 90)               // first quadrant
                    m_ballMotionPolar[1] = (m_ballMotionPolar[1]-10)*-1;
                else if(m_ballMotionPolar[1] > 90 && m_ballMotionPolar[1] < 180)        // second quadrant
                    m_ballMotionPolar[1] = (m_ballMotionPolar[1]+10)*-1;
                else if(m_ballMotionPolar[1] > 180 && m_ballMotionPolar[1] < 270)       // third quadrant - lower angle
                    m_ballMotionPolar[1] = (m_ballMotionPolar[1]+5)*-1;
                else if(m_ballMotionPolar[1] > 270 && m_ballMotionPolar[1] < 360)       // fourth quadrant - lower angle
                    m_ballMotionPolar[1] = (m_ballMotionPolar[1]-5)*-1;
            }
            else if(this->m_position[1] <= y+(height/5)*1) {
                if(abs(m_ballMotionPolar[1]) == 0)
                    m_ballMotionPolar[1] += 10;
                else if(abs(m_ballMotionPolar[1]) == 180)
                    m_ballMotionPolar[1] -= 10;

                if(m_ballMotionPolar[1] > 0 && m_ballMotionPolar[1] < 90) {}            // first quadrant - do nothing
                else if(m_ballMotionPolar[1] > 90 && m_ballMotionPolar[1] < 180) {}     // second quadrant - do nothing
                else if(m_ballMotionPolar[1] > 180 && m_ballMotionPolar[1] < 270)       // third quadrant
                    m_ballMotionPolar[1] = (m_ballMotionPolar[1]+10)*-1;
                else if(m_ballMotionPolar[1] > 270 && m_ballMotionPolar[1] < 360)       // fourth quadrant
                    m_ballMotionPolar[1] = (m_ballMotionPolar[1]-10)*-1;
            }

            /* make sure angle does not get 90 or 270 degree, same for negative angles */
            if(m_ballMotionPolar[1] == 90 || m_ballMotionPolar[1] == -270) {
                m_ballMotionPolar[1] +=15;
            }
            else if(m_ballMotionPolar[1] == 270 || m_ballMotionPolar[1] == -90) {
                m_ballMotionPolar[1] += 15;
            }

            /* finally, we hit a paddle, make a 180° turn */
            this->m_ballMotionPolar[1] = (180+this->m_ballMotionPolar[1]) % 360;

            // play a hit sound
            this->m_mediaObject->setCurrentSource(Phonon::MediaSource(":/sounds/player.wav"));
            this->m_mediaObject->play();

            return 1;   // hit!
        }
    }

    return 0;
}

int pongBall::collusionWall(GLfloat whereY)
{
    if(this->m_position[1]+BALLRADIUS >= whereY || this->m_position[1]-BALLRADIUS <= -whereY) {
        if(this->m_collideAgainstWall) {
            this->m_ballMotionPolar[1] *= -1;   // make a turn

            // play a hit sound
            this->m_mediaObject->setCurrentSource(Phonon::MediaSource(":/sounds/wall.wav"));
            this->m_mediaObject->play();
        }
        else
            this->m_position[1] *= -1;          // change screen position - we moved from top to bottom through the wall :)

        return 1;
    }

    return 0;
}

int pongBall::goal(GLfloat goalposition)
{
    if(this->m_position[0] >= goalposition) {
        // play a goal sound
        this->m_mediaObject->setCurrentSource(Phonon::MediaSource(":/sounds/goal.wav"));
        this->m_mediaObject->play();

        return 1;   // player 1 scored
    }
    else if(this->m_position[0] <= -goalposition) {
        // play a goal sound
        this->m_mediaObject->setCurrentSource(Phonon::MediaSource(":/sounds/goal.wav"));
        this->m_mediaObject->play();

        return 2;   // player 2 scored
    }
    return 0;       // no goal
}

void pongBall::move()
{
    this->m_oldPosition[0] = this->m_position[0];
    this->m_oldPosition[1] = this->m_position[1];

    this->m_position[0] += cos(m_ballMotionPolar[1]*M_PI/180) * m_ballMotionPolar[0];
    this->m_position[1] += sin(m_ballMotionPolar[1]*M_PI/180) * m_ballMotionPolar[0];
}

GLfloat pongBall::getX()
{
    return this->m_position[0];
}

GLfloat pongBall::getY()
{
    return this->m_position[1];
}

GLfloat pongBall::getZ()
{
    return this->m_position[2];
}
