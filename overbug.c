#include <stdio.h>
#include <strings.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

// REMOVE GAMEPIECE PARAMETER ON FUNCTION FREEOROCCUPIED

//Numerical Constants
#define MAX_ROWS 7
#define MAX_COLS 5
#define MAX_PIECES 5

//Font Color
//if 48 background, if 38 font-color
// printf("\033[48;5;127m \033[38;5;1m");
// ANSI Characters
#define BOX 254

//Position of Each ordered pairs in the board
typedef struct orderedPairs{
    int nRow;
    int nCol;
    int sort;
    bool eaten;
} coordinates;

//Gameboard Declaration
typedef struct Battleships{
    bool shadedTile[MAX_ROWS][MAX_COLS];
    coordinates alpha[MAX_PIECES]; // array of pieces, data type contains row and column
    coordinates beta[MAX_PIECES];
    // int alphaAlive;
    // int betaAlive;
} board;

// game design functions
void gotoxy(int x, int y);
void boxMaker(int tbLeft, int tbRight, int yTopHor, int yBotHor, int iLHor, int iRHor, int iLVer, int iRVer);
void homeMenu();
void welcomeMenu();
void rulesMenu();

board intilializeGameBoard(board freshGameBoard);
void nullBoard(board gameBoard);
coordinates getPlayerPiece(bool aTurn, coordinates gamePiece);
coordinates getNextMove(bool aTurn, coordinates gamePiece, coordinates nextMove);
int getTokenIndex(bool aTurn, coordinates gamePiece, board gameBoard);
board movePiece(bool aTurn, coordinates gamePiece, coordinates nextMove, board gameBoard);
bool validatePiece(coordinates gamePiece, bool aTurn, board gameBoard); // utilized by getPlayerPiece()
bool validateNextMove(bool aTurn, coordinates gamePiece, coordinates nextMove, board gameBoard); // utilized by getNextMove()

// Validation 
bool checkMove(bool aTurn, coordinates gamePiece, coordinates nextMove, board gameBoard); // 1st Validation for Moving Forward or Diagonal but not backward or forward
bool FreeOrOccupied( coordinates nextMove, board gameBoard); // 2nd Validation 
bool takeShaded(bool aTurn, coordinates nextMove, board gameBoard); // 3rd Validation for Eating Chess Pieces
bool gameOver(board gameBoard);

void congratulationsAlpha();
void congratulationsBeta();

int main(void){
    int l, k;
    int test = 0;
    char menu;
    char instance;
    
    //System Initilizations
    bool aTurn = true;
    bool over = false;

    // do-while loop initializations
    bool validPiece = false;
    bool validNextPiece = false;

    board gameBoard;

    // gameBoard.alphaAlive = 5;
    // gameBoard.betaAlive = 5;

    coordinates gamePiece;
    coordinates nextMove;

    //Initilize a fresh gameboard with initial positions
    gameBoard = intilializeGameBoard(gameBoard);

    welcomeMenu();

    do
    {
        homeMenu();
        menu = getch();
        menu = toupper(menu);

        switch (menu)
        {
            case 'S':
                do
                {
                    //printf("CLearScreen at Menu\n"); sleep(1);
                    system ("cls"); // clear screen

                    nullBoard(gameBoard);

                    //Get Input from User
                    if (aTurn) {
                        gotoxy (36, 6); printf("\e[48;5;70m"); printf("PLAYER ALPHA'S TURN\n"); printf("\e[0m");
                    } else {
                        gotoxy (36, 6); printf("\e[48;5;124m"); printf("PLAYER BETA'S TURN\n"); printf("\e[0m");
                    }

                    // This is for checking if we need to get the piece of the player
                    do
                    { 
                        gamePiece = getPlayerPiece(aTurn, gamePiece);
                        validPiece = validatePiece(gamePiece, aTurn, gameBoard);
                        if (validPiece == false) {
                            for (int i = 9; i < 12; i++)
                            {
                                gotoxy(36, i); printf("                            ");
                            }
                            
                            gotoxy(36,19); printf("                                ");
                            gotoxy(36, 19); printf("\e[0;91m"); printf("INVALID SELECTION! PLEASE RETRY.");  
                        }
                        //validate game piece
                    } while (validPiece == false);

                    do 
                    {
                        nextMove = getNextMove(aTurn, gamePiece, nextMove);
                        validNextPiece = validateNextMove(aTurn, gamePiece, nextMove, gameBoard);
                        if (validNextPiece == false) {

                            for (int i = 14; i < 18; i++)
                            {
                                gotoxy(36, i); printf("                            ");
                            }
                            gotoxy(36,19); printf("                                ");
                            gotoxy(36, 19); printf("\e[0;91m"); printf("INVALID MOVE! PLEASE RETRY.");  
                            // getch();
                        }
                    } while(validNextPiece == false); 
                    
                    gameBoard = movePiece(aTurn, gamePiece, nextMove, gameBoard);
                    aTurn = !aTurn;
                    test++;
                    printf("\n");

                    over = gameOver(gameBoard);
                    if(over == true){
                        menu = 'X';
                    }

                } while(over == false);
                break;
            
            case 'R':
                
                homeMenu();
                rulesMenu();
                break;

            case 'X':
                // exit
                break;
        }    
        //printf("clearscreen at end of while loop MM\n"); sleep(1); system("cls");
    } while(menu != 'X');

    return 0;
}

