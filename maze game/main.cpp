/*
This maze game is a fun stand-alone mini project.

NOTE:
**NCURSES library should be installed and included in the linker of host machine**
Screen size 24x80 (terminal)
Please use the ~ ARROW KEYS ~ to play the game instead of w,a,s,d keys.

ABOUT:
The maze is loaded into the 2-D array via file 'level'.
Player's position is updated via changing the values contained in the array blocks
To determine if the player has won or not the players position is cross checked againts the three 'x's at the bottom of the screen.
ncurses.h provides the input/output manupulation functions as well as the predefined macros which which enable the use of
arrow keys.
The player is denoted by small case 'o'(as in october)
*/

//---------------------------------HEADER FILES---------------------------------------------------------------------------------
#include <fstream>      // to work with files.
#include <ncurses.h>    // input/output and cursor manupulation
#include<exception>
using namespace std;    //for ifstream

//--------------------------------CLASS DECLARATION----------------------------------------------------------------------------

class MAZE
{
    private: char level[24][80];// stores the maze
             int player_position_row,// a counter for rows
             player_position_column,//a counter for columns
             player_won;//attains the value 1 if player wins

    public: void menu();                        // displays starting menu
            void start();                       // loads the level into array "level"
            void display();                     // displays the game level with current position of player
            void update_player_position();      // updates the position of player as per input
            void check_if_won();                // check if the player has won or not
            wchar_t get_user_input();           // gets the input from user
};

//---------------------------------MAIN FUNCTION-------------------------------------------------------------------------------

int main()
{
    MAZE MAZE_obj;                              //create object of class MAZE
    MAZE_obj.menu();                            //call to MAZE::menu()
    return 0;                                   // returns 0 to main on successfull run
}

//---------------------------------FUNCTION DECLARATIONS-----------------------------------------------------------------------

//                                      MENU FUNCTION
/*Shows a menu with following options
1-Play game
2-EXIT
any other input other than 1 or 2 is shown as in valid
*/
void MAZE::menu()
{
    initscr();                                  //initiates screen
    raw();                                      //no need of newline character
    curs_set(0);                                //makes cursor invisible
    char choice;
    do
    {   clear();                                //clears screen
        mvprintw(11,32,"1- Play the game\n");   //menu options
        mvprintw(12,32,"2-Exit\n");
        refresh();
        choice=getch();                         //gets the choice of user

            switch(choice)                      //appropriate action based on choice
        {
            case '1': clear();
                      player_position_row=1; player_position_column=34; //sets the variables to starting position of player
                      player_won=0;                                     //sets player_won flag = 0 (false)
                      start();                                          //call to MAZE::start
                      update_player_position();                         //call to MAZE::update_player_position
                      break;

            case '2': clear();
                      mvprintw(11,32,"Exiting.............");           //to exit from program
                      refresh();                                        //refresh output window to show changes
                      getch();
                      break;

            default:  clear();
                      mvprintw(11,32,"Enter a valid input");            //detection of invalid input
                      refresh();                                        //refresh output window to show changes
                      getch();
                      break;
        }
    }while(choice!='2');

    endwin();                                                           //ends the window started by initscr()
}

//                                         START FUNCTION
/*
Loads the maze from file 'level' into a 2-D array, then closes the file.
Displays "Press 'q' to exit game at any moment"
*/
void MAZE::start()
{
    curs_set(0);
    mvprintw(12,25,"Press 'q' to exit game at any moment");
    refresh();
    getch();
    mvprintw(12,25,"\t\t\t\t\t");                               //clears "Press 'q' to exit game at any time"

    ifstream file("level.txt");                                 //file 'level' is opened, which has the maze in it
    for(int i=0;i<24;i++)
    {
          for(int j=0;j<80;j++)
          {
                file>>level[i][j];                              //maze is loaded from file into 2-D array
          }

    }
    file.close();                                               //closes file 'level'
    display();                                                  //call to MAZE::display()
}

//                                             DISPLAY FUNCTION
/*
Displays the maze along with player position
*/
void MAZE::display()
{
    initscr();                                                  //initiate screen
    raw();                                                      // no newline
    curs_set(0);                                                //cursor invisible
    clear();

    for(int i=0;i<24;i++)
    {
        for(int j=0;j<80;j++)
        {
            if (level[i][j]=='0')
                {
                    continue;                                   //does not print array bloack with 0(zero) in them
                }
            else
                {
                    mvaddch(i,j,level[i][j]);                   //prints every box with character other than 0(zero)
                }
        }
    }
    refresh();                                                  //changes take effect on output window
}
//                                            CHECK IF WON FUNCTION
/*
check whether the player has won the game or not, accordingly changes the player_won flag (1- true/0-false)
*/
void MAZE::check_if_won()
{
   if(player_position_row==22&&(player_position_column==41||player_position_column==42||player_position_column==43))
   { player_won=1;                              // player has won (true)
     curs_set(0);
     mvprintw(10,31,"\t\t\t    ");
     mvprintw(11,31,"  Congrats, you won  "); //player wins if row =22 and column's value is 41,42 or 43
     mvprintw(12,31,"\t\t\t    ");
     refresh();                                 //refreshes screen to show the output
     getch();                                   //holds the screen
    }
}

//                                         UPDATE PLAYER POSITION FUNCTION
/*
changes the player's position as per the input
calls the check_if_won function to check whether the player has won or not
*/
void MAZE::update_player_position()
{
    while(player_won==0)
    {   check_if_won();                         //check if the player has won.
        wchar_t player_input=get_user_input();  //gets user input

/*update the player postion as per the arrow key pressed*/
        if(player_input==KEY_DOWN&&level[player_position_row+1][player_position_column]!='|'&&level[player_position_row+1][player_position_column]!='-')
        {
            level[player_position_row][player_position_column]='0';
            player_position_row+=1;
            level[player_position_row][player_position_column]='o';
        }
        else if(player_input==KEY_UP&&level[player_position_row-1][player_position_column]!='|'&&level[player_position_row-1][player_position_column]!='-')
        {
            level[player_position_row][player_position_column]='0';
            player_position_row-=1;
            level[player_position_row][player_position_column]='o';
        }
        else if(player_input==KEY_LEFT&&level[player_position_row][player_position_column-1]!='|'&&level[player_position_row][player_position_column-1]!='-')
        {
            level[player_position_row][player_position_column]='0';
            player_position_column-=1;
            level[player_position_row][player_position_column]='o';
        }
        else if(player_input==KEY_RIGHT&&level[player_position_row][player_position_column+1]!='|'&&level[player_position_row][player_position_column+1]!='-')
        {
            level[player_position_row][player_position_column]='0';
            player_position_column+=1;
            level[player_position_row][player_position_column]='o';
        }
    /*exits the game if player wins or if any one out of 'q' or 'Q' is pressed*/
        else if(player_input=='q'||player_input=='Q'||player_won==1)
        {
            break;
        }
        display();          // calls MAZE::display() to show updated player position
    }
}

//                                      GET USER INPUT FUNCTION
/*
gets the input from user
*/
wchar_t MAZE::get_user_input()
{
    raw();                          // no new line
    noecho();                       // do not echo what is being typed to output screen
    curs_set(0);                    // cursor invisible
    keypad(stdscr,true);            // enables use of special keyboard keys such as arrow keys and function keys
    wchar_t input = getch();        // gets input
    return input;                   // returns input
}
