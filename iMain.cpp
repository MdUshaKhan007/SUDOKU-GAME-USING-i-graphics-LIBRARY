/*
PROJECT NAME: MAKING SUDOKU GAME USING I-GRAPHICS
AUTHOR NAME: MD. USHA KHAN
E-MAIL: mdushakhan007@gmail.com
INSTITUTION: BANGLADESH UNIVERSITY OF ENGINEERING & TECHNOLOGY
STUDENT ID: 2205065
SESSION: 2022-2023
PROJECT SUPERVISOR NAME: MUSTARI SADIA, LECTURER, BUET
ALL RIGHTS RESERVED TO THE AUTHOR
copyright @ mdushakhan007
*/

#include "iGraphics.h"
#include <windows.h> 
#include <stdio.h> 
#include <math.h> 
#include <time.h> 
#include <stdlib.h>
#include <mmsystem.h> 
#include <iostream> 
#include <string> 
#include <stdbool.h> 
#pragma comment(lib, "Winmm.lib")
using namespace std; 

///DEFINATION
#define N 9 
#define screenWidth 1080
#define screenHeight 1080
#define rows 9 
#define columns 9 
#define cellWidth 60
#define cellHeight 60

/// DECLARATION
int digit = 0;
int puzzleLoaded = 0;
int playbg = 0;
int bg = 1; 
int mouseClick = 0;
int keypress = 0; 
int musicFlag = 1;
int r = 255; 
int g = 255; 
int b = 255; 
int levelstate = 0; 
int gameTime = 0; 
int mistakesCount = 0;
int tempScore; 
int gameState = -2;
int picturesIndex = 0; 
int index_number = 0; 
int currentScore = 0;
int grid[9][9];
int subgrid[3][3];
int selectedGrid[2];
int selectedGrid2[2];
int sudoku_grid[N][N];
int user_grid[N][N];

bool mousesel = false; 

char mistakes[20];
char timeString[20]; 
char score[20];
char Name[20];
char errorMessage[100];
char errorMessage2[100] = "";

char endgame[1][50] = {"pictures\\gameEnd.bmp"};
char mainplay[1][50] = {"pictures\\main_game.bmp"};
char home[2][50] = {"pictures\\Homemenu.bmp", "pictures\\Login.bmp"};
char about[2][50] = {"pictures\\About_us_page.bmp","pictures\\back_button.bmp"};
char logininter[2][50] = {"pictures\\Login_inter.bmp", "pictures\\sudoku_logo2.bmp"};
char rules[3][50] = {"pictures\\Rules_in.bmp","pictures\\Loginbg.bmp","pictures\\back_button.bmp"};
char sound[3][50] = {"pictures\\SoundOffOn.bmp","pictures\\Loginbg.bmp","pictures\\back_button.bmp"};
char level[3][50] = {"pictures\\Game_level.bmp","pictures\\Loginbg.bmp","pictures\\back_button.bmp"};
char play[3][50] = {"pictures\\Play_Game_In.bmp","pictures\\Loginbg.bmp","pictures\\back_button.bmp"};
char leaderboard[4][50] = {"pictures\\Sample_lead.bmp","pictures\\Loginbg.bmp","pictures\\back_button.bmp"};
char levels[3][50] = {"pictures\\level\\easy.bmp", "pictures\\level\\medium.bmp", "pictures\\level\\hard.bmp"};
char lines[4][50] = {"pictures\\vertical.bmp", "pictures\\horizental.bmp","pictures\\vertical2.bmp", "pictures\\horizental2.bmp"};
char login[8][50] = {"pictures\\Play_game.bmp", "pictures\\Level.bmp","pictures\\Rules.bmp","pictures\\Leaderboard.bmp","pictures\\Sound.bmp","pictures\\Loginbg.bmp","pictures\\back_button.bmp","pictures\\About_us.bmp"};
char digits[9][50] = {"pictures\\digits\\1.bmp","pictures\\digits\\2.bmp","pictures\\digits\\3.bmp","pictures\\digits\\4.bmp","pictures\\digits\\5.bmp","pictures\\digits\\6.bmp","pictures\\digits\\7.bmp","pictures\\digits\\8.bmp","pictures\\digits\\9.bmp"};
char userdigits[9][50] = {"pictures\\userdigit\\1.bmp","pictures\\userdigit\\2.bmp","pictures\\userdigit\\3.bmp","pictures\\userdigit\\4.bmp","pictures\\userdigit\\5.bmp","pictures\\userdigit\\6.bmp","pictures\\userdigit\\7.bmp","pictures\\userdigit\\8.bmp","pictures\\userdigit\\9.bmp"};

