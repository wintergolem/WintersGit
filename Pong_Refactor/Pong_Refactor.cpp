// =======================================
// Pong_Refactor.cpp
// Copyright (c) Steven Hoover, 2013
// All rights reserved
// ---------------------------------------

//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "../Inc/SimpleFramework.h"
#include "../Inc/gl/freeglut.h"
#include <time.h>
#include <math.h>


char disScore[2000];
clock_t prevTime;

//start of classes
enum EWinner {none, LeftP, RightP};
enum Which {Left, Right};

class Paddle {
public:
	Paddle()
	{
		movingUp = false;
		movingDown = false;
	}
	/*Paddle(Which w, double moveSpeed, int appSize, int halfSize) {
		which = w;

		if(which = Left)
			x = 50;
		else
			x = appSize - 50;
		y = halfSize;
		xPaddleSize = 10;
		yPaddleSize = 50;
		paddleSpeed = moveSpeed;
		movingUp = false;
		movingDown = false;
		score = 0;
	}*/
	~Paddle() {}

	//functions
	void Init(Which w, double moveSpeed, int appSize, int halfSize)
	{
		which = w;

		if(which == Left)
			x = 50;
		else
			x = appSize - 50;
		y = halfSize;
		xPaddleSize = 10;
		yPaddleSize = 50;
		//paddleSpeed = moveSpeed;
		score = 0;
	}
	void Draw(){
		glBegin(GL_QUADS);
		glVertex2d(x, y);
		glVertex2d(x, y + yPaddleSize);
		glVertex2d(x + xPaddleSize, y + yPaddleSize);
		glVertex2d(x + xPaddleSize, y);
		glEnd();
	}
	void Update(double paddleSpeed){
		if(movingUp){
			y += paddleSpeed;
		}
		else if(movingDown){
			y -= paddleSpeed;
		}
		movingUp = false;
		movingDown = false;
	}
	void MoveUp(){
		movingUp = true;
		movingDown = false;
	}
	void MoveDown(){
		movingUp = false;
		movingDown = true;
	}
	void Reset(int halfSize){
		y = halfSize;
	}
	void Scored(int increase)
	{
		score += increase;
	}

	//variables
	int score;
	double x,y;
	double xPaddleSize, yPaddleSize;
	//double paddleSpeed;
	Which which;
	bool movingUp;
	bool movingDown;

	
};

class Ball{
public:
	Ball(){}
	/*Ball(double moveSpeed, int incomingAppSize, int incomingHalfSize){
		x = y = incomingHalfSize;
		xd = 0;
		yd = 0;
		ballSpeed = moveSpeed;
		appSize = incomingAppSize;
	}*/

	//functions
	void Init(double moveSpeed,int incomingAppSize,int incomingHalfSize)
	{
		x = y = incomingHalfSize;
		xd = 1; //positive 1 for right and negative 1 for left
		yd = 1; //positive 1 for right and negative 1 for left
		multipler = 1;
		multiplerAdd = .1;
		appSize = incomingAppSize;
		multiplerDefault = 1;
	}
	void Draw(){
		glPointSize(10);
		glBegin(GL_POINTS);
		glVertex2d(x,y);
		glEnd();
		}
	void Update(double moveby,Paddle pad1,Paddle pad2) {
		// moveball
		moveby *= multipler;
		x += xd *moveby ;
		y += yd *moveby ;

		//change direction
		//use ball x
		if((x > appSize && xd >0))
		{
			hasSomeoneScored = true;
			whoScored = 1;
		}
		if(x < 0 && xd <0)
		{
			hasSomeoneScored = true;
			whoScored = 2;
		}

		//Move ball y
		if((y > appSize && yd >0) || (y < 0 && yd <0))
		{
			yd = -yd; //reverse direction
		}

		if((x <= (pad1.x + pad1.xPaddleSize)) && (y >= pad1.y && y <= pad1.y + pad1.yPaddleSize && xd < 0) || (x >= pad2.x && xd > 0) && (y >= pad2.y && y <= pad2.y+50))
		{
			multipler += multiplerAdd; //speed up ball
			xd = -xd; //reverse direction
		}
	}//end of update
	void Reset(int halfSize){
		y = halfSize; //set at halfsize
		x = halfSize;
		xd = -xd; // change direction towards person who just scored
	}
	//variables
	double x, y;
	double xd, yd;
	double multipler;		//to increase the ball each paddle hit
	double multiplerAdd;	//to add to multipler
	double multiplerDefault;
	int appSize;
	int whoScored; //starting at 1 for player 1
	bool hasSomeoneScored;
};

class Game
{
public:
	Game()
	{
		increaseSpeed = false;
		moveSpeedIncrease = 1;
		moveSpeedDefault = 55;
		moveSpeed = moveSpeedDefault;
		scoreIncrease = 1;
		winScore = 5;
		appSize = 600, halfSize = appSize /2;
		winState = none;
	}
	//classes
	Ball ball;
	Paddle pad1, pad2;

