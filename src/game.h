#pragma once

#include <bits/stdc++.h>
#include <GL/glut.h>
#include "lib/components/car.h"
#include "lib/components/curtain.h"
#include "lib/components/tree.cpp"

class Game
{
    static float speed;
    static int score;             //score of the player , goes increasing as he moves forward.
    static int collisionDistance; // if the distance between car and object is within this value , its treated as collision
    static list<Tree> trees;
    static unsigned long long int counter; //this is used to create objects at regular intervals
    static int windowWidth, windowHeight;
    static Tree * lastCollided ; 
    static bool gameOverFlag  ; 
    static int lives;

  public:
    static void setSpeed(float x) { speed = x; }
    static void setWindowWidth(float w) { windowWidth = w; }
    static void setWindowHeight(float h) { windowHeight = h; }
    static float getSpeed() { return speed; }
    static int getWindowHeight() { return windowHeight; }
    static int getWindowWidth() { return windowWidth; }

    static void mainGameLoop() // runs every time the screen renders . Do all the game logic from here
    {

        if(gameOverFlag){
            putGameOverScreen() ; 
            return ; 
        }
        
        counter++;
        if (counter >= 100 * (1 / speed))
        {
            if (trees.size() > 30)
            { //remove trees that are out of screen for long
                trees.pop_front();
            }
            createTree();
            cout << " SCORE TILL NOW = " << score;
            score += 1;
            counter = 0;
        }

        for (auto tree = trees.begin(); tree != trees.end(); tree++)
        {
            Tree &mytree = *tree;
            mytree.makeTreeMovement();
            mytree.renderTree();
        }

        renderRoad();
        handleCollisionWithTrees();
        Car::renderCar();
        renderLives();
        FrontCurtain::handleCurtainDisplay();
    }



    static void renderLives()
    {
        glPushMatrix();
        glColor3f(1, 0, 0);
        glTranslated(-40, 80, 0);
        if (lives > 2)
        {
            glPushMatrix();
            glTranslated(-10, 0, 0);
            glutWireSphere(2, 50, 50);
            glPopMatrix();
        }
        if (lives > 1)
        {
            glPushMatrix();
            glTranslated(-20, 0, 0);
            glutWireSphere(2, 50, 50);
            glPopMatrix();
        }
        if (lives > 0)
        {

            glPushMatrix();
            glTranslated(-30, 0, 0);
            glutWireSphere(2, 50, 50);
            glPopMatrix();
        }
        glPopMatrix();
    }


    static void restartGame(){
        speed = 0.1;
        score = 0;
        collisionDistance = 5;
        lives = 3;
        gameOverFlag = false ; 
        counter = 0;
        trees.clear() ; 
    }

    static void renderScore()
    {
        // NOT WORKING !
        // glRasterPos2f(-10 , 20)  ;
        // string k = "Score is " + score ;
        // glColor3f(0,0,0) ;
        // for(int i =0 ; i < k.length() ; i++){
        //     glutBitmapCharacter(GLUT_BITMAP_8_BY_13,k[i]) ;
        // }
    }


    static void createTree()
    {
        Tree tree(speed);
        trees.push_back(tree);
    }

    static void handleCollisionWithTrees()
    { //checks if the car collides with trees
        for (auto tree = trees.begin(); tree != trees.end(); tree++)
        {
            Tree &mytree = *tree;
            if (abs(Car::getZ() - mytree.getZ()) < collisionDistance && abs(Car::getX() - mytree.getX()) < collisionDistance && &mytree!=lastCollided)
            {
                lastCollided = &mytree ; 
                cout << "COLLISION DETECTED : Score = " << score << endl;
                score -= 1;
                mytree.changeTreeColorToRed();
                lives-- ; 

                if(lives==0){
                    // Call gameover
                    gameOverFlag = true ; 
                    putGameOverScreen() ; 
                }
            }
        }
    }

    static void putGameOverScreen(){
        glPushMatrix() ; 
            string message = "GAME OVER :(  .  Score = " ; 
            cout<<message <<endl;; 

            glTranslated(-30,0,0) ; 

            glColor3f(0,0,0) ; 
            glRasterPos2d(0,0) ; 

            for(int i =0 ;i < message.length() ; i++){
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15 , message[i]) ;
            }

            string stringscore = to_string(score) ; 
            for(int i =0 ;i < stringscore.length() ; i++){
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15 , stringscore[i]) ;
            }

            glRasterPos2d(0,-10) ; 
            message = "Right Click to Restart the Game " ; 
            for(int i =0 ;i < message.length() ; i++){
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15 , message[i]) ;
            }


        glPopMatrix() ; 
    }

    static void accelerate()
    {
        speed = speed * 1.05;
        accelerateObjects();
        Car::goForward();
    }

    static void decelerate()
    {
        speed = speed * 0.95;
        decelerateObjects();
        Car::goBackward();
    }

    static void accelerateObjects()
    {
        for (auto tree = trees.begin(); tree != trees.end(); tree++)
        {
            Tree &mytree = *tree;
            mytree.setSpeed(speed); //accelerate
        }
    }

    static void decelerateObjects()
    {
        for (auto tree = trees.begin(); tree != trees.end(); tree++)
        {
            Tree &mytree = *tree;
            mytree.setSpeed(speed); //decelerate
        }
    }

    static void handleKeyPress(int key)
    {
        switch (key)
        {
        case ARROW_UP:
            accelerate();
            break;
        case ARROW_DOWN:
            decelerate();
            break;
        case ARROW_LEFT:
            Car::goLeft();
            break;
        case ARROW_RIGHT:
            Car::goRight();
            break;
        }
    }

  private:
    static void renderRoad()
    {
        glPushMatrix();
        // @sadiq draw the road within this block

        glPopMatrix();
    }

    static void moveAllTreesForward()
    {
        for (auto tree = trees.begin(); tree != trees.end(); tree++)
            (*tree).incZ();
    }

    static void moveAllTreesBackward()
    {
        for (auto tree = trees.begin(); tree != trees.end(); tree++)
            (*tree).decZ();
    }

    static void moveAllTreesLeft()
    {
        for (auto tree = trees.begin(); tree != trees.end(); tree++)
            (*tree).decX();
    }

    static void moveAllTreesRight()
    {
        for (auto tree = trees.begin(); tree != trees.end(); tree++)
        {
            (*tree).incX();
        }
    }
};

float Game::speed = 0.1;
int Game::score = 0;
int Game::windowWidth, Game::windowHeight;
int Game::collisionDistance = 5;
int Game::lives = 3;
bool Game::gameOverFlag = false ; 
Tree * Game::lastCollided ;
unsigned long long int Game::counter = 0;
list<Tree> Game::trees;