/// FUNCTION PROTOTYPES
void resetGame();
void updateTimer();
void updateMistakes();
void saveGameResult();
void mouseClickCheck();
void initialtemplates();
void trimNewline(char *str);
void handleKey(unsigned char key);
int readPlayerData(struct PlayerData players[]);
void findTopScorers(struct PlayerData players[], int count, struct PlayerData topScorers[]);

/// STRUCTURED PLAYER DATA FOR LEADERBOARD
struct PlayerData 
{
    char name[20];
    int score;
    int level;
    char time[20];
};

/// STRUCTURE MAIN MENU OPTIONS
struct Cordinate 
{
	int x,y;
}
bCordinate[5];

/// READ TEMPLATE SUDOKU FROM FILES
void initialtemplates()
{
    FILE *file;

	if(levelstate == 0)
	{
    file = fopen("templates\\1.txt", "r");
	}
	else if(levelstate == 1)
	{
    file = fopen("templates\\2.txt", "r");
	}
	else if(levelstate == 2)
	{
    file = fopen("templates\\3.txt", "r");
	}

    if (file == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0 ; j < N; j++)
        {
            if (fscanf(file, "%d", &sudoku_grid[i][j]) != 1)
            {
                printf("Error reading file\n");
                fclose(file);
                return;
            }
        }
    }
    fclose(file);
}

/// READ PLAYERS DATUM FROM GAMERESULT
int readPlayerData(struct PlayerData players[]) 
{
    FILE *file = fopen("game_result.txt", "r");
    if (file == NULL) 
	{
        printf("Error opening file\n");
        return 0;
    }

    int count = 0;
    while (fscanf(file, "Name: %[^,], Score: %d, Level: %d, Time: %s\n", players[count].name, &players[count].score, &players[count].level, players[count].time) == 4) 
				  {
       				 count++;
  				  }

    fclose(file);
    return count;
}

/// FIND OUT THE GAME TOPSCORERS
void findTopScorers(struct PlayerData players[], int count, struct PlayerData topScorers[]) 
{
    for (int level = 0; level < 3; level++) 
	{
        int maxScore = -100;
        for (int i = 0; i < count; i++) 
		{
            if (players[i].level == level && players[i].score > maxScore) 
			{
                maxScore = players[i].score;
                topScorers[level] = players[i];
            }
        }
    }
}

/// SAVE THE GAME RESULTS, SUCH AS PLAYERS NAME, SCORE,LEVEL,TIMEING
void saveGameResult() 
{
    FILE *file = fopen("game_result.txt", "a"); 
    if (file == NULL) 
	{
        printf("Error opening file\n");
        return;
    }
    fprintf(file, "Name: %s, Score: %d, Level: %d, Time: %s\n", Name, currentScore, levelstate, timeString);
    fclose(file);
}

/// RESET THE MISTAKE COUNT, SCORE COUNT & RELOARD THE PUZZLES
void resetGame() 
{
    mistakesCount = 0;
    currentScore = 0;
    puzzleLoaded = 0; 
    errorMessage[0] = '\0'; 
}

/// IMPLEMENT THE TIMER FUNCTION
void updateTimer() 
{
 	if (gameState == 1)
	{
    	gameTime++;
		int minutes = gameTime / 60;
	    int seconds = gameTime % 60;
    	sprintf(timeString, "%02d:%02d", minutes, seconds);
	}
}

/// COUNTS THE MISTAKES & UPDATES IT
void updateMistakes() 
{
 	if (gameState == 1)
	{
    	sprintf(mistakes, "%02d", mistakesCount);
	}
}

