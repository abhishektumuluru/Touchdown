//ROT8
#include <stdio.h>
#include "text.h"
#include "player.h"
#include "mylib.h"
#include "drawImages.h"
#include "menuBackground.h"
#include "gameplayBackground.h"
#include "gameOverBackground.h"

enum GBAState
{
    //All the statesfor this game
	MAIN_MENU,
	MAIN_MENU_NODRAW,
	GAMEPLAY,
    GAMEPLAY_NODRAW,
    GAMEOVER,
    GAMEOVER_NODRAW,
};

unsigned int td = 0;
//td represents the number of touchdowns scored
unsigned int deltaTime = 0;
//deltaTime is a measure of the time. Increments 60 times a second
int main()
{
	REG_DISPCNT = MODE_3 | BG2_EN;
//states set up below
	enum GBAState state = MAIN_MENU;

    unsigned int triggered = 0;

	while(1) {
    //increment deltaTime  
        deltaTime++;
		switch(state) {
		case MAIN_MENU:
        //call the mainMenuScene function which sets up the main menu
            mainMenuScene();
   	        state = MAIN_MENU_NODRAW;
			break;
		case MAIN_MENU_NODRAW:

            if (KEY_DOWN_NOW(BUTTON_A)) 
            {
                if(!triggered) {
                    triggered = 1;
                    state = GAMEPLAY;
                }
            } else 
            {
                triggered = 0;
            }
            break;

        case GAMEPLAY:
        //call the mainMenuScene function which sets up the gameplay
            gamePlayScene();
    		state = GAMEPLAY_NODRAW;
			break;

        case GAMEPLAY_NODRAW:
            if (KEY_DOWN_NOW(BUTTON_SELECT)) 
            {
                //mapped to ENTER
                //select button to reset at any time
                if(!triggered) {
                    triggered = 1;
                    state = MAIN_MENU;
                }
            }
            if(!triggered) {
                    triggered = 1;
                    state = GAMEOVER;
                }
             else 
            {
                triggered = 0;
            }
            break;

        case GAMEOVER:
        //call the mainMenuScene function which sets up the end of the game
            gameOverScene();
			state = GAMEOVER_NODRAW;
			break;
	    case GAMEOVER_NODRAW:
        //select button to reset at any time
            if (KEY_DOWN_NOW(BUTTON_SELECT)) 
            {
                if(!triggered) {
                    triggered = 1;
                    state = MAIN_MENU;
                }
            }
	        else if (KEY_DOWN_NOW(BUTTON_A)) 
            {
                if(!triggered) {
                    triggered = 1;
                    state = GAMEPLAY;
                }
            } else 
            {
                triggered = 0;
            }
            break;
		}

	}
    waitForVBlank();
	return 0;
}


void waitForVBlank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}


void mainMenuScene()
{
    //function that setups up the main menu
    updateMenu();
}

void updateMenu()
{
    //function that sets up and draws the menu

    drawImage3(0, 0, 240, 160, menuBackground);

    drawString(50, 90, "TOUCHDOWN", BLUE);

    drawString(65, 20, "Stay out of reach of the defenders!", BLACK);


    drawString(85, 40, "Stay safely in end zones", BLACK);
    drawString(95, 70, "for bonus points.", BLACK);
    drawString(115, 35, "Use Arrows to direct movement.", BLACK);

    drawString(125, 60, "Press A to continue", RED);
}