/* Initialize */
board intilializeGameBoard(board freshGameBoard)
{
    int i, j, k, l;
    int aSorter = 0, bSorter = 0;
    int alphaToken = 0, betaToken = 0;

    //Initilize Shaded Tile and White Tile
    for (i = 0; i < MAX_ROWS; i++) {
        for (j = 0; j < MAX_COLS; j++) {
            //If statement is true, set that coordinate to a shaded tile
            if (i % 2 == j % 2){
                freshGameBoard.shadedTile[i][j] = true;
            }
            //If statement is false, set that coordinate to a white tile
            else{
                freshGameBoard.shadedTile[i][j] = false;
            }
        }
    }

    /* Initialize the Coordinates for Alpha Piece and Beta Piece */
    for (i = 0; i < MAX_ROWS; i++){
        for (j = 0; j < MAX_COLS; j++) {
            /* Initialize the Player Pieces in a Shaded Tile */
            if (freshGameBoard.shadedTile[i][j] == true){
                /* Initialize starting (x,y) position of Beta */
                if(i < 2){
                    freshGameBoard.beta[betaToken].nRow = i;
                    freshGameBoard.beta[betaToken].nCol = j;
                    freshGameBoard.beta[betaToken].sort = bSorter+1;
                    freshGameBoard.beta[betaToken].eaten = false;
                    betaToken++;
                    bSorter++;
                }
                /* Initialize starting (x,y) position of Alpha */           
                else if(i > 4){
                    freshGameBoard.alpha[alphaToken].nRow = i;
                    freshGameBoard.alpha[alphaToken].nCol = j;
                    freshGameBoard.alpha[alphaToken].sort = aSorter+1;
                    freshGameBoard.alpha[alphaToken].eaten = false;
                    alphaToken++;
                    aSorter++;
                }

            }

        }
    }
    //For Readability
    return freshGameBoard;
}

coordinates getPlayerPiece(bool aTurn, coordinates gamePiece){
    char xLetter;

    /* Get the Piece-Index from the Player that will be stored in gamePiece */
    gotoxy(36, 8); printf("\e[0;36m"); printf("SELECT PIECE TO MOVE"); printf("\e[0m");
    do
    {
        gotoxy (36, 10); printf("\e[0;33m"); printf("ROW: "); scanf(" %c", &xLetter);
        xLetter = toupper(xLetter);
        if (xLetter != 'A' && xLetter != 'B' && xLetter != 'C' && xLetter != 'D' && xLetter != 'E' && xLetter != 'F' && xLetter != 'G')
        {
            gotoxy(41, 10); printf("                      ");
        }
    } while(xLetter != 'A' && xLetter != 'B' && xLetter != 'C' && xLetter != 'D' && xLetter != 'E' && xLetter != 'F' && xLetter != 'G');
    do
    {
        gotoxy (36, 11); printf("\e[38;5;208m"); printf("COLUMN: "); scanf("%d", &gamePiece.nCol);
        if (gamePiece.nCol < 1 || gamePiece.nCol > 5)
        {
            gotoxy(44, 11); printf("                      ");
        }
    } while(gamePiece.nCol < 1 || gamePiece.nCol > 5);
    
    switch (xLetter){
        case 'A': gamePiece.nRow = 0; break;
        case 'B': gamePiece.nRow = 1; break;
        case 'C': gamePiece.nRow = 2; break;
        case 'D': gamePiece.nRow = 3; break;
        case 'E': gamePiece.nRow = 4; break;
        case 'F': gamePiece.nRow = 5; break;
        case 'G': gamePiece.nRow = 6; break;
    }
    gamePiece.nCol--;
    
    return gamePiece;
}

coordinates getNextMove(bool aTurn, coordinates gamePiece, coordinates nextMove)
{
    char xLetter;

    /* Get Coordinates from Player on where the next-move will be, stored in nextMove */

    gotoxy(36, 13); printf("\e[0;36m"); printf("SELECT PIECE TO GO TO"); printf("\e[0m");
    do
    {
        gotoxy(36, 15);printf("\e[0;33m"); printf("ROW: "); scanf(" %c", &xLetter);
        xLetter = toupper(xLetter);
        if (xLetter != 'A' && xLetter != 'B' && xLetter != 'C' && xLetter != 'D' && xLetter != 'E' && xLetter != 'F' && xLetter != 'G')
        {
            gotoxy(41, 15); printf("                      ");
        }
    }while(xLetter != 'A' && xLetter != 'B' && xLetter != 'C' && xLetter != 'D' && xLetter != 'E' && xLetter != 'F' && xLetter != 'G');
    do
    {
        gotoxy(36, 16); printf("\e[38;5;208m"); printf("COLUMN: ");scanf("%d", &nextMove.nCol);
        if (nextMove.nCol < 1 || nextMove.nCol > 5)
        {
            gotoxy(44, 16); printf("                      ");
        }
    }while(nextMove.nCol < 1 || nextMove.nCol > 5);
    switch (xLetter){
        case 'A': nextMove.nRow = 0; break;
        case 'B': nextMove.nRow = 1; break;
        case 'C': nextMove.nRow = 2; break;
        case 'D': nextMove.nRow = 3; break;
        case 'E': nextMove.nRow = 4; break;
        case 'F': nextMove.nRow = 5; break;
        case 'G': nextMove.nRow = 6; break;
    }
    nextMove.nCol--;

    return nextMove;
}