	//functions
	void InitClasses()
	{
		//ball
		ball.Init(moveSpeed, appSize, halfSize);
		pad1.Init(Left, moveSpeed, appSize, halfSize);
		pad2.Init(Right, moveSpeed, appSize, halfSize);
	}
	void Update(double dt)
	{
		if(increaseSpeed == true)
		{
			moveSpeed += moveSpeedIncrease;
			increaseSpeed = false;
		}
		moveby= moveSpeed * dt;
		ball.Update(moveby, pad1, pad2);
		pad1.Update(moveby);
		pad2.Update(moveby);
		CheckForScore();
	}
	void Draw()
	{
		glColor3f(255, 0, 205);
		pad1.Draw();
		pad2.Draw();
		glColor3f(0, 255, 255);
		ball.Draw();
		sprintf(disScore, "Player 1 score: %i | Player 2 score: %i", pad1.score, pad2.score);
		Text(2000, 5000, disScore);

	}
	void Score(int player)
	{
		//score and reset; sent 1 for Player 1 or sent 2 for Player 2
		if(player == 1)
		{
			pad1.Scored(scoreIncrease);
		}
		else if(player == 2)
		{
			pad2.Scored(scoreIncrease);
		}
		else
		{
			printf("Player %i can't score", &player);
		}
	}
	void Reset()
	{
		pad1.Reset(halfSize);
		pad2.Reset(halfSize);
		ball.Reset(halfSize);
		winState = none;
	}
	void CheckForScore()
	{
		if(ball.hasSomeoneScored == true)
		{
			if(ball.whoScored == 1)
			{
				pad1.Scored(scoreIncrease);
				if(CheckForWin(pad1.score) )
				{
					winState = LeftP;
				}
				Reset();
			}
			else if(ball.whoScored ==2)
			{
				pad2.Scored(scoreIncrease);
				if(CheckForWin(pad2.score) )
				{
					winState = RightP;
				}
				Reset();
			}
			//return to false
			ball.hasSomeoneScored = false;
		}
	}
	void GameOver()
	{

	}
	bool CheckForWin(int score)
	{
		if(score >= winScore)
			return true;
		return false;
	}

	
	//vars
	double moveSpeed;		//universal speed
	double moveby;			//moveSpeed times dt to pass into ball and paddle
	double moveSpeedIncrease; //increase universal speed by this amount
	double moveSpeedDefault;
	double movebyDefault;
	int scoreIncrease;
	int winScore;
	int appSize;
	int halfSize;
	EWinner winState;
	bool increaseSpeed; //triggered by testkeys
};
Game game;

//end of classes

//independent functions
void DrawInstructions()
{

	char *disInstruc;
	disInstruc = "w";
	Text(650, 5000, disInstruc);
	disInstruc = "x";
	Text(650, 600, disInstruc);
	disInstruc = "o";
	Text(5300, 5000, disInstruc);
	disInstruc = "m";
	Text(5300, 600, disInstruc);
	disInstruc = "G or H to SpeedUp";
	Text(2500, 500, disInstruc);
}

//Movement
void TestKeys(Paddle *pad1, Paddle *pad2, int appSize)
{
	// W to Raise Left Paddle
	if(KeyDown('W') || KeyDown('w'))
	{
		if(pad1->y + pad1->yPaddleSize < appSize)
		{
			pad1->MoveUp();
			// raise left paddle
		}
	}
	// X to Lower Left Paddle
	if(KeyDown('X') || KeyDown('x'))
	{
		if(pad1->y >= 0)
		{
			pad1->MoveDown();
			// lower left paddle
		}
	}
	// O to Raise Right Paddle
	if(KeyDown('O') || KeyDown('o'))
	{
		if(pad2->y + pad2->yPaddleSize < appSize)
		{
			pad2->MoveUp();
			// raise right paddle
		}
	}//end o if
	// M to Lower Right Paddle
	if(KeyDown('M') || KeyDown('m'))
	{
		if(pad2->y + pad2->yPaddleSize < appSize)
		{
			pad2->MoveDown();
			// lower right paddle
		}
	}//end M if
	//G or H to increase universal speed
	if(KeyDown('G') || KeyDown('g') || KeyDown('H') || KeyDown('h'))
	{
		game.increaseSpeed = true;
	}//end M if
	
	
	
	if(KeyDown('Q') || KeyDown('q'))
	{
		glutDestroyWindow(1);
	}

}//end of Key


void Draw() 
{
	ClearScreen(0,0,0);
	DrawInstructions();
	game.Draw();
	
    glFlush();
}

void Idle (){
	clock_t time = clock();
	double dt = (double) (time-prevTime)/CLOCKS_PER_SEC;
	prevTime = time;

	TestKeys(&game.pad1, &game.pad2, game.appSize);
	game.Update(dt);
    glutPostRedisplay();
}

void main(){
	//Game game;
	game.InitClasses();
	//make new instances of Game class for different variables
	Make2dWindow(100, 100, game.appSize, game.appSize, "Pong");
	
		// set callbacks
		glutDisplayFunc(Draw);                           // respond to redraw requests
		glutIdleFunc(Idle);                              // animate ball
		//glutKeyboardFunc(Key);							 //Keystroke
		// begin event handling, display, and updates
		glutMainLoop();
}
