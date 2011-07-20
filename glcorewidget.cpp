#include "glcorewidget.h"

#define FRAMETIME 33
#define FIELDLENGTH 200                 // Fieldlength: whole field is from -fieldlength to +fieldlength (origin is in the middle of the glWidget)
#define FIELDHEIGHT 100                 // Fieldheight: whole field is from -fieldheight to +fieldheight (origin is in the middle of the glwidget)
#define PLAYERLENGTH FIELDHEIGHT/2      // Length of each player bar
#define PLAYERWIDTH PLAYERLENGTH/10     // Thickness of each player
#define PLAYERSTEP PLAYERLENGTH/5                   // Stepwidth for each player
#define BALLRADIUS 2                    // Radius of the sphere/ball
#define BALLSPEED 5                     // ballspeed per frame => ~30FPS *BALLSPEED = MOVEMENT PER SECOND


glCoreWidget::glCoreWidget(QWidget *parent) :
    QGLWidget(parent)
{
    resetGame();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(newFrame()));
}

glCoreWidget::~glCoreWidget()
{
    if(timer)
        delete timer;
}

void glCoreWidget::initializeGL()
{
    // opengl version number
    qDebug() << "OpenGL Information:" << (char*)glGetString(GL_VERSION)
             << (char*)glGetString(GL_VENDOR) << (char*)glGetString(GL_RENDERER);

    qDebug() << FIELDLENGTH << FIELDHEIGHT << PLAYERLENGTH << PLAYERWIDTH << PLAYERSTEP;

    // loaded once before paintGL is called
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glShadeModel(GL_SMOOTH);
    glShadeModel(GL_FLAT);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    qglClearColor(Qt::black);
    //glFrustum(-10, 10, -10 , 10, -10, -100);

}

void glCoreWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
    glLoadIdentity();                                                                   // Reset The View

    // draw player 1 left
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.5f, 1.0f);
    glVertex3f(p1pos[0],             p1pos[1],              p1pos[2]);
    glVertex3f(p1pos[0]+PLAYERWIDTH, p1pos[1],              p1pos[2]);
    glVertex3f(p1pos[0]+PLAYERWIDTH, p1pos[1]+PLAYERLENGTH, p1pos[2]);
    glVertex3f(p1pos[0],             p1pos[1]+PLAYERLENGTH, p1pos[2]);
    glEnd();

    // draw player 2 right
    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.0f, 1.0f);
    glVertex3f(p2pos[0],             p2pos[1],              p2pos[2]);
    glVertex3f(p2pos[0]+PLAYERWIDTH, p2pos[1],              p2pos[2]);
    glVertex3f(p2pos[0]+PLAYERWIDTH, p2pos[1]+PLAYERLENGTH, p2pos[2]);
    glVertex3f(p2pos[0],             p2pos[1]+PLAYERLENGTH, p2pos[2]);
    glEnd();

    // draw ball
    glTranslatef( ball[0], ball[1], ball[2]-BALLRADIUS );       // translate to the ball origin
    glColor3f(0.0f, 1.0f, 0.0f);                                // apply the ball color
    gluSphere(gluNewQuadric(), BALLRADIUS, 32, 16);              // draw the ball
    glLoadIdentity();                                           // redo translation

    // a circle with lines drawn - only lines are colored
//    glBegin(GL_LINES);
//    for (int i = 0; i < 180; i++)
//    {
//        balltmp[0] = BALLRADIUS * cos(i) - ball[0];
//        balltmp[1] = BALLRADIUS * sin(i) + ball[1];
//        glVertex3f(balltmp[0] + ball[1],balltmp[1] - ball[0],0);

//        balltmp[0] = BALLRADIUS * cos(i + 0.1) - ball[0];
//        balltmp[1] = BALLRADIUS * sin(i + 0.1) + ball[1];
//        glVertex3f(balltmp[0] + ball[1],balltmp[1] - ball[0],0);
//    }
//    glEnd();

    // a sphere with lines drawn, so a circle :)
//    glColor3f(0.0f, 1.0f, 0.0f);
//    glBegin(GL_LINE_LOOP);
//    const float PI = 3.1415926535897932;
//    for (float i = 0.0; i < 2.0*PI; i += 2.0*PI/30.0){
//        float x = BALLRADIUS * cos(i) + ball[0];
//        float y = BALLRADIUS * sin(i) + ball[1];
//        glVertex3f(x,y,0);
//    }
//    glEnd();



}

void glCoreWidget::resizeGL(int w, int h)
{
//    glViewport(0, 0, w, h);
//    glMatrixMode(GL_PROJECTION);
//    glFrustum((w/2)*-1, w/2, (h/2)*-1, h/2, 4.0, 15.0);

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-FIELDLENGTH, FIELDLENGTH, -FIELDHEIGHT, FIELDHEIGHT, 0, 10);       // set the game-coordinate system to x= -100 ... 100, y= -100 ... 100, z= 0 ... 10

    glMatrixMode(GL_MODELVIEW);
}

