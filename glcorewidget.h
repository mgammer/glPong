#ifndef GLCOREWIDGET_H
#define GLCOREWIDGET_H

#include <QGLWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QApplication>
#include <math.h>
#include <QDebug>

class glCoreWidget : public QGLWidget
{
    Q_OBJECT

public:
    explicit glCoreWidget(QWidget *parent = 0);
    ~glCoreWidget();

signals:
    void goal(int player);
    void toggleGame();
    void resetLCD();
    void toggleFullscreen();

public slots:
    void startGame(bool state);
    void resetGame();

private slots:
    void newFrame();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void keyPressEvent(QKeyEvent *e);


private:
    QTimer *timer;

    GLfloat p1pos[3];       // player 1 position - lower left point
    GLfloat p2pos[3];       // player 2 position - lower left point

    GLfloat ball[3];        // ball position - middle
//    GLfloat balltmp[2];   // only needed for line drawn circle ... but ball is drawn as a sphere
    bool ballMoving;        // ball needs to be "shot" from a player in order to move at the beginning of a game and after each goal
    int ballMotionPolar[2];   // ballMothionPolarVector [0] = BALLSPEED, [1] = angle
    int ballXMoveDirection; // ball move left (-1) or right (+1)
    int ballYMoveDirection; // ball move down (-1) or up (+1)

    int calculateBallMovement();
    void resetBallMotion();
};

#endif // GLCOREWIDGET_H
