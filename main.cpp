#include<windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gl/glut.h>
#include <math.h>
#include<string.h>

#define PI 3.14159
#define GAME_SCREEN 0			//Constant to identify background color
#define MENU_SCREEN 4
#define MAX_STONES  100
#define MAX_STONE_TYPES 5
#define stoneRotationSpeed 5
#define SHIP_SPEED 20
int stoneTranslationSpeed=5;

GLint m_viewport[4];
GLint CI=0;
int x,y;
int i;
int randomStoneIndices[100];
int index,rotateShip=0;
int Score=0;
int alienLife=100;
int GameLvl= 1;
float mouseX ,mouseY ;				//Cursor coordinates;
float LaserAngle=0 ,stoneAngle =0,lineWidth = 1;
float xOne=0,yOne=0;				//Spaceship coordinates
float xStone[MAX_STONES] ,yStone[MAX_STONES];//coordinates of stones
float xHealthBarStart = 1200;				//Health bar starting coodinate
GLint stoneAlive[MAX_STONES];		//check to see if stone is killed

bool mButton= false,startGame=false,gameOver=false,isLaser=false;		//boolean values to check state of the game
bool startScreen = true ,nextScreen=false,previousScreen=false;
bool gameQuit = false,instructionsGame = false, optionsGame = false;


void display();
void StoneGenerate();
void displayRasterText(float x ,float y ,float z ,char *stringToDisplay)
{
	int length;
	glRasterPos3f(x, y, z);
		length = strlen(stringToDisplay);

	for(int i=0 ;i<length ;i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24 ,stringToDisplay[i]);
	}
}
void SetDisplayMode(int modeToDisplay) {
		switch(modeToDisplay){
		case GAME_SCREEN: glClearColor(0, 0, 0, 1);break;
		case MENU_SCREEN : glClearColor(1, 0 , 0, 1);break;
	}
}
void initializeStoneArray() {
	//random stones index

	for(int i = 0;i < MAX_STONES ;i++) {
		randomStoneIndices[i]=rand()%MAX_STONE_TYPES;
		stoneAlive[i]=true;
	}

	xStone[0] = -(200*MAX_STONES)-600;             //START LINE for stone appearance

	for(int i = 0;i<MAX_STONES ;i++) {				//ramdom appearance yIndex for each stone
		yStone[i]=rand()%600;
		if(int(yStone[i])%2)
			yStone[i]*=-1;
		xStone[i+1] = xStone[i] + 200;				//xIndex of stone aligned with 200 units gap
	}
}

void DrawSpaceShipLazer() {

	glColor3f(1, 0, 0);
	glPushMatrix();
	glBegin(GL_POLYGON);           //Lazer stem
		glVertex2f(-55 ,10);
		glVertex2f(-55 ,30);
		glVertex2f(-50, 30);
		glVertex2f(-50 ,10);
	glEnd();

	float xMid =0,yMid =0;
	//Mid point of the lazer horizontal
	xMid = (55+50)/2.0;
	yMid = (25+35)/2.0;

	//Rotating about the point ,20
	glTranslated(-xMid, yMid, 0);
	glRotated(LaserAngle, 0, 0 ,1);
	glTranslated(xMid , -yMid ,0);

	//find mid point of top of lazer stem
	float midPoint = -(55+50)/2.0;

	glBegin(GL_POLYGON);           //Lazer horizontal stem
		glVertex2f(midPoint + 10 ,25);
		glVertex2f(midPoint + 10 ,35);
		glVertex2f(midPoint - 10 ,35);
		glVertex2f(midPoint - 10 ,25);
	glEnd();

	glPopMatrix();
}