void setupSaws()
{
    //function that sets up player and opponent players
        
    Saw saws[NUM_SAWS];
    Saw oldSaws[NUM_SAWS];
    //have an array of players named saws
    int size = 16;
    Saw *cur;
    int speeds[] = {-2, -1, 1, 2};
    int numSpeeds = sizeof(speeds)/sizeof(speeds[0]);
    Player p = {70, 20, 16, 16, 1};
    Player oldP = p;
    int speed = 6;

    setBG(GREEN);

    for(int i = 0; i < NUM_SAWS; i++)
    {
        saws[i].x = rand() % 20 + 70;
        saws[i].y = rand() % 20 + 110;
        saws[i].rd =  speeds[rand()%numSpeeds]; 
        saws[i].cd =  speeds[rand()%numSpeeds];
        oldSaws[i] = saws[i];
        //pick a random speed for each opponent
        
    }
    while (1) 
    {
        for(int i=0; i<NUM_SAWS; i++)
        {
            //move the opponents by increasing their x and y positions
            cur = saws + i;   
            cur->y += cur->cd;

            cur->x += cur->rd;
            if(cur->x < 0)
            {
                cur->x = 0;
                cur->rd = -cur->rd;
            }
            if(cur->x > 159-size)
            {
                cur->x = 159-size;
                cur->rd = -cur->rd;
            }
            if(cur->y < 0)
            {
                cur->y = 0;
                cur->cd = -cur->cd;
            }
            if(cur->y > 239-size)
            {
                cur->y = 239-size;
                cur->cd = -cur->cd;
            }

    
        }
        waitForVBlank();
        for (int j = 0; j < NUM_SAWS; j++)
        {
            //draw a rectangle with the same color as the background
            //to cover old drawings
            drawRectangle(oldSaws[j].x, oldSaws[j].y, 16, 16, GREEN);
            drawRectangle(oldSaws[j].x + 2, oldSaws[j].y, 16, 16, GREEN);

        }
        for (int k = 0; k < NUM_SAWS; k++)
        {
            cur = saws + k;
            //draw the opponent player
            drawImage3(cur->x, cur->y, 16, 16, player);
            if (structCollision(p, oldSaws[k]))
            {
                return;
            }
            oldSaws[k] = saws[k];
        }
        if (p.y <= p.height - 5) {
            td++;
            //increase the touchdown score if the player is in the end zone
        }
        if (p.y >= 240 - p.height)
        {
            td++;
            //increase the touchdown score if the player is in the end zone
        }

        //check the bounds of the player to make sure he/she can't exit the screen
        if (KEY_DOWN_NOW(BUTTON_RIGHT))
        {
            int disp = speed;
            p.y += disp;
            if (p.y >= 240 - p.height)
            {
                p.y = 224;
            }
        }
        if (KEY_DOWN_NOW(BUTTON_LEFT))
        {
            int disp = speed;
            p.y -= disp;
            if (p.y <= p.height - 5)
            {
                p.y = 0;
                //td++;
            }
        }
        if (KEY_DOWN_NOW(BUTTON_UP))
        {
            int disp = speed;
            p.x -= disp;
            if (p.x <= 0)
            {
                p.x = 0;
            }
        }
        if (KEY_DOWN_NOW(BUTTON_DOWN))
        {
            int disp = speed;
            p.x += disp;
            if (p.x >= 160 - p.width)
            {
                p.x = 144;
            }
            
        }
        
        if (KEY_DOWN_NOW(BUTTON_SELECT))
        {
            //mapped to ENTER
            break;
        }

        //draw the midfield and endzone Lines
        drawRectangle(0, 120, 160, 2, WHITE);

        drawRectangle(0, 230, 160, 5, WHITE);
        
        drawRectangle(0, 10, 160, 5, WHITE);

        //cover the old user player with a rectangle
        drawRectangle(oldP.x, oldP.y, 16, 16, GREEN);
        
        oldP = p;

        //draw the current user player
        drawRectangle(p.x, p.y, 16, 16, RED);
        
        //drawing touchdown score

        drawString(0, 130, "Touchdowns:", WHITE);

        char tdBuffer[12];
        sprintf(tdBuffer, "%d", td);
        drawRectangle(0, 200, 20, 20, GREEN);
        drawString(0, 200, tdBuffer, WHITE);
    }   
}

void gamePlayScene()
{
    //initialize the deltaTime and touchdown score to 0
    deltaTime = 0;
    td = 0;
    //call the setupSaws function
    setupSaws();    
}


void gameOverScene()
{

    //function that organizes the gameOver scene

    //draw the score to the screen
    char scoreBuffer[10];
    sprintf(scoreBuffer, "%d", deltaTime/100 + td);

    waitForVBlank();

    //draw the background
    drawImage3(1, 0, 240, 160, gameOverBackground);

    drawString(30, 60, "YOU GOT TAKEN DOWN!", RED);


    drawString(60, 25, "Victory is granted to the worthy.", BLACK);


    drawString(80, 55, "Press A to Try Again!", BLACK);

    if (td > 500)
    {
        drawString(115, 85, "SCORE:", YELLOW);
        drawString(115, 130, scoreBuffer, YELLOW);
        drawString(130, 80, "IMPRESSIVE!", RED);    
    }
    else if (deltaTime / 100 > 2000)
    {
        drawString(115, 85, "Only A Few Have Won.", GREEN);
    }
    else
    {
        drawString(120, 65, "FINAL SCORE:", YELLOW);
        drawString(120, 145, scoreBuffer, YELLOW);
    }
}


int onCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    //method that checks if two objects with coordinates and heights and widths are colloding
    //returns 0 if false
	return ((x1 < x2 + w2)
		&& (x1 + w1 > x2)
		&& (y1 < y2 + h2)
		&& (h1 + y1 > y2));
}


int structCollision(Player p, Saw s) {
    //checks if a user player and the opponent player are colloding (saw)
    return onCollision(p.x, p.y, 16, 16, s.x, s.y, 16, 16);
}