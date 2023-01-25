#include <FEHImages.h>
#include <FEHLCD.h>
#include <FEHUtility.h>
#include <stdio.h>
#include <cmath>
#include <ctime>
#include <stdlib.h>
#include <iostream>
#include <sstream>
using namespace std;

// Prototypes for functions
void mainScreen();
void playerSelectionScreen();
void modeSelectionScreen();
void instructionsScreen();
void statsScreen();
void creditsScreen();
void play(bool);
void drawTiles();
float moveVersusComputer(int *);

// Global variables
bool click = false;
bool done = true;
float eHealth = 5;

/*
The Projectile class is what handles the movement and calculations for projectiles.

The Projectile class stores its horizontal and vertical speed as vx and vy.
The Projectile class stores its current position as x and y.
The Projectile class stores its initial position as sStart and yStart.
The Projectile class stores the path to its texture as well as and FEHImage object as path and texture.
The Projectile class stores its dimensions as rows and cols.
The Projectile class stores the distance its traveled horizontaly and vertically as xDist and yDist.
The Projectile class stores its its max displacement as range.
The Projectile class tracks if an enemy projectile has traveled its range through zFlag.

The Projectile class has 2 different constructors. The first one is used by the player and sets the initial position
relative to the position of the player. The second one is used by the enemy and sets the initial position
relative to the position of the enemy.
The draw function draws the projectile at the specified x and y using the texture.
The move function takes horizontal and vertical displacement and a flag variable as input. It then calculates the
projectile's new position and how much it has traveled and draws itself based on that. It then checks if it has travelled
more than its range. If it has, then it will reset its position and distance variables.
The getD function returns a float array that contains the distance traveled and the range of the projectile.
The getA function returns the horizontal distance that has been traveled.
The getB function returns the vertical distance that has been traveled.
The getC funtion returns the range of the projectile.
The getTop function returns the y value of the top edge of the projectile.
The getBot function returns the y value of the bottom edge of the projectile.
The getLeft function returns the x value of the left edge of the projectile.
The getRight funtion returns the x value of the right edge of the projectile.

This class was written by Alex
*/
class Projectile {
    private:
        float vx = 5, vy = 5;
        float x, y;
        float xStart, yStart;
        char path[50];
        FEHImage texture;
        int rows;
        int cols;
        float range = 100;
        float xDist = 0, yDist = 0;
        int zFlag = 0;
    public:
        Projectile(char p[50], float r)
        {
            strcpy(path, p);
            range = r;
            texture.Open(path);
            rows = texture.getLen();
            cols = texture.getWid();
            xStart = 160 - cols/2.0;
            yStart = 120 - rows / 2.0;
            x = 160 - cols/2.0;
            y = 120 - rows / 2.0;
        }
        Projectile(char p[50], int xs, int ys, float r)
        {
            strcpy(path, p);
            range = r;
            texture.Open(path);
            rows = texture.getLen();
            cols = texture.getWid();
            x = xs - cols/2.0;
            y = ys - rows / 2.0;
            xStart = xs - cols/2.0;
            yStart = ys - rows / 2.0;
            zFlag = 123;
        }

        void draw(float x, float y)
        {
            texture.Open(path);
            texture.Draw(x, y);
        }
        void move(float dx, float dy, int aFlag)
        {
            x += vx * dx * -1;
            y += vy * dy * -1;
            xDist += vx * dx * -1;
            yDist += vy * dy * -1;
            draw(x, y);
            if((sqrt(pow(xDist, 2) + pow(yDist, 2)) >= range))
            {
                x = xStart;
                y = yStart;
                xDist = 0;
                yDist = 0;
                if(zFlag == 123)
                {
                    done = true;
                }
                if(aFlag == 1)
                {
                    click = false;
                }

               
            }

           
        }
        float *getD()
        {
            float d[] = {xDist, yDist, range};
            return d;
        }
        float getA()
        {
            return xDist;
        }
        float getB()
        {
            return yDist;
        }
        float getC()
        {
            return range;
        }
        float getTop()
        {
            return y;
        }
        float getBot()
        {
            return y + rows;
        }
        float getLeft()
        {
            return x;
        }
        float getRight()
        {
            return x + cols;
        }

};

/*
The timer class acts as a stopwatcha and keeps track of time elapsed on the Proteus screen.

Each Timer object stores the initial time it was created as startTime.

The timer constructor sets startTime to the current time.
The updateTime function uses the current time to write on the screen the elapsed time since startTime.

This class was written by Trevor
*/
class Timer
{
public:
    float startTime;
    float gameTime;
    Timer(float time)
    {
        startTime = time;
        gameTime = 0;
    }
    void updateTime(float timeNow)
    {
        gameTime = timeNow - startTime;
        LCD.SetFontColor(WHITESMOKE);
        LCD.WriteAt(gameTime, 240, 15);
    }
};

/*
The Tile class is what is used to create the map and the things that the player interacts with

The Tile class has and FEHImage called texture and a path to the .pic file called path
The Tile class stores its current position as x and y
The tile class stores its dimensions in the dims array

The Tile constructor takes a position and pathName as input. It sets its current position to the position that it was given.
Then it copies the pathName to path and opens the texture using pathName, which is then drawn at the current position.
The draw function take a point as input and draws the texture there.
The move function takes a horizontal and vertical displacement as input. It then updates the tile's position and draws it
at the new position.
The getRows function returns the height of the texture.
The getCols function returns the width of the texture.
The getX function returns the x value of the texture.
The getY function returns the y value of the texture.

This class was written by Alex
*/
class Tile
{
private:
    FEHImage texture;
    char path[50];
    float x, y;
    // Tiles are 20x30
    int dims[2] = {20, 30};

public:
    // Constructor takes texture path, coords
    Tile(float x, float y, char pathName[50])
    {
        this->x = x;
        this->y = y;
        strcpy(path, pathName);
        texture.Open(pathName);
        draw(x, y);
    }
    void draw(int x, int y)
    {
        texture.Draw(x, y);
    }
    void move(float delX, float delY)
    {
        x += delX;
        y += delY;
        draw(x, y);
    }
    int getRows()
    {
        return dims[1];
    }
    int getCols()
    {
        return dims[0];
    }
    float getX()
    {
        return x;
    }
    float getY()
    {
        return y;
    }
};