void glCoreWidget::startGame(bool state)
{
    if(state) {
        timer->start(FRAMETIME);
    }
    else {
        timer->stop();;
    }
}

void glCoreWidget::newFrame() /* function is only called, when game is running (not in pause mode)  */
{
    updateGL();                 // update GLWidget painting
    calculateBallMovement();    // check for collusion detection and calc new ball position
}

void glCoreWidget::keyPressEvent(QKeyEvent *e)
{
    switch(e->key()) {
    case Qt::Key_Up:
        // move player 2 up
        if(p2pos[1] < FIELDHEIGHT-PLAYERLENGTH)
            p2pos[1] += PLAYERSTEP;
        break;
    case Qt::Key_Down:
        // move player 2 down
        if(p2pos[1] > -FIELDHEIGHT)
            p2pos[1] -= PLAYERSTEP;
        break;
    case Qt::Key_Space:
        emit toggleGame();
        break;
    case Qt::Key_Enter:
        ballMoving = true;
        break;
    case Qt::Key_Return:
        ballMoving = true;
        break;
    case Qt::Key_A:
        // move player 1 up
        if(p1pos[1] < FIELDHEIGHT-PLAYERLENGTH)
            p1pos[1] += PLAYERSTEP;
        break;
    case Qt::Key_Y:
        // move player 1 down
        if(p1pos[1] > -FIELDHEIGHT)
            p1pos[1] -= PLAYERSTEP;
        break;
    case Qt::Key_R:
        resetGame();
        break;
    case Qt::Key_Escape:
        QApplication::instance()->quit();
        break;
    case Qt::Key_F:
        emit toggleFullscreen();
        break;
    default:
        qDebug() << "unhandled key pressed:" << e->key();
        break;
    }

    // if game is on pause, updateGL
    if(!timer->isActive())
        updateGL();
}

void glCoreWidget::resetGame()
{
    resetLCD();
    // TODO: reset ball and player position
    p1pos[0] = -FIELDLENGTH+PLAYERWIDTH*2;  // x-coordinate never changes
    p1pos[1] = 0.0f;    // y-coordinate
    p1pos[2] = 0.0f;    // z-coordinate never changes

    p2pos[0] = FIELDLENGTH-(PLAYERWIDTH*3);   // x-coordinate never changes
    p2pos[1] = 0.0f;    // y-coordinate
    p2pos[2] = 0.0f;    // z-coordinate never changes

    ball[0] = p1pos[0]+PLAYERWIDTH+BALLRADIUS;     // x-coordinate in front of player 1
    ball[1] = 0.0f + PLAYERLENGTH/2;        // y-coordinate in the middle of player 1
    ball[2] = 0.0f;                         // z-coordinate never changes - we are in a 2D environment :)
    ballMoving = false;
    ballXMoveDirection = -1;
    ballYMoveDirection = 1;
    resetBallMotion();
}