/// CHECK IF THE PLACEMENT WAS CORRECT IN THE MAIN SUDOKU GRID 
bool isSafe(int sudoku_grid[N][N], int row, int col, int num) 
{
    for (int x = 0; x < N; x++) 
	{
        if (sudoku_grid[row][x] == num) 
		{
            return false;
        }
    }

    for (int x = 0; x < N; x++) 
	{
        if (sudoku_grid[x][col] == num) 
		{
            return false;
        }
    }

    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++)
	 {
        for (int j = 0; j < 3; j++) 
		{
            if (sudoku_grid[i + startRow][j + startCol] == num) 
			{
                return false;
            }
        }
    }

    return true; 
}

/// SHOW BMP IMAGES RESPECTIVE TO THEIR INPUTS
void Selection() 
{
	mouseClickCheck();
    for (int i = 0; i < 9; i++) 
	{ 
        for (int j = 0; j < 9; j++) 
		{ 
            int x =  i * cellWidth + 55; 
            int y =  j * cellHeight + 275; 


            if (sudoku_grid[i][j] > 0) 
			{
            	iShowBMP(x, y, digits[sudoku_grid[i][j] - 1]);
            }
        }
    }
}

/// VERIFY MOUSECLICK & UPDATES STATE ACCORDING TO WITH EACH CLICKS
void mouseClickCheck()
{
    if(mousesel)
    {
        int i = selectedGrid[0];
        int j = selectedGrid[1];
        int digit = 7 + selectedGrid2[0] - 3 * selectedGrid2[1]; 

        if(isSafe(sudoku_grid, i, j, digit))
        {
            sudoku_grid[i][j] = digit;
			errorMessage[0] = '\0';
			currentScore += 10;
        }
        else
        {
			mistakesCount++;
			currentScore -= 5;
            sprintf(errorMessage, "Error: Digit %d cannot be placed at  this position!!!\n", digit, i+1, j+1);
        }
        mousesel = false;
    }
}