/*
The enemy class hanldes the actions of the enemy charachter.

This class stores its health as health. It is 5 by default.
This class stores the position as x and y.
This class stores the enemy's displaced position as xNew and yNew.
This class stores FEHImage left, which stores the sprite of the enemy facing left.
Thic class stores a Prjoectile, bullet with a range and a texture.

The Enemy constructor takes a position as input. It determines the current time and stores it, then opens its texture to draw it.
Next, its position is set using the inputs and the enemy is drawn.
The redrawEnemy function updates the current position based on its input and redraws the charachter.
The drawEnemy function drawys the enemy at the location specified.
The move function calculates the enemy's distance from the player using its inputs and determines the angle between them.
It then checks to see if it is touching any obstacles and only updates its position if it is not. It then attempts to shoot in
the direction of the player.
The shoot function takes an x and y displacement as input and attempts to move the bullet in that direction.
The getTop function returns the y value of the top edge of the enemy.
The getBot fucntion returns the y value of the bottom edge of the enemy.
The getLeft funtion returns the x value of the left edge of the enemy.
The getRight function returns the x value of the right edge of the enemy.
The getPro function returns the character's projectile.
The takeDmg function decreases the character's health.
The isDead function checks if the character's health falls below 0.

This class was mostly written by Trevor.
*/
class Enemy
{
private:
    float health = 5;
    float x, y, xNew, yNew;
    int rows, cols;
    FEHImage left;
    float currentTime;
    Timer timer = Timer(TimeNow());
    Projectile bullet = Projectile("images/box.pic", 100);

public:
    Enemy(float xStart, float yStart)
    {
        currentTime = TimeNow();
        left.Open("images/eLeft.pic");
        rows = left.getLen();
        cols = left.getWid();
        x = xStart;
        y = yStart;
        drawEnemy(x, y);
    };
    void redrawEnemy(float delX, float delY)
    {
        x += delX;
        y += delY;
        left.Draw(x, y);
    };
    void drawEnemy(float xStart, float yStart)
    {
        left.Draw(xStart, yStart);
    };
    void move(float delX, float delY, float playerX, float playerY)
    {
        int quadrant;
        xNew = playerX - x;
        yNew = playerY - y;
        if (xNew > 0 && yNew < 0)
        {
            quadrant = 1;
        }
        else if (xNew < 0 && yNew < 0)
        {
            quadrant = 2;
        }
        else if (xNew < 0 && yNew > 0)
        {
            quadrant = 3;
        }
        else
        {
            quadrant = 4;
        }
        float theta = std::atan(yNew / xNew);
        float delYNew = std::sin(theta) * 2, delXNew = std::cos(theta) * 2;
        if (quadrant == 1)
        {
            if (delYNew > 0)
            {
                delYNew *= -1;
            }
            if (delXNew < 0)
            {
                delXNew *= -1;
            }
        }
        else if (quadrant == 2)
        {
            if (delYNew > 0)
            {
                delYNew *= -1;
            }
            if (delXNew > 0)
            {
                delXNew *= -1;
            }
        }
        else if (quadrant == 3)
        {
            if (delYNew < 0)
            {
                delYNew *= -1;
            }
            if (delXNew > 0)
            {
                delXNew *= -1;
            }
        }
        else
        {
            if (delYNew < 0)
            {
                delYNew *= -1;
            }
            if (delXNew < 0)
            {
                delXNew *= -1;
            }
        }

        delX *= -1;
        delY *= -1;
        bool above = false, below = false, isLeft = false, isRight = false, isBarrier = false;
        if (x + (delX * -1) > 590) {
            isRight = true;
        }
        if (x + (delX * -1) < 15) {
            isLeft = true;
        }
        if (y + (delY * -1) > 420) {
            below = true;
        }
        if (y + (delY * -1) < 25) {
            above = true;
        }
        if ((x + (delX * -1) < 190 && x + (delX * -1) > 90) && (y + (delY * -1) < 230 && y + (delY * -1) > 150)) {
            isBarrier = true;
        }
        if ((x + (delX * -1) < 115 && x + (delX * -1) > 30) && (y + (delY * -1) < 400 && y + (delY * -1) > 317)) {
            isBarrier = true;
        }
        if ((x + (delX * -1) < 290 && x + (delX * -1) > 190) && (y + (delY * -1) < 340 && y + (delY * -1) > 275)) {
            isBarrier = true;
        }
        if ((x + (delX * -1) < 470 && x + (delX * -1) > 370) && (y + (delY * -1) < 280 && y + (delY * -1) > 215)) {
            isBarrier = true;
        }
        if ((x + (delX * -1) < 410 && x + (delX * -1) > 345) && (y + (delY * -1) < 420 && y + (delY * -1) > 335)) {
            isBarrier = true;
        }
        if ((x + (delX * -1) < 510 && x + (delX * -1) > 410) && (y + (delY * -1) < 180 && y + (delY * -1) > 115)) {
            isBarrier = true;
        }
        if ((x + (delX * -1) < 435 && x + (delX * -1) > 385) && (y + (delY * -1) < 180 && y + (delY * -1) > 65)) {
            isBarrier = true;
        }
        if (!above && !below && !isLeft && !isRight && !isBarrier)
        {
            x += delXNew;
            y += delYNew;

        }
        left.Draw(x, y);
        if(done)
        {
            bullet = Projectile("images/box.pic", x, y, 100);
            done = false;
        }
        if(!done)
        {

            shoot(delXNew * 1, delYNew * 1);
        }
    }
    void shoot(float dx, float dy)
    {
        float *d = bullet.getD();
        if(sqrt(pow(bullet.getA(), 2) + pow(bullet.getB(), 2)) < bullet.getC() && !done)
        {
            bullet.move(dx, dy, 0);
        }

    }
    float getTop()
    {
        return y;
    }
    float getBot()
    {
        return y + rows;
    }
    float getLeft()
    {
        return x;
    }
    float getRight()
    {
        return x + cols;
    }
    Projectile getPro()
    {
        return bullet;
    }
    void takeDmg()
    {
        eHealth -= 0.1;
    }
    bool isDead()
    {
        return eHealth < 0.0;
       
    }
};
/*
The player class handles all of the actions done by the user in the multiplayer mode.

The class keeps track if which direction it is facing with isLeftImg, isBackL,isBackS, isBackR, isRightImg, isFrontR,
isFrontS, and isFrontL.
The class keeps an FEHImage for each directional sprite with backS, backL, backR, left, and right.
The class keeps an FEHImage for the floor texture with frontR, frontS, frontL, floor1, floor2, floor3, and floor4.
The class stores the character's health.
The class keeps the position of each floor with x, y, x1, y1, x2, y2, x3, y3, x4, and y4.
The class keeps a Timer as timer.
The class keeps an array of tiles as fillInTiles.
The class keeps an Enemy as enemy.
The class keeps a Projectile as bullet.
The class keeps its dimensions as rows and cols.

The constructor takes in a position as input and opens the picures that it uses. It then draws in bricks to fill the map
and initializes any variables.
The move function takes in an angle and the quadrant the mouse is in to calculate how the player should move. It then checks for
collisions, draws everything relative to the player, and attempts to shoot.
The shoot function take in a horizontal and vertical displaement as well as a flag and attempys to move bullet.
The getTop function returns the y value of the top edge.
The getBot function returns the y value of the bottom edge.
The getLeft function returns the x value of the left edge.
The getRight function returns the x value of the right edge.
The getEne function returns the enemy object within the class.
The getProj function returns bullet.
The take damage function reduces the character's health.
The isDead function checks if the charachter's health is less than 0.

This class was written by Trevor and Alex.
*/
class Player
{
private:
    bool isLeftImg = false, isBackL = false, isBackS = false, isBackR = false, isRightImg = false,
            isFrontR = false, isFrontS = false, isFrontL = false;
    FEHImage backS, backL, backR, left, right, frontR, frontS, frontL, floor1, floor2, floor3, floor4;
    FEHImage healthPic, storm1, storm2;
    float x, y, x1, y1, x2, y2, x3, y3, x4, y4;
    float x5, y5, x6, y6, x7, y7, x8, y8, x9, y9, x10, y10, x11, y11, x12, y12, x13, y13, x14, y14, 
    x15, y15, x16, y16;
    Tile *fillInTiles = (Tile *)malloc(sizeof(Tile) * 2);
    Enemy enemy = Enemy(40, 35);
    Projectile bullet = Projectile("images/box.pic", 100);
    int rows = 40;
    int cols = 40;

public:
    // Constructor takes coords
    Player(float xStart, float yStart)
    {
        backL.Open("images/backL.pic");
        backR.Open("images/backR.pic");
        backS.Open("images/backS.pic");
        left.Open("images/left.pic");
        right.Open("images/right.pic");
        frontL.Open("images/frontL.pic");
        frontR.Open("images/frontR.pic");
        frontS.Open("images/frontS.pic");
        healthPic.Open("images/100.pic");
        floor1.Open("images/floor1.pic");
        floor2.Open("images/floor2.pic");
        floor3.Open("images/floor3.pic");
        floor4.Open("images/floor4.pic");
        storm1.Open("images/storm1.pic");
        storm2.Open("images/storm2.pic");
        for (int i = 0; i < 2; i++)
        {
            fillInTiles[i] = Tile(i * 621, 240, "images/brick.pic");
        }
        health = 5;
        x = xStart;
        y = yStart;
        x1 = 0;
        y1 = 0;
        x2 = 320;
        y2 = 0;
        x3 = 0;
        y3 = 240;
        x4 = 320;
        y4 = 240;
        // image coodinates for outside of the borders
        x5 = -320; y5 = -240; x6 = 0; y6 = -240; x7 = 320; y7 = -240; x8 = 640; y8 = -240;
        x9 = -320; y9 = 480; x10 = 0; y10 = 480; x11 = 320; y11 = 480; x12 = 640; y12 = 480;
        x13 = -320; y13 = 0; x14 = -320; y14 = 240; x15 = 640; y15 = 0; x16 = 640; y16 = 240;

        isLeftImg = false; isBackL = false; isBackS = false; isBackR = false; isRightImg = false;
            isFrontR = false; isFrontS = false; isFrontL = false;
        click = true;
    }
    // public vars so they can be accessed later in the code
    Timer timer = Timer(TimeNow());
    float health = 5;