int getTokenIndex(bool aTurn, coordinates gamePiece, board gameBoard) // params
{
    // declare variables
    int i = 0, j = 0;
    int tokenIndex;
    int found = 0;

    if(aTurn)
    {
        // Get the (x,y) of alpha piece
        while(found == 0){
            if (gameBoard.alpha[i].nRow == gamePiece.nRow && gameBoard.alpha[i].nCol == gamePiece.nCol) {
                tokenIndex = i;
                found = 1;
            }
            else
            {
                i++;
            }
        }
    } 
    else
    {
        // Get the (x,y) of beta piece
        while(found == 0){
            if (gameBoard.beta[j].nRow == gamePiece.nRow && gameBoard.beta[j].nCol == gamePiece.nCol) {
                tokenIndex = j;
                found = 1;
            }
            else
            {
                j++; 
            }
        }
    }
    
    
    return tokenIndex;
}

board movePiece(bool aTurn, coordinates gamePiece, coordinates nextMove, board gameBoard){

    bool isOccupied = false, canAttack = false; 
    bool validNextMove = false;
    bool checkOccupied = false;
    int tokenIndex = getTokenIndex(aTurn, gamePiece, gameBoard);
    int i, j, k;
    
    /* If aTurn is False, then it is Alpha's Turn to Play */
    if(aTurn){
        
        // validNextMove = Validation 1
        validNextMove = checkMove(aTurn, gamePiece, nextMove, gameBoard);

        // validNextMove = Validation 2
        if(checkOccupied == true)
        {
        checkOccupied = FreeOrOccupied( nextMove, gameBoard);
        }

        //Validation 3
        if(checkOccupied == true) //will run if occupied ang tiles, no matter if may piece alpha piece or beta piece na occupied
        {
            validNextMove = takeShaded(aTurn, nextMove, gameBoard);
        }

        /* Move to the COORDINATES fetched from nextMove*() of the index of chosen Piece*/
        gameBoard.alpha[tokenIndex].nRow = nextMove.nRow;
        gameBoard.alpha[tokenIndex].nCol = nextMove.nCol;
            //if we say this is true, nasa shaded tile ung kakain, tas ung nakain, set eaten to true
            if(validNextMove == true){
                
                for(i = 0; i < MAX_PIECES; i++){
                    if(gameBoard.beta[i].nRow == nextMove.nRow && gameBoard.beta[i].nCol == nextMove.nCol){
                        gameBoard.beta[i].eaten = true;
                        // printf("gameboard.beta[%d].eaten is true!\n", gameBoard.beta[i]);
                        // sleep(2);
                        // gameBoard.betaAlive -= 1;
                    }
                }
            }
        /* Declare the previous COORDINATES to the COORDINATES of Alpha */
        nextMove.nRow = gameBoard.alpha[gamePiece.nRow].nRow;
        nextMove.nCol = gameBoard.alpha[gamePiece.nCol].nRow;
    }
    
    /* If aTurn is False, then it is Beta's Turn to Play */
    else if(aTurn == false){
        
        // validNextMove = Validation 1
        validNextMove = checkMove(aTurn, gamePiece, nextMove, gameBoard);

        // validNextMove = Validation 2
        checkOccupied = FreeOrOccupied( nextMove, gameBoard);

        //Validation 3
        if(checkOccupied == true) //will run if occupied ang tiles, no matter if may piece alpha piece or beta piece na occupied
        {
            validNextMove = takeShaded(aTurn, nextMove, gameBoard);
        }

        /* Move to the COORDINATES fetched from nextMove*() of the index of chosen Piece*/
        gameBoard.beta[tokenIndex].nRow = nextMove.nRow;
        gameBoard.beta[tokenIndex].nCol = nextMove.nCol;
        //if we say this is true, nasa shaded tile ung kakain, tas ung nakain, set eaten to true
        if(validNextMove == true){
            // printf("ValidNextMove is true!\n");
            // sleep(2);
            for(i = 0; i < MAX_PIECES; i++)
            {
                if(gameBoard.alpha[i].nRow == nextMove.nRow && gameBoard.alpha[i].nCol == nextMove.nCol){
                    gameBoard.alpha[i].eaten = true;
                    // printf("gameboard.alpha[%d].eaten is true!\n", gameBoard.alpha[i]);
                    // sleep(2);
                    // gameBoard.alphaAlive -= 1;
                }
            }
        }
        /* Declare the previous COORDINATES to the COORDINATES of Beta */
        nextMove.nRow = gameBoard.beta[gamePiece.nRow].nRow;
        nextMove.nCol = gameBoard.beta[gamePiece.nCol].nRow;

        for(k = 0; k < MAX_PIECES; k++)
        {
            if(gameBoard.beta[k].eaten == true)
            {
                gameBoard.beta[k].nRow = 8;
                gameBoard.beta[k].nCol = 8;
            }
            if(gameBoard.alpha[k].eaten == true)
            {
                gameBoard.alpha[k].nRow = 8;
                gameBoard.alpha[k].nCol = 8;

            }
        }
    }
    return gameBoard;
}

bool validatePiece(coordinates gamePiece, bool aTurn, board gameBoard)
{
    // declare variables
    int i;
    bool valid = false;

        if (aTurn) 
        {
            // check if piece is in alpha array
            for (i = 0; i < MAX_PIECES; i++) {
                if (gameBoard.alpha[i].nRow == gamePiece.nRow && gameBoard.alpha[i].nCol == gamePiece.nCol) {
                    valid = true;
                    break;
                }
            }
        } 
        else 
        {
            // check if piece is in beta array
            for (i = 0; i < MAX_PIECES; i++) {
                if (gameBoard.beta[i].nRow == gamePiece.nRow && gameBoard.beta[i].nCol == gamePiece.nCol) {
                    valid = true;
                    break;
                }
            }
        }
    return valid;
}
//
bool validateNextMove(bool aTurn, coordinates gamePiece, coordinates nextMove, board gameBoard) // utilized by getNextMove()
{

    bool validNextMove = false;
    bool checkOccupied = false;

    // validNextMove = Validation 1
    validNextMove = checkMove(aTurn, gamePiece, nextMove, gameBoard);

    // validNextMove = Validation 2
    if (validNextMove == true)
    {
        checkOccupied = FreeOrOccupied( nextMove, gameBoard);
    }

    //Validation 3
    if(checkOccupied == true) //will run if occupied ang tiles, no matter if may piece alpha piece or beta piece na occupied
    {
        validNextMove = takeShaded(aTurn, nextMove, gameBoard);
    }

    return validNextMove;
}