/// DRAWS THE GAMESTATE
void iDraw()
{
	iClear();

	///MUSIC INITIALIZATION PROPERTIES
	if(playbg == 1 && gameState == 1)
	{
		PlaySound("sound\\playbg.wav", NULL, SND_LOOP | SND_ASYNC);
		playbg = 0;
	}
	if(mouseClick == 1)
    {
        PlaySound(TEXT("sound\\mouse-click.wav"), NULL, SND_FILENAME | SND_ASYNC);
        mouseClick = 0;
    }
    if(keypress == 1)
    {
        PlaySound(TEXT("sound\\keyboard.wav"), NULL, SND_FILENAME | SND_ASYNC);
        keypress = 0;
    }

	/// HOMEMENU DRAWS
	if(gameState == -2)
	{
		iShowBMP(0, 0, home[0]);
		iShowBMP(280, 170, home[1]);
	}

	/// LOGIN INTERFACE INITIALIZATION
	else if(gameState == -1)
	{
    	iShowBMP(0, 0, logininter[0]);
   	 	iSetColor(255,255,255);
		iText(455, 380, Name, GLUT_BITMAP_TIMES_ROMAN_24);

    	if(errorMessage2[0] != '\0') 
    	{
    	    iSetColor(255, 0, 0);
    	    iText(415, 200, errorMessage2, GLUT_BITMAP_TIMES_ROMAN_24);
   		}
	}

	/// INTERFACE FOR CHOOSING VARIOUS STATES
	else if(gameState == 0)
	{	
		iShowBMP(0, 0, login[5]);
		iShowBMP(0, 980 , login[6]);	
		{
			for(int i=0; i<5; i++)
			{
				iShowBMP2(bCordinate[i].x, bCordinate[i].y, login[i],255);
			}
		}
		iShowBMP(270,180, login[7]);
	}

	///MAIN GAME PLAY
	else  if(gameState == 1)
	{
		iShowBMP(0, 0, play[1]);
		iShowBMP(0, 0, mainplay[0]);
		iShowBMP(30, 250, play[0]);

		Selection();
		updateMistakes();

		iSetColor(0, 255, 255);
		iRectangle(selectedGrid[0]*60+55, selectedGrid[1]*60+275,60,60);
		iSetColor(255,255,255);
		iRectangle(selectedGrid2[0]*100+695, selectedGrid2[1]*100+315, 98, 100);
		
		if(levelstate == 0)
		{
			iSetColor(0,0,0);
			iText(170, 902, "EASY", GLUT_BITMAP_TIMES_ROMAN_24);
		}
		else if(levelstate == 1)
		{
			iSetColor(0,0,0);
			iText(153, 902, "MEDIUM", GLUT_BITMAP_TIMES_ROMAN_24);
		}
		else if(levelstate == 2)
		{
			iSetColor(0,0,0);
			iText(170, 902, "HARD", GLUT_BITMAP_TIMES_ROMAN_24);
		}

        iSetColor(0,0,0); 
        iText(965, 902, timeString, GLUT_BITMAP_TIMES_ROMAN_24); 

		iSetColor(0,0,0); 
        iText(475, 902, mistakes, GLUT_BITMAP_TIMES_ROMAN_24); 

		if(errorMessage[0] != '\0')
		{
			iSetColor(255,0,0);
			iText(30, 220, errorMessage, GLUT_BITMAP_TIMES_ROMAN_24);
		}

		sprintf(score, "%d", currentScore);
    	iSetColor(0, 0, 0);
    	iText(725, 902, score, GLUT_BITMAP_TIMES_ROMAN_24);

		iShowBMP(53, 452,lines[1] );
		iShowBMP(53, 630,lines[3] );
		iShowBMP(230, 273,lines[0] );
		iShowBMP(410, 273,lines[2] );
	}

	/// SELECT LEVELS
	else if(gameState == 2)
	{
		iShowBMP(0, 0, level[0]);			
	}

	/// RULES DISPLAYING
	else if(gameState == 3)
	{
		iShowBMP(0, 0, rules[0]);		
	}

	/// LEADERBOARD DISPLAYING
	else if(gameState == 4)
	{
		iShowBMP(0, 0, leaderboard[1]);
		iShowBMP(0, 0, leaderboard[0]);	
		iShowBMP(0, 980 , leaderboard[2]);	

		struct PlayerData players[100]; 
  		int count = readPlayerData(players);

 	    struct PlayerData topScorers[3];
    	findTopScorers(players, count, topScorers);

	    int nameX = 110; 
		int scoreX = 630; 
		int timeX = 885; 

		for (int i = 0; i < 3; i++) 
		{
    		int y = 545 - i * 80;
    		iText(nameX, y, topScorers[i].name, GLUT_BITMAP_TIMES_ROMAN_24);
    		char scoreText[20];
    		sprintf(scoreText, "%d", topScorers[i].score);
    		iText(scoreX, y, scoreText, GLUT_BITMAP_TIMES_ROMAN_24);
    		iText(timeX, y, topScorers[i].time, GLUT_BITMAP_TIMES_ROMAN_24);
		}
	}

	///SOUND OFF/ON 
	else if(gameState == 5)
	{
		iShowBMP(0, 0, sound[0]);	
	}	

	///ABOUT ME PAGE
	else if(gameState == 6)
	{
		iShowBMP(0, 0, about[0]);	
		iShowBMP(0, 980, about[1]);	
	}

	///GAMESCORE DISPLAY PAGE
	else if(gameState == 7)
	{	
		iShowBMP(0, 0, endgame[0]);	
		sprintf(score, "%d", tempScore);
   	 	iSetColor(255,255,255);
    	iText(530, 310, score, GLUT_BITMAP_TIMES_ROMAN_24); 
	}
	iSetColor(r,g,b);

	///PUZZLE LOAD CONDITION CHECKS
	if (gameState == 1)
    {
        if (!puzzleLoaded)
        {
            initialtemplates();
            puzzleLoaded = 1;
        }
    }
    else
    {
        puzzleLoaded = 0; 
    }
}

void iMouseMove(int mx, int my)
{
	printf("x = %d, y= %d\n",mx,my);
}