    void move(float theta, int quadrant)
    {
        // Calculate the change in the X and Y direction
        float delY = std::sin(theta) * 5, delX = std::cos(theta) * 5;

        if (quadrant == 2 || quadrant == 3)
        {
            delX *= -1;
        }
        else if (quadrant == 1 || quadrant == 4)
        {
            delY *= -1;
        }
        delX *= -1;
        delY *= -1;
       
        bool above = false, below = false, isLeft = false, isRight = false, isBarrier = false;
        if (x + (delX * -1) > 590) {
            isRight = true;
        }
        if (x + (delX * -1) < 15) {
            isLeft = true;
        }
        if (y + (delY * -1) > 420) {
            below = true;
        }
        if (y + (delY * -1) < 25) {
            above = true;
        }
        if ((x + (delX * -1) < 190 && x + (delX * -1) > 90) && (y + (delY * -1) < 230 && y + (delY * -1) > 150)) {
            isBarrier = true;
        }
        if ((x + (delX * -1) < 115 && x + (delX * -1) > 30) && (y + (delY * -1) < 400 && y + (delY * -1) > 317)) {
            isBarrier = true;
        }
        if ((x + (delX * -1) < 290 && x + (delX * -1) > 190) && (y + (delY * -1) < 340 && y + (delY * -1) > 275)) {
            isBarrier = true;
        }
        if ((x + (delX * -1) < 470 && x + (delX * -1) > 370) && (y + (delY * -1) < 280 && y + (delY * -1) > 215)) {
            isBarrier = true;
        }
        if ((x + (delX * -1) < 410 && x + (delX * -1) > 345) && (y + (delY * -1) < 420 && y + (delY * -1) > 335)) {
            isBarrier = true;
        }
        if ((x + (delX * -1) < 510 && x + (delX * -1) > 410) && (y + (delY * -1) < 180 && y + (delY * -1) > 115)) {
            isBarrier = true;
        }
        if ((x + (delX * -1) < 435 && x + (delX * -1) > 385) && (y + (delY * -1) < 180 && y + (delY * -1) > 65)) {
            isBarrier = true;
        }

        if (!above && !below && !isLeft && !isRight && !isBarrier)
        {
            x += (delX * -1);
            y += (delY * -1);
            x1 += delX;
            y1 += delY;
            x2 += delX;
            y2 += delY;
            x3 += delX;
            y3 += delY;
            x4 += delX;
            y4 += delY;
            x5 += delX;
            y5 += delY;
            x6 += delX;
            y6 += delY;
            x7 += delX;
            y7 += delY;
            x8 += delX;
            y8 += delY;
            x9 += delX;
            y9 += delY;
            x10 += delX;
            y10 += delY;
            x11 += delX;
            y11 += delY;
            x12 += delX;
            y12 += delY;
            x13 += delX;
            y13 += delY;
            x14 += delX;
            y14 += delY;
            x15 += delX;
            y15 += delY;
            x16 += delX;
            y16 += delY; 
            if (delY == 0 && delX > 0)
            {
                LCD.Clear();
                floor1.Draw(x1, y1);
                floor2.Draw(x2, y2);
                floor3.Draw(x3, y3);
                floor4.Draw(x4, y4);
                isLeftImg = true;
                isBackL = false;
                isBackS = false;
                isBackR = false;
                isRightImg = false;
                isFrontL = false;
                isFrontS = false;
                isFrontR = false;
                left.Draw(143, 103);
            }
            else if (delY == 0 && delX < 0)
            {
                LCD.Clear();
                floor1.Draw(x1, y1);
                floor2.Draw(x2, y2);
                floor3.Draw(x3, y3);
                floor4.Draw(x4, y4);
                isLeftImg = false;
                isBackL = false;
                isBackS = false;
                isBackR = false;
                isRightImg = true;
                isFrontL = false;
                isFrontS = false;
                isFrontR = false;
                right.Draw(143, 103);
            }
            else if (delY > 0 && delX < 0)
            {
                LCD.Clear();
                floor1.Draw(x1, y1);
                floor2.Draw(x2, y2);
                floor3.Draw(x3, y3);
                floor4.Draw(x4, y4);
                isLeftImg = false;
                isBackL = false;
                isBackS = false;
                isBackR = true;
                isRightImg = false;
                isFrontL = false;
                isFrontS = false;
                isFrontR = false;
                backR.Draw(139, 97);
            }
            else if (delY > 0 && delX > 0)
            {
                LCD.Clear();
                floor1.Draw(x1, y1);
                floor2.Draw(x2, y2);
                floor3.Draw(x3, y3);
                floor4.Draw(x4, y4);
                isLeftImg = false;
                isBackL = true;
                isBackS = false;
                isBackR = false;
                isRightImg = false;
                isFrontL = false;
                isFrontS = false;
                isFrontR = false;
                backL.Draw(139, 97);
            }
            else if (delY > 0 && delX == 0)
            {
                LCD.Clear();
                floor1.Draw(x1, y1);
                floor2.Draw(x2, y2);
                floor3.Draw(x3, y3);
                floor4.Draw(x4, y4);
                isLeftImg = false;
                isBackL = false;
                isBackS = false;
                isBackR = false;
                isRightImg = false;
                isFrontL = false;
                isFrontS = true;
                isFrontR = false;
                frontS.Draw(144, 102);
            }
            else if (delY < 0 && delX < 0)
            {
                LCD.Clear();
                floor1.Draw(x1, y1);
                floor2.Draw(x2, y2);
                floor3.Draw(x3, y3);
                floor4.Draw(x4, y4);
                isLeftImg = false;
                isBackL = false;
                isBackS = false;
                isBackR = false;
                isRightImg = false;
                isFrontL = false;
                isFrontS = false;
                isFrontR = true;
                frontR.Draw(139, 98);
            }
            else if (delY < 0 && delX > 0)
            {
                LCD.Clear();
                floor1.Draw(x1, y1);
                floor2.Draw(x2, y2);
                floor3.Draw(x3, y3);
                floor4.Draw(x4, y4);
                isLeftImg = false;
                isBackL = false;
                isBackS = false;
                isBackR = false;
                isRightImg = false;
                isFrontL = true;
                isFrontS = false;
                isFrontR = false;
                frontL.Draw(139, 98);
            }
            else
            {
                LCD.Clear();
                floor1.Draw(x1, y1);
                floor2.Draw(x2, y2);
                floor3.Draw(x3, y3);
                floor4.Draw(x4, y4);
                isLeftImg = false;
                isBackL = false;
                isBackS = true;
                isBackR = false;
                isRightImg = false;
                isFrontL = false;
                isFrontS = false;
                isFrontR = false;
                backS.Draw(144, 100);
            }
            for (int i = 0; i < 2; i++)
            {
                fillInTiles[i].move(delX, delY);
            }
            enemy.move(delX, delY, x, y);
            float xTrsh, yTrsh;
            LCD.ClearBuffer();
            if(LCD.Touch(&xTrsh, &yTrsh))
            {
                click = true;
            }
            // shoot(delX, delY, click);
            healthPic.Draw(10, 10);
            timer.updateTime(TimeNow());
            storm1.Draw(x5, y5);
            storm1.Draw(x6, y6);
            storm2.Draw(x7, y7);
            storm2.Draw(x8, y8);
            storm1.Draw(x9, y9);
            storm1.Draw(x10, y10);
            storm2.Draw(x11, y11);
            storm2.Draw(x12, y12);
            storm1.Draw(x13, y13);
            storm2.Draw(x14, y14);
            storm1.Draw(x15, y15);
            storm2.Draw(x16, y16);
        }
        else {
            LCD.Clear();
            floor1.Draw(x1, y1);
            floor2.Draw(x2, y2);
            floor3.Draw(x3, y3);
            floor4.Draw(x4, y4);
            if (isLeftImg == true) {
                left.Draw(143, 103);
            }
            if (isBackL == true) {
                backL.Draw(139, 97);
            }
            if (isBackS == true) {
                backS.Draw(144, 100);
            }
            if (isBackR == true) {
                backR.Draw(139, 97);
            }
            if (isRightImg == true) {
                right.Draw(143, 103);
            }
            if (isFrontL == true) {
                frontL.Draw(139, 98);
            }
            if (isFrontS == true) {
                frontS.Draw(144, 102);
            }
            if (isFrontR == true) {
                frontR.Draw(139, 98);
            }
            for (int i = 0; i < 2; i++)
            {
                fillInTiles[i].move(0, 0);
            }

            enemy.move(delX, delY, x, y);
            healthPic.Draw(10, 10);
            timer.updateTime(TimeNow());
            storm1.Draw(x5, y5);
            storm1.Draw(x6, y6);
            storm2.Draw(x7, y7);
            storm2.Draw(x8, y8);
            storm1.Draw(x9, y9);
            storm1.Draw(x10, y10);
            storm2.Draw(x11, y11);
            storm2.Draw(x12, y12);
            storm1.Draw(x13, y13);
            storm2.Draw(x14, y14);
            storm1.Draw(x15, y15);
            storm2.Draw(x16, y16);
        }
    }
    void shoot(float dx, float dy, bool click)
        {
            float *d = bullet.getD();
           
            if(sqrt(pow(d[0], 2) + pow(d[1], 2)) < d[2] && click)
            {
                bullet.move(dx, dy, 1);
            }

        }