// 1st Validation for Alpha and Beta Pieces (~~ ok âˆ§ aTurn âˆ§ next âˆˆ Free)
bool checkMove(bool aTurn, coordinates gamePiece, coordinates nextMove, board gameBoard)
{
    // declare variables
    int tokenIndex;
    bool valid = false;
    bool restricted = false;

    tokenIndex = getTokenIndex(aTurn, gamePiece, gameBoard);

    printf("%d \n", gameBoard.alpha[tokenIndex].nRow);
    printf("%d \n", gameBoard.alpha[tokenIndex].nCol);
    

        if (aTurn == true) {
            // check if piece moves forward or diagonally
            if (nextMove.nRow == gameBoard.alpha[tokenIndex].nRow - 1  && nextMove.nCol == gameBoard.alpha[tokenIndex].nCol
                || nextMove.nRow == gameBoard.alpha[tokenIndex].nRow - 1 && nextMove.nCol == gameBoard.alpha[tokenIndex].nCol + 1 
                || nextMove.nRow == gameBoard.alpha[tokenIndex].nRow - 1 && nextMove.nCol == gameBoard.alpha[tokenIndex].nCol - 1) 
            {
                valid = true;
            }
        } else {
            // check if piece moves downward or diaagonally
            if (nextMove.nRow == gameBoard.beta[tokenIndex].nRow + 1 && nextMove.nCol == gameBoard.beta[tokenIndex].nCol
                || nextMove.nRow == gameBoard.beta[tokenIndex].nRow + 1 && nextMove.nCol == gameBoard.beta[tokenIndex].nCol + 1 
                || nextMove.nRow == gameBoard.beta[tokenIndex].nRow + 1 && nextMove.nCol == gameBoard.beta[tokenIndex].nCol - 1) 
            {
                valid = true;
            }
        }

        // check if piece is still on the board
        if (nextMove.nRow < 0 || nextMove.nRow > 6 || nextMove.nCol < 0 || nextMove.nCol > 4) 
        {
            valid = false;
            restricted = true;
        }

        // if invalid, ask for valid move then rerun function
        if (valid == false) 
        {
            if (restricted == true) {
                gotoxy(36, 19); printf("\e[0;91m"); printf("OUT OF BOUNDS! PLEASE MAKE A NEW MOVE.");//CHANGEABLE
            } else {
                gotoxy(36, 20); printf("\e[0;91m"); printf("FORWARD AND DIAGONAL MOVEMENT ONLY!");//CHANGEABLE
                gotoxy(36, 21); printf("\e[0;91m"); printf("PLEASE MAKE A NEW MOVE.");//CHANGEABLE
            }
        }   
        
    return valid;
}

// Validation 2 || chesskers, position, and rows/cols are modified to dstru7.c variables, function name also changed.
//Note: di nagamit yung aTurn variable sa validation 2, we may have to remove it.
bool FreeOrOccupied( coordinates nextMove, board gameBoard) 
{
    // declare variables
    int i;
    // int countAlpha = 0;
    // int countBeta = 0; 
    bool occupied = false;


    for (i = 0; i < 5; i++)
    {
       if(gameBoard.alpha[i].nRow == nextMove.nRow)
        {
            if (gameBoard.alpha[i].nCol == nextMove.nCol)
            {
                occupied = true;
            }
        } else if (gameBoard.beta[i].nRow == nextMove.nRow)
        {
            if (gameBoard.beta[i].nCol == nextMove.nCol)
            {
                occupied = true;
            }
        }
        if (occupied == true) 
        {
            break;
        }
    }
    

    return occupied;
}

// Validation 3 || chesskers, position, and rows/cols are modified to dstru7.c variables, function name also changed.
// nextMove variable name subject to change basedon sa maiilagay
bool takeShaded(bool aTurn, coordinates nextMove, board gameBoard)
{
    // declare variables
    bool bTake = false; //changed from valid to bTake
    int i;

    // else if(gameBoard.shadedTile[i][j] == true){

    if (aTurn == true) // Alpha Turn
    { 
        // For Alpha's Turn,  check if enemy piece is in shaded tile
        for (i = 0; i < MAX_PIECES; i++) 
        {
            if (gameBoard.shadedTile[nextMove.nRow][nextMove.nCol] == true && gameBoard.beta[i].nRow == nextMove.nRow && gameBoard.beta[i].nCol == nextMove.nCol)
            {
                bTake = true;
            } 
        }
    } 
    else if (aTurn == false) // Beta Turn
    {
        // Beta's Turn, check if enemy piece is in shaded tile
        for (i = 0; i < MAX_PIECES; i++) 
            {
            if (gameBoard.shadedTile[nextMove.nRow][nextMove.nCol] == true && gameBoard.alpha[i].nRow == nextMove.nRow && gameBoard.alpha[i].nCol == nextMove.nCol) 
                {
                    bTake = true;
                }
            }
    }

    return bTake;
}