void iMouse(int button, int state, int mx, int my) 
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if(gameState == -2)
        {
			/// NAVIGATES FROM HOME PAGE TO LOGIN PAGE
            if(mx >= 280 && mx <= 280+550 && my >= 170 && my <= 170+100)
            {
                gameState = -1; 
				
            }
				if(musicFlag == 0)
				{
					mouseClick = 1;
				}
        }
		else if(gameState == -1)
        {
			/// NAVIGATES FROM LOGIN PAGE TO SELECTION PAGE
            if(mx >= 415 && mx <= 715 && my >= 280 && my <= 330)
            {
                if(strlen(Name) > 0)
				{
					gameState = 0 ; 
				}
				else 
				{
					strcpy(errorMessage2, "Please Enter Your Name!!!");
				}
            }

			/// RETURN BACK TO THE HOMEPAGE
			else if(mx >= 0 && mx <= 125 && my >= 980 && my <= 1080)
			{
				gameState--; 
			}
				if(musicFlag == 0)
				{
					mouseClick = 1;
				}
        }
        else if (gameState == 0)
        {
			/// RETURN BACK TO LOGIN PAGE
			if(mx >= 0 && mx <= 125 && my >= 980 && my <= 1080)
			{
				gameState--; 
				playbg = 0;
			}

			/// SELECT VARIOUS FUNCTIONALITIES
            if(mx >= 270 && mx <= 770) 
            {
                if(my >= 780 && my <= 880)
                {
                    gameState = 1; // Play button

					playbg = 1;
					gameTime = 0;

                }
                else if(my >= 660 && my <= 760)
                {
                    gameState = 2; // Level button					
                }
                else if(my >= 540 && my <= 640)
                {
                    gameState = 3; // Rules button
                }
                else if(my >= 420 && my <= 520)
                {
                    gameState = 4; // Leaderboard button
                }
                else if(my >= 300 && my <= 400)
                {
                    gameState = 5; // Sound button
					
                }
				else if(my >= 180 && my <= 280)
                {
                    gameState = 6; // About us	
                }
            }
				if(musicFlag == 0)
				{
					mouseClick = 1;
				}
        }

		if(gameState == 1 )
		{
			for(int i = 0; i < 9; i++)
			{
				for(int j = 0; j < 9; j++)
				{
					/// SELECT THE MAIN SUDOKU GRIDS
					if(mx >= 55 + i*60 && mx <= 115 + i * 60 && my >= 295 + j*60 && my <= 355 + j*60)
					{
						selectedGrid[0] = i; 
						selectedGrid[1] = j; 
						
					}
				}
			}

			for(int k = 0; k < 3; k++)
            {
                for(int l = 0; l < 3; l++)
                {
					/// SELECT THE DIGITS
                    if(mx >= 695 + k*100 && mx <= 795 + k*100 && my >=320 + l*100 && my <= 420 + l*100)
                    {
                        selectedGrid2[0]=k;
                        selectedGrid2[1]=l;
						mousesel = true;
                    }
                }
            }
				if(musicFlag == 0)
				{
					mouseClick = 1;
				}
		}

		if(gameState == 1)
		{
			/// SAVE THE GAME AND & NAVIGATE TO THE GAMEEND PAGE
			if(mx >= 845 && mx <= 995 && my >= 100 && my <= 210)
            {
				tempScore = currentScore;
				saveGameResult();
				resetGame();
                gameState = 7; 
            }

			///ERASE BUTTON FUNCTIONALITIES
			if(mx >= 60 && mx <= 210 && my >= 100 && my <= 210)
			{
				sudoku_grid[selectedGrid[0]][selectedGrid[1]] = 0;
			}
				if(musicFlag == 0)
				{
					mouseClick = 1;
				}
		}

		if(gameState == 2)
		{
			/// CHOOSE LEVEL BY CLICKING
			if(mx >= 315 && mx <= 765)
			{
				if(my >= 550 && my <= 700)
				{
					levelstate = 0;
				}
				else if(my >= 355 && my <= 505)
				{
					levelstate = 1;
				}
				else if(my >= 170 && my <= 320)
				{
					levelstate = 2;
				}
			}
				if(musicFlag == 0)
				{
					mouseClick = 1;
				}
		}

		if(gameState == 5)
		{
			/// SOUND OFF
			if((mx - 365)*(mx - 365) + (my - 465) * (my - 465) <= 125 * 125)
			{
				bg = 0;
				playbg = 0;
				PlaySound(0,0,0);
				musicFlag = 0;
			}

			/// SOUND ON
			if((mx - 710)*(mx - 710) + (my - 470) * (my - 470) <= 125 * 125)
			{
				musicFlag = 1;
				playbg = 1;
				PlaySound("sound\\bg.wav", NULL, SND_LOOP | SND_ASYNC);
			}
				if(musicFlag == 0)
				{
					mouseClick = 1;
				}
		}
		if(gameState == 7)
		{
			/// RETURN THE HOMEMENU FROM THE END PAGE
			if(mx >= 290 && mx <= 790 && my >= 130 && my <= 230)
			{
				gameState = 0; 
				bg = 1;
				playbg = 0;
				PlaySound("sound\\bg.wav", NULL, SND_LOOP | SND_ASYNC);
			}
				if(musicFlag == 0)
				{
					mouseClick = 1;
				}
		}

		if(gameState == 1)
		{
			/// EXIT THE GAME DIRECTLY FROM THE PLAY PAGE
			if(mx >= 0 && mx <= 125 && my >= 980 && my <= 1080)
			{
				saveGameResult();
				resetGame();
				gameState = 0 ; 

				bg = 1;
				playbg = 0;
				PlaySound("sound\\bg.wav", NULL, SND_LOOP | SND_ASYNC);
			}
				if(musicFlag == 0)
				{
					mouseClick = 1;
				}
		}

		if ((gameState == 2 || gameState == 3 ||gameState == 4 ||gameState == 5 ||gameState == 6)) 
		{
			/// RETURN TO THE SELECTION PAGE 
			if(mx >= 0 && mx <= 125 && my >= 980 && my <= 1080)
				{
					gameState = 0 ; 
				}
				if(musicFlag == 0)
				{
					mouseClick = 1;
				}
		}
	}
}