    float getTop()
    {
        return 100;
    }
    float getBot()
    {
        return 140;
    }
    float getLeft()
    {
        return 140;
    }
    float getRight()
    {
        return 180;
    }
    Enemy getEne()
    {
        return enemy;
    }
    Projectile getProj()
    {
        return bullet;
    }
    void takeDmg()
    {
        health -= 0.1;
    }
    bool isDead()
    {
        return health < 0;
    }
};

// Function prototypes that had to be defined after class definitions
bool pHit(Projectile b, Player p);
bool eHit(Projectile b, Enemy p);


/*
The player class handles all of the actions done by the user in the single player mode.

The class keeps track if which direction it is facing with isLeftImg, isBackL,isBackS, isBackR, isRightImg, isFrontR,
isFrontS, and isFrontL.
The class keeps an FEHImage for each directional sprite with backS, backL, backR, left, and right.
The class keeps an FEHImage for the floor texture with frontR, frontS, frontL, floor1, floor2, floor3, and floor4.
The class stores the charachter's health.
The class keeps the position of each floor with x, y, x1, y1, x2, y2, x3, y3, x4, and y4.
The class keeps a Timer as timer.
The class keeps an array of tiles as fillInTiles.
The class keeps its dimensions as rows and cols.

The constructor takes in a position as input and opens the picures that it uses. It then draws in bricks to fill the map
and initializes any variables.
The move function takes in an angle and the quadrant the mouse is in to calculate how the player should move. It then checks for
collisions, draws everything relative to the player, and attempts to shoot.
The getWin functino returns isStar.

This class was written by Trevor and Alex.
*/
class SinglePlayer
{
private:
    bool isLeftImg = false, isBackL = false, isBackS = false, isBackR = false, isRightImg = false,
            isFrontR = false, isFrontS = false, isFrontL = false, collision;
    FEHImage backS, backL, backR, left, right, frontR, frontS, frontL, floor1, floor2, floor3, floor4;
    float health = 5;
    FEHImage healthPic, storm1, storm2;
    float x, y, x1, y1, x2, y2, x3, y3, x4, y4;
    float x5, y5, x6, y6, x7, y7, x8, y8, x9, y9, x10, y10, x11, y11, x12, y12, x13, y13, x14, y14, 
    x15, y15, x16, y16;
    Timer timer = Timer(TimeNow());
    Tile *fillInTiles = (Tile *)malloc(sizeof(Tile) * 2);
   