void boxMaker(int tbLeft, int tbRight, int yTopHor, int yBotHor, int iLHor, int iRHor, int iLVer, int iRVer)
{
    // tbLeft, tbRight, yTopHor, yBotHor, iLHor(tbleft+1), iRHor(tbRight-1), iLVer(yTopHor+1), iRVer(yBotHor-1)
    gotoxy(tbLeft, yTopHor); printf("%c", 201); // top-left 'â•”'
    gotoxy(tbRight, yTopHor); printf("%c", 187); // top-right 'â•—'
    gotoxy(tbLeft, yBotHor); printf("%c", 200); // bottom-left 'â•š'
    gotoxy(tbRight, yBotHor); printf("%c", 188); // bottom-right 'â•'

    for (int i = iLHor; i <= iRHor; i++) 
    { 
        gotoxy(i, yTopHor); printf("%c", 205); // top horizontals 'â•'
        gotoxy(i, yBotHor); printf("%c", 205); // bottom horizontals 'â•'
    }

    for (int i = iLVer; i <= iRVer; i++) 
    {
        gotoxy(tbLeft, i); printf("%c", 186); // left side vertical 'â•‘'
        gotoxy(tbRight, i); printf("%c", 186); // right side vertical 'â•‘'
    }
}

void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}

void nullBoard(board gameBoard)
{

    int i, j;
    int a, b;

    char col[7] = {'A','B','C','D','E','F','G'};
    int row[5] = {1, 2, 3, 4, 5};
    //printf("clearscreen at nullboard top\n"); sleep(1);
    system ("cls"); // clear screen

    // tbLeft, tbRight, yTopHor, yBotHor, iLHor(tbleft+1), iRHor(tbRight-1), iLVer(yTopHor+1), iRVer(yBotHor-1)
    printf("\e[4;36m"); boxMaker(3, 32, 2, 22, 4, 31, 3, 21); printf("\e[0m");

    gotoxy(8, 4); printf("\e[46m"); printf("COMMAND AND CONQUER!"); printf("\e[0m"); // title

    gotoxy(7, 6); printf("\e[4;0m"); printf("X"); printf("\e[0m"); // column legends
    gotoxy(11, 6); printf("\e[38;5;208m"); printf("1"); printf("\e[0m");
    gotoxy(15, 6); printf("\e[38;5;208m"); printf("2"); printf("\e[0m");
    gotoxy(19, 6); printf("\e[38;5;208m"); printf("3"); printf("\e[0m");
    gotoxy(23, 6); printf("\e[38;5;208m"); printf("4"); printf("\e[0m");
    gotoxy(27, 6); printf("\e[38;5;208m"); printf("5"); printf("\e[0m");


    printf("\e[4;33m"); // row legends
    gotoxy(7, 8); printf("%c", col[0]); // A   
    gotoxy(7, 10); printf("%c", col[1]); // B    
    gotoxy(7, 12); printf("%c", col[2]); // C 
    gotoxy(7, 14); printf("%c", col[3]); // D
    gotoxy(7, 16); printf("%c", col[4]); // E
    gotoxy(7, 18); printf("%c", col[5]); // F
    gotoxy(7, 20); printf("%c", col[6]); // G
    printf("\e[0m");

    for (i = 10; i < 30; i=i+4) // per row wc is 5
    {
        // printf("\033[48;5;7m \033[38;5;0m");
        printf("\e[4;36m");
        gotoxy(i, 8); printf("[ ]");
        gotoxy(i, 10); printf("[ ]");
        gotoxy(i, 12); printf("[ ]");
        gotoxy(i, 14); printf("[ ]");
        gotoxy(i, 16); printf("[ ]");
        gotoxy(i, 18); printf("[ ]");
        gotoxy(i, 20); printf("[ ]");
        printf("\e[0m");
    }

    for(j = 0; j < 5; j++) // for printing the shaded tiles
    {
        for (i = 0; i < 7; i++)
            if (gameBoard.shadedTile[i][j])
            {
                gotoxy(( 11 + (j) * 4) , (8+(i)*2)); 
                printf("\e[1;97m"); printf("%c", 240); printf("\e[0m"); //since we are starting at (0,0), we remove -1 in an=a1+(n-1)d
            }
    }
    
    for(j = 0; j < 5; j++) // for printing the beta pieces
    {
        if (gameBoard.beta[j].nCol > 5 || gameBoard.beta[j].nRow > 7)
        {
            printf("\0");
        }
        else // ARITHMETIC SEQUENCE TO PRINT THE POSITIONS OF BETA CORRESPONDING TO THE GOTOXY();
        { 
            gotoxy((11+(gameBoard.beta[j].nCol)*4) , (8+(gameBoard.beta[j].nRow)*2)); 
            printf("\e[38;5;124m"); printf("%c", BOX); printf("\e[0m");//since we are starting at (0,0), we remove -1 in an=a1+(n-1)d
        }
    }
    
    for(j = 0; j < 5; j++) // for printing the alpha pieces
    {
        if (gameBoard.alpha[j].nCol > 5 || gameBoard.alpha[j].nRow > 7)
        {
            printf("\0");
        }
        else // ARITHMETIC SEQUENCE TO PRINT THE POSITIONS OF ALPHA CORRESPONDING TO THE GOTOXY();
        {
            gotoxy( (11+(gameBoard.alpha[j].nCol)*4) , (8+(gameBoard.alpha[j].nRow)*2)); 
            printf("\e[38;5;70m"); printf("%c", BOX); printf("\e[0m");//since we are starting at (0,0), we remove -1 in an=a1+(n-1)d
        }
    }

    gotoxy(1, 26); printf(" ");
}

