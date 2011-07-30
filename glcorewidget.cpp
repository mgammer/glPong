#include "glcorewidget.h"

#define FRAMETIME 33
#define FIELDLENGTH 200                 // Fieldlength: whole field is from -fieldlength to +fieldlength (origin is in the middle of the glWidget)
#define FIELDHEIGHT 100                 // Fieldheight: whole field is from -fieldheight to +fieldheight (origin is in the middle of the glwidget)
#define PLAYERLENGTH FIELDHEIGHT/2      // Length of each player bar
#define PLAYERWIDTH PLAYERLENGTH/10     // Thickness of each player
#define PLAYERSTEP PLAYERLENGTH/5       // Stepwidth for each player
#define BALLRADIUS 2                    // Radius of the sphere/ball
#define BALLSPEED 5                     // ballspeed per frame => ~30FPS *BALLSPEED = MOVEMENT PER SECOND


glCoreWidget::glCoreWidget(QWidget *parent) :
    QGLWidget(parent)
{
    player1 = new paddle(-FIELDLENGTH+PLAYERWIDTH*2, 0, 0, PLAYERWIDTH, PLAYERLENGTH, PLAYERSTEP);
    player1->setColor(0.0, 0.5, 1.0);
    player2 = new paddle(FIELDLENGTH-(PLAYERWIDTH*3), 0, 0, PLAYERWIDTH, PLAYERLENGTH, PLAYERSTEP);
    player2->setColor(0.5, 0.0, 1.0);

    ball = new pongBall(player1->getX()+player1->getWidth()+BALLRADIUS*2, 0.0f+PLAYERLENGTH/2, -BALLRADIUS, BALLSPEED, 180);
    ball->setColor(0.0, 1.0, 0.0);

    resetGame();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(newFrame()));
}

glCoreWidget::~glCoreWidget()
{
    if(player1)
        delete player1;
    if(player2)
        delete player2;
    if(ball)
        delete ball;
    if(timer)
        delete timer;
}

void glCoreWidget::initializeGL()
{
    // opengl version number
    qDebug() << "OpenGL Information:" << (char*)glGetString(GL_VERSION)
             << (char*)glGetString(GL_VENDOR) << (char*)glGetString(GL_RENDERER);

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
    player1->draw();

    // draw player 2 right
    player2->draw();

    // draw ball
    ball->draw();
}

void glCoreWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-FIELDLENGTH, FIELDLENGTH, -FIELDHEIGHT, FIELDHEIGHT, -10, 10);       // set the game-coordinate system to x= -100 ... 100, y= -100 ... 100, z= 0 ... 10

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
    if(ballMoving)
        ball->move();

    collusionDetection();
    updateGL();                 // update GLWidget painting
}

void glCoreWidget::collusionDetection()
{
    /*
     * Some Steps to do:
     * 1. Check for wall collusion
     * 2. Check for player collusion
     * 3. Check for a goal
     */
    ball->collusionWall(FIELDHEIGHT);

    ball->collusionPaddle(player1->getX(), player1->getY(), player1->getWidth(), player1->getHeight());
    ball->collusionPaddle(player2->getX(), player2->getY(), player2->getWidth(), player2->getHeight());

    int goalForPlayer = ball->goal(FIELDLENGTH);
    if( goalForPlayer == 1) {
        emit goal(1);
        ball->setPosition(player2->getX()-BALLRADIUS*2, 0.0f+player2->getHeight()/2, -BALLRADIUS);
        ballMoving = false;
        ball->setAngle(0);
    }
    else if( goalForPlayer == 2) {
        emit goal(2);
        ball->setPosition(player1->getX()+player1->getWidth()+BALLRADIUS*2, 0.0f+player1->getHeight()/2, -BALLRADIUS);
        ballMoving = false;
        ball->setAngle(180);
    }

}

void glCoreWidget::keyPressEvent(QKeyEvent *e)
{
    switch(e->key()) {
    case Qt::Key_Up:
        player2->moveUp();
        break;
    case Qt::Key_Down:
        player2->moveDown();
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
        player1->moveUp();
        break;
    case Qt::Key_Y:
        player1->moveDown();
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

    player1->setPosition(-FIELDLENGTH+PLAYERWIDTH*2, 0.0f , 0.0f);
    player2->setPosition(FIELDLENGTH-(PLAYERWIDTH*3), 0.0f, 0.0f);

    ball->setPosition(player1->getX()+player1->getWidth()+BALLRADIUS*2, 0.0f+player1->getHeight()/2, -BALLRADIUS);
    ball->setAngle(180);

    ballMoving = false;
}

void glCoreWidget::ballSpeed(int speed)
{
    ball->setSpeed(speed);
}

void glCoreWidget::ballMoveThroughWalls(bool state)
{
    ball->setMoveThroughWall(state);
}