void iKeyboard(unsigned char key) 
{
    if(gameState == -1) 
	{
		/// NAME ENTERING DURING LOGIN
         if (key != '\b')
		{ 
            if (index_number < sizeof(Name) - 1) 
			{
                Name[index_number++] = key;
                Name[index_number] = '\0'; 
            }
        } 
		else 
		{
            if (index_number > 0)
			{
                Name[--index_number] = '\0';
            }
        }
		
		if(key =='\r' || key == '\n')
		{
			if(strlen(Name) > 0)
			{
				gameState = 0 ; 
			}
			else 
			{
				strcpy(errorMessage2, "Please Enter Your Name!!!");
			}
		}
				if(musicFlag == 0)
				{
					keypress = 1;
				}
    }
	else 
	{
		handleKey(key);
			if(musicFlag == 0)
				{
					keypress = 1;
				}
	}
}

/// TAKE INPUTS FROM THE KEYBOARD 
void handleKey(unsigned char key)
{
	if(gameState == 1 )
	{
		updateMistakes();

		int i = selectedGrid[0];
		int j = selectedGrid[1];

		if(key >= '1' && key <= '9')
		{
			int digit = key - '0';

			if(isSafe(sudoku_grid, i, j, digit))
            {
                sudoku_grid[i][j] = digit; 
				errorMessage[0] = '\0';
				currentScore += 10;
            }
            else
            {
				mistakesCount++; 
				currentScore -= 5;
                sprintf(errorMessage, "Error: Digit %d cannot be placed at  this position!!!\n", digit, i+1, j+1);
            }
		}
			if(musicFlag == 0)
				{
					keypress = 1;
				}
	}
}

/// NAVIGATE THE SELECTION BY UP, DOWN, LEFT, RIGHT KEY
void iSpecialKeyboard(unsigned char key) 
{
	if (key == GLUT_KEY_END) 
	{
		exit(0);
	}
	if(gameState == 1)
	{
		if(key == GLUT_KEY_RIGHT)
		{
			selectedGrid[0]++;
		}
		if(key == GLUT_KEY_LEFT)
		{
			selectedGrid[0]--;
		}
		if(key == GLUT_KEY_UP)
		{
			selectedGrid[1]++;
		}
		if(key == GLUT_KEY_DOWN)
		{
			selectedGrid[1]--;
		}
			if(musicFlag == 0)
				{
					keypress = 1;
				}
	}
}

/// THE BOSS MAIN FUNCTION 
int main() 
{
	iSetTimer(1000, updateTimer);
	int sum = 300; 
	for(int i=4; i>=0; i--)
	{
		bCordinate[i].x = 270; 
		bCordinate[i].y = sum;
		sum +=120; 
	}
	PlaySound("sound\\bg.wav", NULL, SND_LOOP | SND_ASYNC);
	Name[0] = '\0';
	index_number = 0 ; 
	iInitialize(screenWidth, screenHeight, "SUDUKO KING");
	return 0;
}

/// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!THE END!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!    ///