void welcomeMenu()
{
    system ("cls");

    // tbLeft, tbRight, yTopHor, yBotHor, iLHor(tbleft+1), iRHor(tbRight-1), iLVer(yTopHor+1), iRVer(yBotHor-1)
    printf("\e[4;36m"); boxMaker(3, 32, 2, 13, 4, 31, 3, 12); printf("\e[0m");

    gotoxy(8, 4); printf("\e[46m"); printf("COMMAND AND CONQUER!"); printf("\e[0m"); // title

    printf("\e[48;5;15m\e[38;5;0m"); gotoxy(12, 6); printf("DEVELOPERS:"); printf("\e[0m");
    printf("\e[48;5;7m\e[38;5;0m"); gotoxy(8, 8); printf("BACCAY, Dominic Luis");
    gotoxy(8, 9); printf("DIGNO, Kenneth Clark");
    gotoxy(8, 10); printf("MIRANDA, Bien Aaron ");
    gotoxy(8, 11); printf("RANA, Luis Miguel   "); printf("\e[0m");
    getch();
}

void homeMenu()
{
    system ("cls");
    // tbLeft, tbRight, yTopHor, yBotHor, iLHor(tbleft+1), iRHor(tbRight-1), iLVer(yTopHor+1), iRVer(yBotHor-1)
    printf("\e[4;36m"); boxMaker(3, 32, 2, 13, 4, 31, 3, 12); printf("\e[0m");
    gotoxy(8, 4); printf("\e[46m"); printf("COMMAND AND CONQUER!"); printf("\e[0m"); // title

    printf("\e[41m");
    gotoxy(6, 6); printf("Conquer The Battlefield!");
    gotoxy(6, 7); printf("  Become the Commander  ");
    gotoxy(6, 8); printf("and Eradicate the Enemy.");
    printf("\e[0m");

    printf("\e[42m");
    gotoxy(13, 10); printf("[S] START");
    gotoxy(13, 11); printf("[R] RULES");
    gotoxy(13, 12); printf("[X] EXIT ");
    printf("\e[0m");
}

//Game Over Function
bool gameOver(board gameBoard)
{
    bool over = false;
    // declare variables
    int i, j, k;
    int alphaEaten = 0; //for counting how many were considered eater
    int betaEaten = 0;
    
    //variables for the enemy base condition
    int alphaCount;
    int alphaHomerun = 0;
    int betaCount;
    int betaHomerun = 0;

    int remainingAlpha;
    int remainingBeta;

    // check if alpha has pieces left
    for (int i = 0; i < MAX_PIECES; i++) {
        if (gameBoard.alpha[i].eaten == true) { //to be changed based on how we will implement the kill board
            alphaEaten++;
        }
        if (gameBoard.beta[i].eaten == true) { //to be changed based on how we will implement the kill board
            betaEaten++;
        }
    }
    

    // check if players reached enemy territory
    for (i = 0; i < MAX_ROWS; i++) 
    {
        for (j = 0; j < MAX_COLS; j++) 
        {
            if (gameBoard.shadedTile[i][j] == true) 
            {       
                if (i < 2) 
                { //rows 2
                    for(alphaCount = 0; alphaCount < MAX_PIECES; alphaCount++){

                        if (gameBoard.alpha[alphaCount].nRow == i && gameBoard.alpha[alphaCount].nCol == j) 
                        {
                            alphaHomerun++;         
                            printf(" AlphaHomeRun: %d |   alphaCount: %d\n", alphaHomerun, alphaCount);
                        } 
                    }
                } else if (i > 4) 
                { //rows 4 and up
                    for(betaCount = 0; betaCount < MAX_PIECES; betaCount++){
                        if (gameBoard.beta[betaCount].nRow == i && gameBoard.beta[betaCount].nCol == j) 
                        {
                            betaHomerun++;
                            printf(" betaHomerun: %d |   betaCount: %d\n", betaHomerun, betaCount);
                        }
                    }
                }
            }
        }
    }

    //we need to know how much is remaining
    remainingAlpha = MAX_PIECES - alphaEaten;
    remainingBeta = MAX_PIECES - betaEaten;
    //Debugger
    printf("\n\n\n AlphaEaten: %d |  BetaEaten: %d\n", alphaEaten, betaEaten);
    printf("remainAlpha: %d | remainBeta: %d\n", remainingAlpha, remainingBeta);
    printf("AlpaHomeRun: %d | BetaHomeRun: %d\n", alphaHomerun, betaHomerun);
    printf(" AlphaCount: %d |   BetaCount: %d\n", alphaCount, betaCount);
    sleep(2);
    // check if game should be stopped
    if (alphaEaten == MAX_PIECES || betaEaten == MAX_PIECES) // over validation 1
    {
        over = true; // return
        if (alphaEaten == MAX_PIECES) {
            printf("CLS Here Over Validation 1\n");
            sleep(2);
            system("cls");
            congratulationsBeta();
            // printf("\nPlayer Alpha has lost all of their pieces.\n"); //changeable
            // printf("player Beta has won the game!\n"); //changeable
            // sleep(2);
        } else if (betaEaten == MAX_PIECES){
            printf("CLS Here Over Validation 1\n");
            sleep(2);
            system("cls");
            congratulationsAlpha();
            // printf("\nplayer Beta has lost all of their pieces.\n"); // changeable
            // printf("Player Alpha has won the game!\n"); //changeable
            // sleep(2);
        }
    } 
    else if (alphaHomerun == remainingAlpha || betaHomerun == remainingBeta) // over validation 2
    {
        over = true; // return
        if (alphaHomerun == remainingAlpha) {
            printf("CLS Here Over Validation 2\n");
            sleep(2);
            system("cls");
            congratulationsAlpha();
            sleep(1);
            // printf("\nPlayer Alpha's remaining pieces touched the enemy tile.\n"); // changeable
            // printf("Player Alpha has won the game!\n"); // changeable 
            // sleep(2);
            //congratulations for alpha
        } else if(betaHomerun == remainingBeta) {
            printf("CLS Here Over Validation 2\n");
            sleep(2);
            system("cls");
            congratulationsBeta();
            sleep(1);
            // printf("\nplayer Beta's remaining pieces touched the enemy tile.\n"); // changeable
            // printf("player Beta has won the game!\n"); // changeable
            // sleep(2);
            //congratulation for beta
        }
    }

    return over;
}