void DrawStone(int StoneIndex)
{
	glPushMatrix();
	glLoadIdentity();
	switch(StoneIndex)                           //CHANGE INDEX VALUE FOR DIFFERENT STONE VARIETY;
	{
	case 0:

		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glColor3f(0.4f, 0.0f, 0.0f);
		glScalef(35,35,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(60,10,1);
		glutSolidSphere(1,5,50);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(10,60,1);
		glutSolidSphere(1,5,50);
		break;

	case 1:
		glColor3f(1.0f, 0.8f, 0.8f);
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(15,20,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(40,5,1);
		glutSolidSphere(1,5,50);
		break;

	case 2:
		glColor3f(0.2f, 0.2f, 0.0f);
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(60,25,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(25,60,1);
		glutSolidSphere(1,9,50);

		break;

	case 3:
		glColor3f(0.8f, 0.8f, 0.1f);
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(35,10,1);
		glutSolidSphere(1,10,7);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(50,20,1);
		glutSolidSphere(1,5,50);
		break;
	case 4:
		glColor3f(0.26f, 0.26f, 0.26f);
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(10,55,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(20,10,1);
		glutSolidSphere(1,9,50);
		glLoadIdentity();

		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle+45 ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(25,10,1);
		glutSolidSphere(1,9,50);
		break;
	}
	glPopMatrix();
    }
bool checkIfSpaceShipIsSafe()
{
		for(int i =0 ;i<MAX_STONES ;i++) {
		if(stoneAlive[i]&((xOne >= (xStone[i]/2 -70) && xOne <= (xStone[i]/2 + 70) && yOne >= (yStone[i]/2 -18 ) && yOne <= (yStone[i]/2 + 53)) || (yOne <= (yStone[i]/2 - 20) && yOne >= (yStone[i]/2 - 90) && xOne >= (xStone[i]/2 - 40) && xOne <= (xStone[i]/2 + 40))))
		{
			stoneAlive[i]=0;
			return false;
		}
	}
	return true;
}
void DrawLazerBeam() {



	float mouseXEnd = -((- mouseX) + xOne);
	float mouseYEnd = -((- mouseY) + yOne);
	glLineWidth(5);   //----Laser beam width

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
		glVertex2f(10,30);
		glVertex2f(mouseXEnd ,mouseYEnd);
	glEnd();
	glLineWidth(1);
}
void SpaceshipCreate(){
	{
    glPushMatrix();
    glTranslated(xOne,yOne,0);
    glScalef(1,1,0);
    glRotatef(rotateShip,0,0,1);
    glBegin(GL_TRIANGLES);//head
    glColor3f(0.0f,.63f,.90f);
    glVertex2f(10,30);
    glVertex2f(100,0);
    glVertex2f(100,60);
    glEnd();

    glColor3f(1,.1,.14);
    glBegin(GL_QUADS);//body

        glVertex2f(100,0);
        glVertex2f(300,0);
        glVertex2f(300,60);
        glVertex2f(100,60);
        glEnd();

    glColor3f(1,.1,.14);
    glBegin(GL_POLYGON);//wingup

    glVertex2f(180,60);
    glVertex2f(230,60);
    glVertex2f(230,150);
    glVertex2f(180,150);
     glEnd();

    glColor3f(0,0,0);
    glBegin(GL_POLYGON);//wingup"

    glVertex2f(180,60);
    glVertex2f(210,130);
    glVertex2f(180,150);


     glEnd();

    glColor3f(1,.1,.14);
    glBegin(GL_POLYGON);//wingdown

    glVertex2f(180,0);
    glVertex2f(230,0);
    glVertex2f(230,-90);
    glVertex2f(180,-90);

    glEnd();


       glColor3f(0,0,0);
        glBegin(GL_POLYGON);//wingdown"

    glVertex2f(180,0);
    glVertex2f(210,-70);
    glVertex2f(180,-90);

     glEnd();

        glBegin(GL_POLYGON);//boost

      glColor3f(1,.1,.14);
    glVertex2f(300,15);

    glColor3f(1,0,0);
    glVertex2f(340,-10);

    glColor3f(1,1,0);
    glVertex2f(410,30);

    glColor3f(1,1,0);
    glVertex2f(340,70);

     glColor3f(1,.1,.14);
    glVertex2f(300,45);

     glEnd();
       glLineWidth(3);
       glColor3f(0,0,0);
    glBegin(GL_LINE_STRIP);
        glVertex2f(10,30);
        glVertex2f(100,60);
        glVertex2f(300,60);
        glVertex2f(300,0);
        glVertex2f(100,0);
        glVertex2f(10,30);
        glVertex2f(100,0);
        glVertex2f(100,60);
glEnd();

          glColor3f(0,0,0);
    glBegin(GL_LINE_STRIP);
        glVertex2f(180,60);
        glVertex2f(210,130);
        glVertex2f(180,150);
        glVertex2f(230,150);
        glVertex2f(230,60);
        glEnd();

              glColor3f(0,0,0);
    glBegin(GL_LINE_STRIP);
        glVertex2f(180,0);
        glVertex2f(210,-70);
        glVertex2f(180,-90);
        glVertex2f(230,-90);
        glVertex2f(230,0);
        glEnd();

        glLineWidth(6);
        glColor3f(1,1,1);
    glBegin(GL_LINE_STRIP);
        glVertex2f(120,30);
        glVertex2f(170,30);
        glEnd();

            glColor3f(1,1,1);
    glBegin(GL_LINE_STRIP);
        glVertex2f(220,30);
        glVertex2f(280,30);
        glEnd();



         if(isLaser)
    {
       DrawLazerBeam();
    }
glPopMatrix();

}
}
void DisplayHealthBar() {


}
void startScreenDisplay()
{
	glLineWidth(50);
	SetDisplayMode(MENU_SCREEN);


    glColor3f(1 ,1, 1);
    displayRasterText(-100 ,500 ,0.6 ,"Main Menu");
	//glLineWidth(1);

	glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);				//START GAME PLOYGON
		glVertex3f(-200 ,300 ,0.5);
		glVertex3f(-200 ,400 ,0.5);
		glVertex3f(200 ,400 ,0.5);
		glVertex3f(200 ,300, 0.5);
	glEnd();



	glBegin(GL_POLYGON);				//INSTRUCTIONS POLYGON
		glVertex3f(-200, 50 ,0.5);
		glVertex3f(-200 ,150 ,0.5);
		glVertex3f(200 ,150 ,0.5);
		glVertex3f(200 ,50, 0.5);
	glEnd();

	glBegin(GL_POLYGON);				//QUIT POLYGON
		glVertex3f(-200 ,-200 ,0.5);
		glVertex3f(-200 ,-100 ,0.5);
		glVertex3f(200, -100 ,0.5);
		glVertex3f(200, -200 ,0.5);
	glEnd();

	if(mouseX>=-100 && mouseX<=100 && mouseY>=150 && mouseY<=200){
		glColor3f(0 ,0 ,1) ;
		if(mButton){
			startGame = true ;
			gameOver = false;
			mButton = false;
		}
	} else
		glColor3f(0 , 0, 0);

	displayRasterText(-100 ,340 ,0.4 ,"Start Game");

	if(mouseX>=-100 && mouseX<=100 && mouseY>=30 && mouseY<=80) {
		glColor3f(0 ,0 ,1);
		if(mButton){
			instructionsGame = true ;
			mButton = false;
		}
	} else
		glColor3f(0 , 0, 0);
	displayRasterText(-120 ,80 ,0.4 ,"Instructions");

	if(mouseX>=-100 && mouseX<=100 && mouseY>=-90 && mouseY<=-40){
		glColor3f(0 ,0 ,1);
		if(mButton){
			gameQuit = true ;
			mButton = false;
		}
	}
	else
		glColor3f(0 , 0, 0);
	displayRasterText(-100 ,-170 ,0.4 ,"    Quit");

}
void GameScreenDisplay()
{
	SetDisplayMode(GAME_SCREEN);
	DisplayHealthBar();
	glScalef(1, 1 ,0);
	if(alienLife)
	{
		SpaceshipCreate();
	}
	else {
		    gameOver=true;
		    instructionsGame = false;
		    startScreen = false;
	     }								//<----------------------gameover screen

	StoneGenerate();

}
void GameOverScreen()
{
	SetDisplayMode(MENU_SCREEN);
	glColor3f(0,0,0);
	glLineWidth(50);
	glBegin(GL_LINE_LOOP);               //Border
		glVertex3f(-650 ,-500 ,0.5);
		glVertex3f(-650 ,520 ,0.5);
		glVertex3f(650 ,520 ,0.5);
		glVertex3f(650 ,-500, 0.5);
	glEnd();

	glLineWidth(1);
	stoneTranslationSpeed=5;
	glColor3f(0, 1, 0);
	glBegin(GL_POLYGON);				//GAME OVER
		glVertex3f(-550 ,810,0.5);
		glVertex3f(-550 ,610 ,0.5);
		glVertex3f(550 ,610 ,0.5);
		glVertex3f(550 ,810, 0.5);
	glEnd();

	glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);				//RESTART POLYGON
		glVertex3f(-200, 50 ,0.5);
		glVertex3f(-200 ,150 ,0.5);
		glVertex3f(200 ,150 ,0.5);
		glVertex3f(200 ,50, 0.5);
	glEnd();

	glBegin(GL_POLYGON);				//QUIT POLYGON
		glVertex3f(-200 ,-200 ,0.5);
		glVertex3f(-200 ,-100 ,0.5);
		glVertex3f(200, -100 ,0.5);
		glVertex3f(200, -200 ,0.5);
	glEnd();


	displayRasterText(-300 ,640 ,0.4 ,"G A M E    O V E R ! ! !");
	glColor3f(0 , 0, 0);
	char temp[40];

	sprintf(temp,"Score : %d",Score);
	displayRasterText(-100 ,340 ,0.4 ,temp);
	char temp2[40];
//	if(atoi(highScore) < Score){
	//	writeIntoFile();
	//	sprintf(temp2 ,"Highest Score :%d" ,Score);
//	} else
//		sprintf(temp2 ,"Highest Score :%s" ,highScore);

	displayRasterText(-250 ,400 ,0.4 ,temp2);

	if(mouseX>=-100 && mouseX<=100 && mouseY>=25 && mouseY<=75){
		glColor3f(0 ,0 ,1);
		if(mButton){                                                       //Reset game default values
			startGame = true ;
			gameOver=false;
			mButton = false;
			initializeStoneArray();
			alienLife=100;
			xHealthBarStart=1200;
			Score=0;
			GameLvl=1;
			GameScreenDisplay();
		}
	} else
		glColor3f(0 , 0, 0);
	displayRasterText(-70 ,80 ,0.4 ,"Restart");

	if(mouseX>=-100 && mouseX<=100 && mouseY>=-100 && mouseY<=-50){
		glColor3f(0 ,0 ,1);
		if(mButton){
			exit(0);
			mButton= false;
		}
	}
	else
		glColor3f(0 , 0, 0);
	displayRasterText(-100 ,-170 ,0.4 ,"    Quit");

}
void StoneGenerate(){

		if(xStone[0]>=1200){      //If the last screen hits the end of screen then go to Nxt lvl
			GameLvl++;
			stoneTranslationSpeed+=3;
			Score+=50;
			initializeStoneArray();
			GameScreenDisplay();
		}

	for(int i=0; i<MAX_STONES ;i++){
		index = i;

		if(mouseX <= (xStone[i]/2+20) && mouseX >=(xStone[i]/2-20) && mouseY >= (yStone[i]/2-20) && mouseY <= (yStone[i]/2+20) && mButton){
			if(stoneAlive[i]){   // IF ALIVE KILL STONE
				stoneAlive[i]=0;
				Score++;
				if(Score%1==0) {
					stoneTranslationSpeed+=1;			//<--------------Rate of increase of game speed
				}
			}
		}
		xStone[i] += stoneTranslationSpeed;
		if(stoneAlive[i] )             //stone alive
			DrawStone(randomStoneIndices[i]);
	}
	stoneAngle+=stoneRotationSpeed;
	if(stoneAngle > 360) stoneAngle = 0;
}
void InstructionsScreenDisplay()
{

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SetDisplayMode(MENU_SCREEN);
	//colorBackground();
	glColor3f(0, 0, 0);
	displayRasterText(-700 ,400 ,0.5 ,"Key 'W' to move up.");
	displayRasterText(-700 ,300 ,0.5 ,"Key 'A' to move down.");
	displayRasterText(-700 ,200 ,0.5 ,"Key 'S' to move right.");
	displayRasterText(-700 ,100 ,0.5 ,"Key 'D' to move left.");
	displayRasterText(-700 ,0 ,0.5 ,"Key 'Q' to Rotate counter clockwise and Key 'E' to Rotate clockwise.");
	displayRasterText(-700 ,-100 ,0.5 ,"* Click Left Mouse button to destroy objects");
	displayRasterText(-700 ,-170 ,0.5 ,"*  The Objective is to obtain highest score");
	displayRasterText(-700, -240,0.5,"* Avoid getting hit by objects as object collison reduces your life ");


    glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);				//Escape To main menu polygon
		glVertex3f(-700 ,-485 ,0.5);
		glVertex3f(-700 ,-585 ,0.5);
		glVertex3f(-1100 ,-585 ,0.5);
		glVertex3f(-1100 ,-485, 0.5);
	glEnd();

	if(mouseX <= -390 && mouseX >= -500 && mouseY >= -275 && mouseY <= -250)
	{
			glColor3f(0, 0, 1);
			if(mButton)
			{
				mButton = false;
				instructionsGame = false;
				startScreenDisplay();
			}
	}
	else glColor3f(0, 0, 0);
	displayRasterText(-1000 ,-550 ,0, "Main Menu");
}
void display() {

	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0,0,1200,700);

	if(startGame && !gameOver)
		GameScreenDisplay();

	else if(instructionsGame)
		InstructionsScreenDisplay();

	else if(gameOver)
		GameOverScreen();

	//Make spaceship bigger
	else if(startScreen){

			startScreenDisplay();
			if(gameQuit || startGame || optionsGame || instructionsGame){
				//startScreen = false;

				if(startGame){
					SetDisplayMode(GAME_SCREEN);
					startScreen = false;

				} else if(gameQuit)
					exit(0);

				} else if(instructionsGame) {
					SetDisplayMode(GAME_SCREEN);
					InstructionsScreenDisplay();
				}
		}

	//Reset Scaling values
	glScalef(0.5 ,0.5 ,0);
	glFlush();
	glLoadIdentity();
	glutSwapBuffers();
}