    int rows = 40;
    int cols = 40;
    bool isStar = false;
public:
    // Constructor takes texture path, coords, and collision bool
    SinglePlayer(float xStart, float yStart, bool c)
    {
        backL.Open("images/backL.pic");
        backR.Open("images/backR.pic");
        backS.Open("images/backS.pic");
        left.Open("images/left.pic");
        right.Open("images/right.pic");
        frontL.Open("images/frontL.pic");
        frontR.Open("images/frontR.pic");
        frontS.Open("images/frontS.pic");
        healthPic.Open("images/100.pic");
        floor1.Open("images/floor1.pic");
        floor2.Open("images/starFloor.pic");
        floor3.Open("images/floor3.pic");
        floor4.Open("images/floor4.pic");
        storm1.Open("images/storm1.pic");
        storm2.Open("images/storm2.pic");
        for (int i = 0; i < 2; i++)
        {
            fillInTiles[i] = Tile(i * 621, 240, "images/brick.pic");
        }
        health = 5;
        x = xStart;
        y = yStart;
        x1 = 0;
        y1 = 0;
        x2 = 320;
        y2 = 0;
        x3 = 0;
        y3 = 240;
        x4 = 320;
        y4 = 240;
        // image coodinates for outside of the borders
        x5 = -320; y5 = -240; x6 = 0; y6 = -240; x7 = 320; y7 = -240; x8 = 640; y8 = -240;
        x9 = -320; y9 = 480; x10 = 0; y10 = 480; x11 = 320; y11 = 480; x12 = 640; y12 = 480;
        x13 = -320; y13 = 0; x14 = -320; y14 = 240; x15 = 640; y15 = 0; x16 = 640; y16 = 240;
        collision = c;
        isLeftImg = false; isBackL = false; isBackS = false; isBackR = false; isRightImg = false;
            isFrontR = false; isFrontS = false; isFrontL = false;
        click = true;
    }

