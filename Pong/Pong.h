// Pong.h
#ifndef PONG_H
#define PONG_H

// Dimensions de la fenêtre
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

// Dimensions de la raquette et de la balle
#define PADDLE_WIDTH  20
#define PADDLE_HEIGHT 100
#define BALL_SIZE     20

// Vitesse de déplacement de la raquette et de la balle
#define PADDLE_SPEED  10
#define BALL_SPEED    5

// Position initiale de la raquette et de la balle
extern int paddleY;
extern int ballX;
extern int ballY;
extern int ballSpeedX;
extern int ballSpeedY;

#endif