void keys(unsigned char key, int x, int y)
{


     	if(key == 'w' ||key =='W')
     	{
     	    yOne+=SHIP_SPEED;
     	     isLaser=false;
     	}
	    if(key == 'a' || key=='A')
	    {
	        xOne-=SHIP_SPEED;
	        isLaser=false;
	    }
	   if(key == 's' || key=='S')
	    {
	       yOne-=SHIP_SPEED;
	       isLaser=false;
        }
	    if(key == 'd' || key =='D')
	    {
	       xOne+=SHIP_SPEED;

	       isLaser=false;
        }
        if(key=='q'||key =='Q')
        {
            rotateShip+=10;
        }
        if(key=='e'||key =='E')
        {
            rotateShip-=10;
        }

	display();

}
void myinit()
{
	glClearColor(0.5,0.5,0.5,0);
	glColor3f(1.0,0.0,0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    gluOrtho2D(-1200,1200,-700,700);
	glMatrixMode(GL_MODELVIEW);
}
void passiveMotionFunc(int x,int y) {

	//when mouse not clicked
	mouseX = float(x)/(m_viewport[2]/1200.0)-600.0;  //converting screen resolution to ortho 2d spec
	mouseY = -(float(y)/(m_viewport[3]/700.0)-350.0);

	display();
}
 void mouseClick(int buttonPressed ,int state ,int x, int y) {

	if(buttonPressed == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
		    mButton= true;
		    isLaser=true;

		}
	else
	{
	    mButton = false;
	    isLaser=false;
	}
	display();
}
 void UpdateColorIndexForSpaceshipLights(int value)
{
	 CI=(CI+1)%3;			//Color Index swapping to have rotation effect
	 display();
 	 glutTimerFunc(250,UpdateColorIndexForSpaceshipLights,0);
}
 void idleCallBack() {			//when no mouse or keybord pressed
	 display();
 }
int main(int argc, char** argv)
 {


	glutInit(&argc, argv);
	glutInitWindowSize(1200,700);
	glutInitWindowPosition(90 ,0);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutTimerFunc(50,UpdateColorIndexForSpaceshipLights,0);
	glutCreateWindow("THE SPACESHIP SHOOTING GAME");
	glutDisplayFunc(display);
	glutKeyboardFunc(keys);
	glutPassiveMotionFunc(passiveMotionFunc);
	glBlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA);
	glutIdleFunc(idleCallBack);
	glutMouseFunc(mouseClick);
	glGetIntegerv(GL_VIEWPORT ,m_viewport);
	myinit();
	SetDisplayMode(GAME_SCREEN);
	initializeStoneArray();
	glutMainLoop();
 }