void rulesMenu()
{
    char prevnext;

    do
    {
        system("cls"); 
        // tbLeft, tbRight, yTopHor, yBotHor, iLHor(tbleft+1), iRHor(tbRight-1), iLVer(yTopHor+1), iRVer(yBotHor-1)
        printf("\e[4;36m"); boxMaker(3, 32, 2, 13, 4, 31, 3, 12); printf("\e[0m");
        gotoxy(7,4); printf("\e[42m"); printf("RULES AND REGULATIONS!"); printf("\e[0m"); 
        gotoxy(10,7); printf("\e[48;5;7m\e[38;5;0m"); printf("[S] SET-UP     ");
        gotoxy(10,8); printf("[H] HOW TO PLAY");
        gotoxy(10,9); printf("[W] WINNING    ");
        gotoxy(10,10); printf("[X] EXIT       "); printf("\e[0m"); 

        prevnext = getch();
        prevnext = toupper(prevnext);

        switch(prevnext)
        {
            case 'S':

                gotoxy(55,4); printf("\e[42m"); printf("SETUP:\n"); printf("\e[0m");
                gotoxy(35,6); printf("This game is played with 2 players, Alpha and Beta.\n");
                gotoxy(35,7); printf("Each player begins with 5 pieces that are placed on\n");
                gotoxy(35,8); printf("the shaded part of the gameboard. Their pieces are\n");
                gotoxy(35,9); printf("placed on the opposite sides of the board.\n"); 

                gotoxy(35,11); printf("The board consists of 35 tiles, alternating between \n");
                gotoxy(35,12); printf("18 black and 17 white tiles. Legends are put in place\n");
                gotoxy(35,13); printf("of which the player will utilize in moving the pieces.\n"); printf("\e[0m");
                getch();
                break;

            case 'H':

                gotoxy(52,4); printf("\e[42m"); printf("HOW TO PLAY:\n"); printf("\e[0m");
                gotoxy(35,6); printf("Player Alpha moves first then players move in a\n");
                gotoxy(35,7); printf("sequential manner. Players can move 1 tile forward\n");
                gotoxy(35,8); printf("or diagonally forward. A player can only eat on a\n");
                gotoxy(35,9); printf("shaded tile occupied by the enemy, invalid otherwise.\n"); printf("\e[0m");
                getch();
                break;

            case 'W':

                gotoxy(53,4);  printf("\e[42m"); printf("WINNING:\n"); printf("\e[0m");
                gotoxy(40,6); printf("A player can win the game in 2 ways:\n");
                gotoxy(36,8); printf("- When the player's pieces are all eaten,\n");
                gotoxy(36,9); printf("  the opposing player wins.\n");

                gotoxy(36,11); printf("- When all of the remaining player's pieces\n");
                gotoxy(36,12); printf("  reach the enemy's base tiles, the player wins.\n");
                getch();
                break;
            case 'X':
                // exit
                break;
            default:
                break;
        }

    } while(prevnext != 'X');

    

    

    // gotoxy(47,3); printf("RULES AND REGULATIONS\n");

    // gotoxy(55,5); printf("\e[44m"); printf("SETUP:\n"); printf("\e[0m");
    // gotoxy(33,7); printf("This game is played with 2 players, Alpha and Beta.\n");
    // gotoxy(33,8); printf("Each player begins with 5 pieces that are placed on\n");
    // gotoxy(33,9); printf("the shaded part of the gameboard. Their pieces are\n");
    // gotoxy(33,10); printf("placed on the opposite sides of the board.\n"); 

    // gotoxy(33,12); printf("The board consists of 35 tiles, alternating between \n");
    // gotoxy(33,13); printf("18 black and 17 white tiles. Position it so that each\n");
    // gotoxy(33,14); printf("player are placed on the longer side of the board.\n"); printf("\e[0m");

    // gotoxy(52,18); printf("\e[43m "); printf("HOW TO PLAY:\n"); printf("\e[0m");
    // gotoxy(33,20); printf("Player Alpha moves first then players move in a\n");
    // gotoxy(33,21); printf("sequential manner. Players can move 1 tile forward\n");
    // gotoxy(33,22); printf("or diagonally forward. A player can only eat on a\n");
    // gotoxy(33,23); printf("shaded tile occupied by the enemy, invalid otherwise.\n"); printf("\e[0m");

    // gotoxy(54,27);  printf("\e[41m"); printf("WINNING:\n"); printf("\e[0m");
    // gotoxy(40,29); printf("A player can win the game in 2 ways:\n");
    // gotoxy(36,30); printf("- When the player's pieces are all eaten,\n");
    // gotoxy(36,31); printf("  the opposing player wins.\n");

    // gotoxy(36,33); printf("- When all of the remaining player's pieces\n");
    // gotoxy(36,34); printf("  reach the enemy's base tiles, the player wins.\n");
    // gotoxy(43,37); printf("***PRESS ENTER TO CONTINUE***\n\n"); printf("\e[0m");
    // getch();
}

