#ifndef GLCOREWIDGET_H
#define GLCOREWIDGET_H

#include <QGLWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QApplication>
#include <QDebug>

#include "paddle.h"
#include "pongBall.h"

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
    void ballSpeed(int speed);
    void ballMoveThroughWalls(bool state);

private slots:
    void newFrame();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void keyPressEvent(QKeyEvent *e);


private:
    QTimer *timer;          // timer that repaints the glWidget on timeout. Used to generate Frames or Frames per Seconds (FPS)

    paddle *player1;        // player 1 paddle
    paddle *player2;        // player 2 paddle

    pongBall *ball;         // pong ball

    bool ballMoving;        // ball needs to be "shot" from a player in order to move at the beginning of a game and after each goal

    void collusionDetection();  // calls and handles all colusion functions and events
};

#endif // GLCOREWIDGET_H