    void move(float theta, int quadrant)
    {
        // Calculate the change in the X and Y direction
        float delY = std::sin(theta) * 5, delX = std::cos(theta) * 5;

        if (quadrant == 2 || quadrant == 3)
        {
            delX *= -1;
        }
        else if (quadrant == 1 || quadrant == 4)
        {
            delY *= -1;
        }
        delX *= -1;
        delY *= -1;
       
        bool above = false, below = false, isLeft = false, isRight = false, isBarrier = false;
        if (x + (delX * -1) > 590) {
            isRight = true;
        }
        if (x + (delX * -1) < 15) {
            isLeft = true;
        }
        if (y + (delY * -1) > 420) {
            below = true;
        }
        if (y + (delY * -1) < 25) {
            above = true;
        }
        if ((x + (delX * -1) < 190 && x + (delX * -1) > 90) && (y + (delY * -1) < 230 && y + (delY * -1) > 150)) {
            isBarrier = true;
        }
        if ((x + (delX * -1) < 115 && x + (delX * -1) > 30) && (y + (delY * -1) < 400 && y + (delY * -1) > 317)) {
            isBarrier = true;
        }
        if ((x + (delX * -1) < 290 && x + (delX * -1) > 190) && (y + (delY * -1) < 340 && y + (delY * -1) > 275)) {
            isBarrier = true;
        }
        if ((x + (delX * -1) < 470 && x + (delX * -1) > 370) && (y + (delY * -1) < 280 && y + (delY * -1) > 215)) {
            isBarrier = true;
        }
        if ((x + (delX * -1) < 410 && x + (delX * -1) > 345) && (y + (delY * -1) < 420 && y + (delY * -1) > 335)) {
            isBarrier = true;
        }
        if ((x + (delX * -1) < 510 && x + (delX * -1) > 410) && (y + (delY * -1) < 180 && y + (delY * -1) > 115)) {
            isBarrier = true;
        }

        if ((x + (delX * -1) < 540 && x + (delX * -1) > 500) && (y + (delY * -1) < 90 && y + (delY * -1) > 50)) {
            isStar = true;
        }

        if ((x + (delX * -1) < 435 && x + (delX * -1) > 385) && (y + (delY * -1) < 180 && y + (delY * -1) > 65)) {
            isBarrier = true;
        }
        if (!above && !below && !isLeft && !isRight && !isBarrier)
        {
            x += (delX * -1);
            y += (delY * -1);
            x1 += delX;
            y1 += delY;
            x2 += delX;
            y2 += delY;
            x3 += delX;
            y3 += delY;
            x4 += delX;
            y4 += delY;
            x5 += delX;
            y5 += delY;
            x6 += delX;
            y6 += delY;
            x7 += delX;
            y7 += delY;
            x8 += delX;
            y8 += delY;
            x9 += delX;
            y9 += delY;
            x10 += delX;
            y10 += delY;
            x11 += delX;
            y11 += delY;
            x12 += delX;
            y12 += delY;
            x13 += delX;
            y13 += delY;
            x14 += delX;
            y14 += delY;
            x15 += delX;
            y15 += delY;
            x16 += delX;
            y16 += delY; 
            if (delY == 0 && delX > 0)
            {
                LCD.Clear();
                floor1.Draw(x1, y1);
                floor2.Draw(x2, y2);
                floor3.Draw(x3, y3);
                floor4.Draw(x4, y4);
                isLeftImg = true;
                isBackL = false;
                isBackS = false;
                isBackR = false;
                isRightImg = false;
                isFrontL = false;
                isFrontS = false;
                isFrontR = false;
                left.Draw(143, 103);
            }
            else if (delY == 0 && delX < 0)
            {
                LCD.Clear();
                floor1.Draw(x1, y1);
                floor2.Draw(x2, y2);
                floor3.Draw(x3, y3);
                floor4.Draw(x4, y4);
                isLeftImg = false;
                isBackL = false;
                isBackS = false;
                isBackR = false;
                isRightImg = true;
                isFrontL = false;
                isFrontS = false;
                isFrontR = false;
                right.Draw(143, 103);
            }
            else if (delY > 0 && delX < 0)
            {
                LCD.Clear();
                floor1.Draw(x1, y1);
                floor2.Draw(x2, y2);
                floor3.Draw(x3, y3);
                floor4.Draw(x4, y4);
                isLeftImg = false;
                isBackL = false;
                isBackS = false;
                isBackR = true;
                isRightImg = false;
                isFrontL = false;
                isFrontS = false;
                isFrontR = false;
                backR.Draw(139, 97);
            }
            else if (delY > 0 && delX > 0)
            {
                LCD.Clear();
                floor1.Draw(x1, y1);
                floor2.Draw(x2, y2);
                floor3.Draw(x3, y3);
                floor4.Draw(x4, y4);
                isLeftImg = false;
                isBackL = true;
                isBackS = false;
                isBackR = false;
                isRightImg = false;
                isFrontL = false;
                isFrontS = false;
                isFrontR = false;
                backL.Draw(139, 97);
            }
            else if (delY > 0 && delX == 0)
            {
                LCD.Clear();
                floor1.Draw(x1, y1);
                floor2.Draw(x2, y2);
                floor3.Draw(x3, y3);
                floor4.Draw(x4, y4);
                isLeftImg = false;
                isBackL = false;
                isBackS = false;
                isBackR = false;
                isRightImg = false;
                isFrontL = false;
                isFrontS = true;
                isFrontR = false;
                frontS.Draw(144, 102);
            }
            else if (delY < 0 && delX < 0)
            {
                LCD.Clear();
                floor1.Draw(x1, y1);
                floor2.Draw(x2, y2);
                floor3.Draw(x3, y3);
                floor4.Draw(x4, y4);
                isLeftImg = false;
                isBackL = false;
                isBackS = false;
                isBackR = false;
                isRightImg = false;
                isFrontL = false;
                isFrontS = false;
                isFrontR = true;
                frontR.Draw(139, 98);
            }
            else if (delY < 0 && delX > 0)
            {
                LCD.Clear();
                floor1.Draw(x1, y1);
                floor2.Draw(x2, y2);
                floor3.Draw(x3, y3);
                floor4.Draw(x4, y4);
                isLeftImg = false;
                isBackL = false;
                isBackS = false;
                isBackR = false;
                isRightImg = false;
                isFrontL = true;
                isFrontS = false;
                isFrontR = false;
                frontL.Draw(139, 98);
            }
            else
            {
                LCD.Clear();
                floor1.Draw(x1, y1);
                floor2.Draw(x2, y2);
                floor3.Draw(x3, y3);
                floor4.Draw(x4, y4);
                isLeftImg = false;
                isBackL = false;
                isBackS = true;
                isBackR = false;
                isRightImg = false;
                isFrontL = false;
                isFrontS = false;
                isFrontR = false;
                backS.Draw(144, 100);
            }
            for (int i = 0; i < 2; i++)
            {
                fillInTiles[i].move(delX, delY);
            }
            healthPic.Draw(10, 10);
            timer.updateTime(TimeNow());
            storm1.Draw(x5, y5);
            storm1.Draw(x6, y6);
            storm2.Draw(x7, y7);
            storm2.Draw(x8, y8);
            storm1.Draw(x9, y9);
            storm1.Draw(x10, y10);
            storm2.Draw(x11, y11);
            storm2.Draw(x12, y12);
            storm1.Draw(x13, y13);
            storm2.Draw(x14, y14);
            storm1.Draw(x15, y15);
            storm2.Draw(x16, y16);
        }
        else {
            LCD.Clear();
            floor1.Draw(x1, y1);
            floor2.Draw(x2, y2);
            floor3.Draw(x3, y3);
            floor4.Draw(x4, y4);
            if (isLeftImg == true) {
                left.Draw(143, 103);
            }
            if (isBackL == true) {
                backL.Draw(139, 97);
            }
            if (isBackS == true) {
                backS.Draw(144, 100);
            }
            if (isBackR == true) {
                backR.Draw(139, 97);
            }
            if (isRightImg == true) {
                right.Draw(143, 103);
            }
            if (isFrontL == true) {
                frontL.Draw(139, 98);
            }
            if (isFrontS == true) {
                frontS.Draw(144, 102);
            }
            if (isFrontR == true) {
                frontR.Draw(139, 98);
            }
            for (int i = 0; i < 2; i++)
            {
                fillInTiles[i].move(0, 0);
            }
            healthPic.Draw(10, 10);
            timer.updateTime(TimeNow());
            storm1.Draw(x5, y5);
            storm1.Draw(x6, y6);
            storm2.Draw(x7, y7);
            storm2.Draw(x8, y8);
            storm1.Draw(x9, y9);
            storm1.Draw(x10, y10);
            storm2.Draw(x11, y11);
            storm2.Draw(x12, y12);
            storm1.Draw(x13, y13);
            storm2.Draw(x14, y14);
            storm1.Draw(x15, y15);
            storm2.Draw(x16, y16);
        }
    }
    bool getWin()
    {
        return isStar;
    }
};