int glCoreWidget::calculateBallMovement()
{
    /*
     * Collusion detection against the field boundaries
     */
    if(ball[0]+BALLRADIUS >= FIELDLENGTH || ball[0]-BALLRADIUS <= -FIELDLENGTH) {
        // goal
        qDebug() << "goal";
        if(ball[0]+BALLRADIUS >= FIELDLENGTH) {
            emit goal(1);
            // reset ball position in front of player
            ball[0] = p2pos[0] - BALLRADIUS;
            ball[1] = 0.0f + PLAYERLENGTH/2;
            ball[2] = 0.0f;
            ballMoving = false;
            resetBallMotion();
        }
        else {
            emit goal(2);
            // reset ball position in front of player
            ball[0] = p1pos[0] + PLAYERWIDTH + BALLRADIUS;
            ball[1] = 0.0f + PLAYERLENGTH/2;
            ball[2] = 0.0f;
            ballMoving = false;
            resetBallMotion();
        }

    }
    if(ball[1]+BALLRADIUS >= FIELDHEIGHT || ball[1]-BALLRADIUS <= -FIELDHEIGHT) {
        // collusion against a boundarie => change ball y-component
        //ballYMoveDirection *= -1;
        ballMotionPolar[1] *= -1;
    }

    if(!ballMoving) // ball not moving, nothing needs to be calculated
        return 0;

    /*
     * Collusion detection against player paddles
     */
    if(ball[0]-BALLRADIUS >= p1pos[0] && ball[0]-BALLRADIUS <= p1pos[0]+PLAYERWIDTH) {  // x-coordinate match
        if((ball[1]-BALLRADIUS <= p1pos[1]+PLAYERLENGTH && ball[1]-BALLRADIUS >= p1pos[1]) || (ball[1]+BALLRADIUS <= p2pos[1]+PLAYERLENGTH && ball[1]+BALLRADIUS >= p2pos[1])) {
            // collusion with player 1 - detect where to apply a usefull angle
            ballXMoveDirection *= -1;
            // from player top to bottum
            if(ball[1] <= p1pos[1]+PLAYERLENGTH && ball[1] >= p1pos[1]+(PLAYERLENGTH/5)*4) {
//                if(ballMotionPolar[1] >= 0)
                    ballMotionPolar[1] = 60 - (ballMotionPolar[1] - 60);
            }
            if(ball[1] <= p1pos[1]+(PLAYERLENGTH/5)*4 && ball[1] >= p1pos[1]+(PLAYERLENGTH/5)*3) {
                ballMotionPolar[1] = 30 - (ballMotionPolar[1] - 30);
            }
            if(ball[1] <= p1pos[1]+(PLAYERLENGTH/5)*3 && ball[1] >= p1pos[1]+(PLAYERLENGTH/5)*2) {
                ballMotionPolar[1] += 0;
            }
            if(ball[1] <= p1pos[1]+(PLAYERLENGTH/5)*2 && ball[1] >= p1pos[1]+(PLAYERLENGTH/5)*1) {
                ballMotionPolar[1] = -30 - (ballMotionPolar[1] + 30);
            }
            if(ball[1] <= p1pos[1]+(PLAYERLENGTH/5)*1 && ball[1] >= p1pos[1]+(PLAYERLENGTH/5)*0) {
                ballMotionPolar[1] = -60 - (ballMotionPolar[1] + 60);
            }
            qDebug() << "@player1:" << ballMotionPolar[1];
        }
    }
    if(ball[0]+BALLRADIUS >= p2pos[0] && ball[0]+BALLRADIUS <= p2pos[0]+PLAYERWIDTH) {  // x-coordinate match
        if((ball[1]-BALLRADIUS <= p2pos[1]+PLAYERLENGTH && ball[1]-BALLRADIUS >= p2pos[1]) || (ball[1]+BALLRADIUS <= p2pos[1]+PLAYERLENGTH && ball[1]+BALLRADIUS >= p2pos[1])) {
            // collusion
            ballXMoveDirection *= -1;
            // from player top to bottum
            if(ball[1] <= p2pos[1]+PLAYERLENGTH && ball[1] >= p2pos[1]+(PLAYERLENGTH/5)*4) {
//                ballMotionPolar[1] += 60;
                qDebug() << "player2 top";
                ballMotionPolar[1] += -60;
            }
            if(ball[1] <= p2pos[1]+(PLAYERLENGTH/5)*4 && ball[1] >= p2pos[1]+(PLAYERLENGTH/5)*3) {
//                ballMotionPolar[1] += 30;
                qDebug() << "player2 middle-top";
                ballMotionPolar[1] += -30;
            }
            if(ball[1] <= p2pos[1]+(PLAYERLENGTH/5)*3 && ball[1] >= p2pos[1]+(PLAYERLENGTH/5)*2) {
//                ballMotionPolar[1] += 0;
                qDebug() << "player2 middle";
                ballMotionPolar[1] += 0;
            }
            if(ball[1] <= p2pos[1]+(PLAYERLENGTH/5)*2 && ball[1] >= p2pos[1]+(PLAYERLENGTH/5)*1) {
//                ballMotionPolar[1] += -30;
                qDebug() << "player2 middle-buttom";
                ballMotionPolar[1] += 30;
            }
            if(ball[1] <= p2pos[1]+(PLAYERLENGTH/5)*1 && ball[1] >= p2pos[1]+(PLAYERLENGTH/5)*0) {
//                ballMotionPolar[1] += -60;
                qDebug() << "player2 buttom";
                ballMotionPolar[1] += 60;
            }
            qDebug() << "@player2:" << ballMotionPolar[1];
        }
    }

    // check ballMotionPolar[1] value. it does not get highter than 90 or lower than -90, actually just 85 and -85 otherwise we would have no X movement ...
    if(ballMotionPolar[1] < -85)
        ballMotionPolar[1] = -85;
    else if(ballMotionPolar[1] > 85)
        ballMotionPolar[1] = 85;

    // calculate from polor coordinates to cartesian points, to get the new ball location
    ball[0] += cos(ballMotionPolar[1]*M_PI/180) * ballMotionPolar[0] * ballXMoveDirection;
    ball[1] += sin(ballMotionPolar[1]*M_PI/180) * ballMotionPolar[0];// * ballYMoveDirection;

    return 1;   // calculation done
}

void glCoreWidget::resetBallMotion()
{
    ballMotionPolar[0] = BALLSPEED;
    ballMotionPolar[1] = 0;
}