//Function for Printing the Congratulations for Beta
void congratulationsBeta()
{
    system("cls");
    gotoxy(32,4); printf("\e[0;32m");printf("                          ______              \n");
    gotoxy(32,5); printf("\e[0;32m");printf("  _________        .---ooo      ooo----.    \n");
    gotoxy(32,6); printf("\e[0;32m");printf(" :______.-':      :  .---------------.  :                       \n");
    gotoxy(32,7); printf("\e[0;32m");printf(" | ______  |      | :                 : |   \n");
    gotoxy(32,8); printf("\e[0;32m");printf(" |:______B:|      | |                 | |     \n");
    gotoxy(32,9); printf("\e[0;32m");printf(" |:______B:|      | |                 | |        \n");
    gotoxy(32,10); printf("\e[0;32m");printf(" |:______B:|      | |                 | |       \n");
    gotoxy(32,11); printf("\e[0;32m");printf(" |         |      | |                 | |     \n");
    gotoxy(32,12); printf("\e[0;32m");printf(" |:_____:  |      | |                 | |         \n");
    gotoxy(32,13); printf("\e[0;32m");printf(" |    ==   |      | :                 : |            \n");
    gotoxy(32,14); printf("\e[0;32m");printf(" |       O |      :  '---------------'  :    \n");
    gotoxy(32,15); printf("\e[0;32m");printf(" |       o |      :'---...______...----'      \n");
    gotoxy(32,16); printf("\e[0;32m");printf(" |       o |-._.-i___/'            \\__         \n");
    gotoxy(32,17); printf("\e[0;32m");printf(" |'-.____o_|   '-.   '-...______...-'  `-._      \n");
    gotoxy(32,18); printf("\e[0;32m");printf(" :_________:      `.____________________   `-.___.-. \n");
    gotoxy(32,19); printf("\e[0;32m");printf("                  .'.eeeeeeeeeeeeeeeeee.'.      :___:  \n");
    gotoxy(32,20); printf("\e[0;32m");printf("                .'.eeeeeeeeeeeeeeeeeeeeee.'.            \n");
    gotoxy(32,21); printf("\e[0;32m");printf("               :____________________________: \n");
    gotoxy(32,23); printf("\e[0;31m	");printf("            ****Press ANY KEY to Continue****    \n");

    gotoxy(54,10); printf("\e[0;31m");printf("Beta Player ");
    gotoxy(54,11); printf("Won! ");
    gotoxy(54,8); printf("CONGRATULATIONS"); printf("\e[0m");
    getch();
}

//Function for Printing the Congratulations for Alpha
void congratulationsAlpha()
{
    system("cls");
    gotoxy(32,4); printf("\e[0;32m");printf("                          ______              \n");
    gotoxy(32,5); printf("\e[0;32m");printf("  _________        .---ooo      ooo----.    \n");
    gotoxy(32,6); printf("\e[0;32m");printf(" :______.-':      :  .---------------.  :                       \n");
    gotoxy(32,7); printf("\e[0;32m");printf(" | ______  |      | :                 : |   \n");
    gotoxy(32,8); printf("\e[0;32m");printf(" |:______B:|      | |                 | |     \n");
    gotoxy(32,9); printf("\e[0;32m");printf(" |:______B:|      | |                 | |        \n");
    gotoxy(32,10); printf("\e[0;32m");printf(" |:______B:|      | |                 | |       \n");
    gotoxy(32,11); printf("\e[0;32m");printf(" |         |      | |                 | |     \n");
    gotoxy(32,12); printf("\e[0;32m");printf(" |:_____:  |      | |                 | |         \n");
    gotoxy(32,13); printf("\e[0;32m");printf(" |    ==   |      | :                 : |            \n");
    gotoxy(32,14); printf("\e[0;32m");printf(" |       O |      :  '---------------'  :    \n");
    gotoxy(32,15); printf("\e[0;32m");printf(" |       o |      :'---...______...----'      \n");
    gotoxy(32,16); printf("\e[0;32m");printf(" |       o |-._.-i___/'            \\___         \n");
    gotoxy(32,17); printf("\e[0;32m");printf(" |'-.____o_|   '-.   '-...______...-'  `-._      \n");
    gotoxy(32,18); printf("\e[0;32m");printf(" :_________:      `.____________________   `-.___.-. \n");
    gotoxy(32,19); printf("\e[0;32m");printf("                  .'.eeeeeeeeeeeeeeeeee.'.      :___:  \n");
    gotoxy(32,20); printf("\e[0;32m");printf("                .'.eeeeeeeeeeeeeeeeeeeeee.'.            \n");
    gotoxy(32,21); printf("\e[0;32m");printf("               :____________________________: \n");
    gotoxy(32,23); printf("\e[0;31m	");printf("             ****Press ANY KEY to Continue****   \n");

    gotoxy(54,10); printf("\e[0;32m");printf("Alpha Player ");
    gotoxy(54,11); printf("Won! ");
    gotoxy(54,8); printf("CONGRATULATIONS"); printf("\e[0m");
    getch();
}       