int main()
{
    mainScreen();
    while (true)
    {
        LCD.Update();
    }
    return 0;
}

/*
The mainScreen function creates the main screen and allows the user to interact with it to decide what they want to do.

This function was made by Trevor
*/
void mainScreen()
{
    float x, y, xTrsh, yTrsh;
    FEHImage bg;
    FEHImage logo;
    FEHImage playBtn;
    FEHImage instrBtn;
    FEHImage statsBtn;
    FEHImage credBtn;
    bg.Open("images/background.pic");
    logo.Open("images/logo.pic");
    playBtn.Open("images/playButton.pic");
    instrBtn.Open("images/instructionsButton.pic");
    statsBtn.Open("images/statsButton.pic");
    credBtn.Open("images/creditsButton.pic");
    bg.Draw(0, 0);
    logo.Draw(122, 24);
    playBtn.Draw(115, 95);
    instrBtn.Draw(115, 130);
    statsBtn.Draw(115, 165);
    credBtn.Draw(115, 200);
    bg.Close();
    logo.Close();
    playBtn.Close();
    instrBtn.Close();
    credBtn.Close();
    while (true)
    {
        LCD.ClearBuffer();
        while (!LCD.Touch(&x, &y))
        {
        };
        while (LCD.Touch(&xTrsh, &yTrsh))
        {
        };
        if (123 <= x && x <= 197 && 95 <= y && y <= 119)
        {
            playerSelectionScreen();
        }
        if (123 <= x && x <= 197 && 130 <= y && y <= 154)
        {
            instructionsScreen();
        }
        if (123 <= x && x <= 197 && 165 <= y && y <= 189)
        {
            statsScreen();
        }
        if (123 <= x && x <= 197 && 200 <= y && y <= 224)
        {
            creditsScreen();
        }
    }
}

/*
The playerSelectionScreen function creates the screen that the player uses to select the character.

This function was made by Trevor.
*/
void playerSelectionScreen()
{
    LCD.Clear();
    float x, y, xTrsh, yTrsh;
    FEHImage backBtn;
    FEHImage bg;
    FEHImage nSelectBtn;
    FEHImage elPSelectBtn;
    FEHImage nita;
    FEHImage elPrimo;
    bg.Open("images/characterSelect.pic");
    backBtn.Open("images/backButton.pic");
    elPSelectBtn.Open("images/selectButton.pic");
    elPrimo.Open("images/elPrimo.pic");
    bg.Draw(0, 0);
    backBtn.Draw(0, 217.5);
    elPSelectBtn.Draw(115, 169);
    elPrimo.Draw(132, 60);
    while (true)
    {
        LCD.ClearBuffer();
        while (!LCD.Touch(&x, &y))
        {
        };
        while (LCD.Touch(&xTrsh, &yTrsh))
        {
        };
        if (0 <= x && x <= 35.5 && 217.5 <= y && y <= 240)
        {
            mainScreen();
        }
        else if (115 <= x && x <= 206 && 169 <= y && y <= 196)
        {
            modeSelectionScreen();
        }
    }
}

/*
The modeSelectionScreen function creates the screen that the user uses to select the game mode.

This function was written by Trevor.
*/
void modeSelectionScreen()
{
    LCD.Clear();
    float x, y, xTrsh, yTrsh;
    FEHImage bg, backBtn;
    bool singPlayer = false;

    bg.Open("images/modeSelection.pic");
    bg.Draw(0, 0);
    backBtn.Open("images/backButton.pic");
    backBtn.Draw(0, 217.5);

    while (true)
    {
        while (!LCD.Touch(&x, &y))
        {
        };
        while (LCD.Touch(&xTrsh, &yTrsh))
        {
        };
        if (0 <= x && x <= 35.5 && 217.5 <= y && y <= 240)
        {
            playerSelectionScreen();
        }
        if (29 <= x && x <= 120 && 169 <= y && y <= 196)
        {
            singPlayer = true;
            play(singPlayer);
        }
        if (198 <= x && x <= 289 && 169 <= y && y <= 196)
        {
            play(singPlayer);
        }
    }
}

/*
The instructionsScreen function creates the instructions screen that goes over how to play the game.

This function was written by Trevor.
*/
void instructionsScreen()
{
    LCD.Clear();
    float x, y, xTrsh, yTrsh;
    FEHImage backBtn;
    FEHImage bg;
    bg.Open("images/instructions.pic");
    backBtn.Open("images/backButton.pic");
    bg.Draw(0, 0);
    backBtn.Draw(0, 217.5);
    while (true)
    {
        LCD.ClearBuffer();
        while (!LCD.Touch(&x, &y))
        {
        };
        while (LCD.Touch(&xTrsh, &yTrsh))
        {
        };
        if (0 <= x && x <= 35.5 && 217.5 <= y && y <= 240)
        {
            mainScreen();
        }
    }
}

/*
The statsScreen function creates the screen used to display statistics in the game.

This function was written by Trevor.
*/
void statsScreen()
{
    LCD.Clear();
    float x, y, xTrsh, yTrsh;
    FEHImage backBtn;
    FEHImage bg;
    bg.Open("images/highScores.pic");
    backBtn.Open("images/backButton.pic");
    bg.Draw(0, 0);
    backBtn.Draw(0, 217.5);

    LCD.WriteLine("");
    LCD.SetFontColor(WHITE);
    ifstream fptr;
    fptr.open("scores.txt");
    string initals[5];
    float times[5];
    int index = 0;
    for (int i = 0; i < 5; i++) {
        fptr >> times[i];
    }
    for (int i = 0; i < 5; i++) {
        fptr >> initals[i];
    }
    for (int i = 0; i < 5; i++) {
        string hiScore;
        stringstream stream;
        stream << (i+1) << ". " << times[i] << " seconds by " << initals[i];
        LCD.WriteAt(stream.str(), 35, 40 * (i+1));
    }
    fptr.close();
    while (true)
    {
        LCD.ClearBuffer();
        while (!LCD.Touch(&x, &y))
        {
        };
        while (LCD.Touch(&xTrsh, &yTrsh))
        {
        };
        if (0 <= x && x <= 35.5 && 217.5 <= y && y <= 240)
        {
            mainScreen();
        }
    }
}

/*
The creditsScreen function creates the screen that is used to display the credits.

This function was written by Trevor.
*/
void creditsScreen()
{
    LCD.Clear();
    float x, y, xTrsh, yTrsh;
    FEHImage backBtn;
    FEHImage bg;
    bg.Open("images/credits.pic");
    backBtn.Open("images/backButton.pic");
    bg.Draw(0, 0);
    backBtn.Draw(0, 217.5);
    while (true)
    {
        LCD.ClearBuffer();
        while (!LCD.Touch(&x, &y))
        {
        };
        while (LCD.Touch(&xTrsh, &yTrsh))
        {
        };
        if (0 <= x && x <= 35.5 && 217.5 <= y && y <= 240)
        {
            mainScreen();
        }
    }
}

/*
The play function sets up the environment for both the single player and multiplayer games.

This function was written by Trevor and Alex.
*/
void play(bool isSinglePLayer)
{
    if (isSinglePLayer == false)
    {
        LCD.Clear();
        FEHImage play1, play2, play3, playStart;
        play1.Open("images/play1.pic");
        play2.Open("images/play2.pic");
        play3.Open("images/play3.pic");
        playStart.Open("images/playStart.pic");
        play3.Draw(0,0);
        Sleep(1.0);
        play2.Draw(0,0);
        Sleep(1.0);
        play1.Draw(0,0);
        Sleep(1.0);
        playStart.Draw(0,0);
        Sleep(1.0);
        play1.Close();
        play2.Close();
        play3.Close();
        playStart.Close();        
        float x, y, xTrsh, yTrsh;
        class Player player(140, 100);
        FEHImage healthPic;
        bool healthDrawn = false;
        while (!player.isDead() && !player.getEne().isDead())
        {
            int quadrant = 0, *quadrantPointer = &quadrant;
            float theta = moveVersusComputer(quadrantPointer);
            player.move(theta, quadrant);

            if(pHit(player.getEne().getPro(), player))
            {
                player.takeDmg();
            }
            if(eHit(player.getProj(), player.getEne()))
            {
                player.getEne().takeDmg();
            }
            if (player.health <= 5 && healthDrawn == false) {
                healthPic.Open("images/100.pic");
                healthPic.Draw(10, 10);
                healthDrawn == true;
            }
            if (player.health <= 4) {
                healthPic.Open("images/80.pic");
                healthPic.Draw(10, 10);
            }
            if (player.health <= 3) {
                healthPic.Open("images/60.pic");
                healthPic.Draw(10, 10);
            }
            if (player.health <= 2) {
                healthPic.Open("images/40.pic");
                healthPic.Draw(10, 10);
            }
            if (player.health <= 1) {
                healthPic.Open("images/20.pic");
                healthPic.Draw(10, 10);
            }
        }
        if (player.getEne().isDead()) {
            ifstream fptr;
            fptr.open("scores.txt");
            float times[6];
            string initials[5];
            int index = 0;
            bool changeScores = false;
            for (int i = 0; i < 5; i++) {   
                fptr >> times[i];
            }
            for (int i = 0; i < 5; i++) {
                fptr >> initials[i];
            }
            fptr.close();
            float score = player.timer.gameTime;
            for (int i = 0; i < 5; i++) {
                if (score <= times[i]) {
                    changeScores = true;
                    times[5] = score;
                }
            }
            if (changeScores == true) {
                int size = (sizeof(times) / sizeof(times[0]));
                for (int i = 0; i < size; i++)
                    {
                        for (int index = 1; index < size; index++)
                        {
                            if (times[index] < times[index - 1])
                            {
                                float temp = times[index];
                                times[index] = times[(index - 1)];
                                times[(index - 1)] = temp;
                            }
                        }
                    }
                    ofstream fptrOut;
                    fptrOut.open("scores.txt");
                    for (int i = 0; i < 5; i++) {
                        fptrOut << times[i] << endl;
                    }
                    for (int i = 0; i < 5; i++) {
                        fptrOut << initials[i] << endl;
                    }
                    fptrOut.close();
            }
            FEHImage youWin;
            youWin.Open("images/youWin.pic");
            youWin.Draw(0,0);
            Sleep(1.5);
        }
        else {
            FEHImage youLose;
            youLose.Open("images/youLose.pic");
            youLose.Draw(0,0);
            Sleep(1.5);
        }
        click = false;
        done = true;
        eHealth = 5;
        mainScreen();
    }
    else
    {
        LCD.Clear();
        FEHImage play1, play2, play3, playStart;
        play1.Open("images/play1.pic");
        play2.Open("images/play2.pic");
        play3.Open("images/play3.pic");
        playStart.Open("images/playStart.pic");
        play3.Draw(0,0);
        Sleep(1.0);
        play2.Draw(0,0);
        Sleep(1.0);
        play1.Draw(0,0);
        Sleep(1.0);
        playStart.Draw(0,0);
        Sleep(1.0);
        play1.Close();
        play2.Close();
        play3.Close();
        playStart.Close();    
        float x, y, xTrsh, yTrsh;
        class SinglePlayer player(140, 100, true);
        while (!player.getWin())
        {
            int quadrant = 0, *quadrantPointer = &quadrant;
            float theta = moveVersusComputer(quadrantPointer);
            player.move(theta, quadrant);
        }
        if (player.getWin()) {
            FEHImage youWin;
            youWin.Open("images/youWin.pic");
            youWin.Draw(0,0);
            Sleep(1.5);
        }
        mainScreen();
    }
}

/*
The moveVersusComputer function returns the angle theta between the player and the mouse and which quadrant it is in

This funtion was written by Trevor.
*/
float moveVersusComputer(int *quadPointer)
{
    float x, y, xTrsh, yTrsh;
    LCD.ClearBuffer();
    LCD.Touch(&x, &y);
    float xNew = x - 160, yNew = 120 - y;
    float theta = std::atan(yNew / xNew);
    if (x >= 160 && y <= 120)
    {
        *quadPointer = 1;
    }
    else if (x < 160 && y <= 120)
    {
        *quadPointer = 2;
    }
    else if (x < 160 && y > 120)
    {
        *quadPointer = 3;
    }
    else
    {
        *quadPointer = 4;
    }
    return theta;
}

/*
The pHit function takes a Projectile and a Player as input and returns true if the Projectile collides with the Player.

This function was written by Alex.
*/
bool pHit(Projectile b, Player p)
{
    return p.getBot() > b.getTop() && p.getTop() < b.getBot() && p.getRight() > b.getLeft() && p.getLeft() < b.getRight();
}

/*
The eHit function takes a Projectile and a Player as input an returns true if the Projectile collides with the enemy.

This function was written by Alex.
*/
bool eHit(Projectile b, Enemy p)
{
    return p.getBot() > b.getTop() && p.getTop() < b.getBot() && p.getRight() > b.getLeft() && p.getLeft() < b.getRight();
}
