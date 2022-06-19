/*  CCDSTRU Machine Project: Command and Conquer!
    Prepared and Programmed by Group 7
    Members:    Baccay, Dominic Luis    ID Number:  12108173
                Digno, Kenneth Clark                12172677
                Miranda, Bien Aaron                 12106773
                Rana, Luis Miguel                   12179124
*/

#include <stdio.h>
#include <strings.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

//Numerical Constants
#define MAX_ROWS 7
#define MAX_COLS 5
#define MAX_PIECES 5
#define BOX 254 //ASCII Value of Box Character

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
    coordinates alpha[MAX_PIECES]; 
    coordinates beta[MAX_PIECES];
} board;

//Game Design Functions
void gotoxy(int x, int y);
void boxMaker(int tbLeft, int tbRight, int yTopHor, int yBotHor, int iLHor, int iRHor, int iLVer, int iRVer);
void homeMenu();
void welcomeMenu();
void rulesMenu();

//Logical Functions
board intilializeGameBoard(board freshGameBoard);
void nullBoard(board gameBoard);
coordinates getPlayerPiece(bool aTurn, coordinates gamePiece);
coordinates getNextMove(bool aTurn, coordinates nextMove);
int getTokenIndex(bool aTurn, coordinates gamePiece, board gameBoard);
board movePiece(bool aTurn, coordinates gamePiece, coordinates nextMove, board gameBoard);
bool validatePiece(coordinates gamePiece, bool aTurn, board gameBoard); // utilized by getPlayerPiece()
bool validateNextMove(bool aTurn, coordinates gamePiece, coordinates nextMove, board gameBoard); // utilized by getNextMove()

// Game Validations 
bool checkMove(bool aTurn, coordinates gamePiece, coordinates nextMove, board gameBoard); // 1st Validation for Moving Forward or Diagonal but not backward or forward
bool FreeOrOccupied( coordinates nextMove, board gameBoard); // 2nd Validation 
bool takeShaded(bool aTurn, coordinates nextMove, board gameBoard); // 3rd Validation for Eating Chess Pieces
bool gameOver(board gameBoard);

//End-Game Designs
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

    //Do-while loop initializations
    bool validPiece = false;
    bool validNextPiece = false;

    board gameBoard;
    coordinates gamePiece;
    coordinates nextMove;

    /* Greet Player and the Game Itself */
    welcomeMenu();

    //Initilize a fresh gameboard with initial positions
    gameBoard = intilializeGameBoard(gameBoard);

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
                        nextMove = getNextMove(aTurn, nextMove);
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
    } while(menu != 'X');

    return 0;
}

/*  initializeGameBoard() : Function that initializes the game board as a new gameboard 
    @param freshGameBoard - takes the gameboard struct variable as input and returns a fresh gameboard with varibale intialization.
*/
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
    //Return Fresh Game Board
    return freshGameBoard;
}

/*  getPlayerPiece() : Function that gets (x,y) coordinates of the playerPiece that the user selected 
    @param aTurn - indicates the player turn of each player whether it is Alpha's Turn or Beta's Turn
    @param gamePiece - takes the coordinate struct variable and returns the (x,y) values that will be stored in gamePiece.
*/
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
    
    //Return (x,y) values of that piece
    return gamePiece;
}

/*  getNextMove() : Function that gets (x,y) coordinates of the tile that the user wishes to move his/her Piece. 
    @param aTurn - indicates the player turn of each player whether it is Alpha's Turn or Beta's Turn
    @param nextMove - takes the coordinate struct variable and returns the (x,y) values of the tile that will be stored in nextMove.
*/
coordinates getNextMove(bool aTurn, coordinates nextMove)
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

    //Return (x,y) values of tile where the user wants to go
    return nextMove;
}

/*  getTokenIndex() : Function that returns the index of what piece is selected from the array of Player Pieces
    @param aTurn - indicates the player turn of each player whether it is Alpha's Turn or Beta's Turn
    @param gamePiece - takes the coordinate struct variable and returns the (x,y) values that will be stored in gamePiece.
*/
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
    
    /* Return the index of the selected piece */
    return tokenIndex;
}

/*  movePiece() : Function that changes the coordinates of gamePiece to the coordinates of nextMove and returns the updated gameboard after function call
    @param aTurn - indicates the player turn of each player whether it is Alpha's Turn or Beta's Turn
    @param gamePiece - the coordinate struct variable that contains the (x,y) values of the gamePiece.
    @param nextMove - the coordinate struct variable that contains the (x,y) values of the tile that will gamePiece will obtain.
    @param gameBoard - the gameboard struct that contains all necessary variables used for game validations and follow the game behavior
    Note: Contains multiple functions for movement validations and for anti-bug measures.
*/
board movePiece(bool aTurn, coordinates gamePiece, coordinates nextMove, board gameBoard){

    bool isOccupied = false, canAttack = false; 
    bool validNextMove = false;
    bool checkOccupied = false;
    int i, j, k;

    /*Integer Value for getting the index of the selected piece */
    int tokenIndex = getTokenIndex(aTurn, gamePiece, gameBoard);
    
    if(aTurn){ /* If aTurn, then it is Alpha's Turn to Play */
        
        /* validNextMove = Validation 1 */
        validNextMove = checkMove(aTurn, gamePiece, nextMove, gameBoard);

        /* validNextMove = Validation 2 */
        if(checkOccupied == true)
        {
            checkOccupied = FreeOrOccupied( nextMove, gameBoard);
        }

        /* Validation 3 Function */
        if(checkOccupied == true) //will run if occupied ang tiles, no matter if may piece alpha piece or beta piece na occupied
        {
            validNextMove = takeShaded(aTurn, nextMove, gameBoard);
        }

        /* Move to the COORDINATES fetched from nextMove() of the index of chosen Piece */
        gameBoard.alpha[tokenIndex].nRow = nextMove.nRow;
        gameBoard.alpha[tokenIndex].nCol = nextMove.nCol;

        /* If we say this is true, nasa shaded tile ung kakain, tas ung nakain, set eaten to true */
        if(validNextMove == true){
            
            for(i = 0; i < MAX_PIECES; i++){
                if(gameBoard.beta[i].nRow == nextMove.nRow && gameBoard.beta[i].nCol == nextMove.nCol){
                    gameBoard.beta[i].eaten = true;
                }
            }
        }

        /* Declare the previous COORDINATES to the COORDINATES of Alpha */
        nextMove.nRow = gameBoard.alpha[gamePiece.nRow].nRow;
        nextMove.nCol = gameBoard.alpha[gamePiece.nCol].nRow;
    }
    else if(aTurn == false){ /* If !aTurn, then it is Beta's Turn to Play */
        
        /* validNextMove = Validation 1 */
        validNextMove = checkMove(aTurn, gamePiece, nextMove, gameBoard);

        /* validNextMove = Validation 2 */
        checkOccupied = FreeOrOccupied( nextMove, gameBoard);

        /* Validation 3 Function */
        if(checkOccupied == true) //will run if occupied ang tiles, no matter if may piece alpha piece or beta piece na occupied
        {
            validNextMove = takeShaded(aTurn, nextMove, gameBoard);
        }

        /* Move to the COORDINATES fetched from nextMove() of the index of chosen Piece */
        gameBoard.beta[tokenIndex].nRow = nextMove.nRow;
        gameBoard.beta[tokenIndex].nCol = nextMove.nCol;

        /* If we say this is true, nasa shaded tile ung kakain, tas ung nakain, set eaten to true */
        if(validNextMove == true){
            for(i = 0; i < MAX_PIECES; i++)
            {
                if(gameBoard.alpha[i].nRow == nextMove.nRow && gameBoard.alpha[i].nCol == nextMove.nCol){
                    gameBoard.alpha[i].eaten = true;
                }
            }
        }

        /* Declare the previous COORDINATES to the COORDINATES of Beta */
        nextMove.nRow = gameBoard.beta[gamePiece.nRow].nRow;
        nextMove.nCol = gameBoard.beta[gamePiece.nCol].nRow;

    }

    /* Check if there are eaten Pieces, if there is, set the (x,y) values of the eaten piece to number outside of the domain of X and Y */
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

    return gameBoard;
}

/*  validatePiece() : Function that check if the selected piece of the user is valid for movemement, returns true if the selected piece is valid
    @param aTurn - indicates the player turn of each player whether it is Alpha's Turn or Beta's Turn
    @param gamePiece - the coordinate struct variable that contains the (x,y) values of the selected Piece.
    @param gameBoard - the gameboard struct that contains all necessary variables used for game validations and follow the game behavior
*/
bool validatePiece(coordinates gamePiece, bool aTurn, board gameBoard)
{
    /* Variables */
    int i;
    bool valid = false;

    if (aTurn) 
    {
        /* Check if Selected Piece is still active in the array of Alpha Pieces */
        for (i = 0; i < MAX_PIECES; i++) {
            if (gameBoard.alpha[i].nRow == gamePiece.nRow && gameBoard.alpha[i].nCol == gamePiece.nCol) {
                valid = true;
                break;
            }
        }
    } 
    else 
    {
        /* Check if Selected Piece is still active in the array of Beta Pieces */
        for (i = 0; i < MAX_PIECES; i++) {
            if (gameBoard.beta[i].nRow == gamePiece.nRow && gameBoard.beta[i].nCol == gamePiece.nCol) {
                valid = true;
                break;
            }
        }
    }

    /* Return Boolean Value */
    return valid;
}

/*  validateNextMove() : Boolean Function that returns true if all previous validations are true. To be used in the main function so that while-loop gets validation
    @param aTurn - indicates the player turn of each player whether it is Alpha's Turn or Beta's Turn
    @param gamePiece - contains the coordinate struct variable of the chosen Piece (Used for Validation)
    @param nextMove - contains the coordinate struct variable  of the tile that will be stored in nextMove. (Used for Validation) 
    @param gameBoard - passes the gameBoard struct from int main()
    Pre-Condition: gamePiece and nextMove contains a valid coordiante 
*/
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

/*  checkMove() : 1st Validation of NextMove. Returns true if the movement and selected coordinates of the piece and tile are valid
    @param aTurn - indicates the player turn of each player whether it is Alpha's Turn or Beta's Turn
    @param gamePiece - contains the coordinate struct variable of the chosen Piece (Used for Validation)
    @param nextMove - contains the coordinate struct variable  of the tile that will be stored in nextMove. (Used for Validation) 
    @param gameBoard - passes the gameBoard struct from validateNextMove->nextMove->Main
*/
bool checkMove(bool aTurn, coordinates gamePiece, coordinates nextMove, board gameBoard)
{
    /* Declare variables */
    int tokenIndex;
    bool valid = false;
    bool restricted = false;

    tokenIndex = getTokenIndex(aTurn, gamePiece, gameBoard);

    printf("%d \n", gameBoard.alpha[tokenIndex].nRow);
    printf("%d \n", gameBoard.alpha[tokenIndex].nCol);
    

        if (aTurn == true) {
            /* Check if selected alphaPiece moves forward or diagonally by exactly one tile */
            if (nextMove.nRow == gameBoard.alpha[tokenIndex].nRow - 1  && nextMove.nCol == gameBoard.alpha[tokenIndex].nCol
                || nextMove.nRow == gameBoard.alpha[tokenIndex].nRow - 1 && nextMove.nCol == gameBoard.alpha[tokenIndex].nCol + 1 
                || nextMove.nRow == gameBoard.alpha[tokenIndex].nRow - 1 && nextMove.nCol == gameBoard.alpha[tokenIndex].nCol - 1) 
            {
                valid = true;
            }
        } else {
            /* Check if selected betaPiece moves forward or diagonally by exactly one tile */
            if (nextMove.nRow == gameBoard.beta[tokenIndex].nRow + 1 && nextMove.nCol == gameBoard.beta[tokenIndex].nCol
                || nextMove.nRow == gameBoard.beta[tokenIndex].nRow + 1 && nextMove.nCol == gameBoard.beta[tokenIndex].nCol + 1 
                || nextMove.nRow == gameBoard.beta[tokenIndex].nRow + 1 && nextMove.nCol == gameBoard.beta[tokenIndex].nCol - 1) 
            {
                valid = true;
            }
        }

        /* Check if selected piece is still active on the board (since eaten will have a (x,y) values that is out-of-bounds) */
        if (nextMove.nRow < 0 || nextMove.nRow > 6 || nextMove.nCol < 0 || nextMove.nCol > 4) 
        {
            valid = false;
            restricted = true;
        }

        /* If validation fails, reprompts the user again since user-input is invalid */
        if (valid == false) 
        {
            if (restricted == true) {
                gotoxy(36, 19); printf("\e[0;91m"); printf("OUT OF BOUNDS! PLEASE MAKE A NEW MOVE."); //CHANGEABLE
            } else {
                gotoxy(36, 20); printf("\e[0;91m"); printf("FORWARD AND DIAGONAL MOVEMENT ONLY!"); //CHANGEABLE
                gotoxy(36, 21); printf("\e[0;91m"); printf("PLEASE MAKE A NEW MOVE."); //CHANGEABLE
            }
        }   
    
    /* Returns True if Validation 1 succeeds */
    return valid;
}

/*  FreeOrOccupied() : 2nd Validation of NextMove. Returns true if the selected tile of the next move is occupied by a Piece
    @param nextMove - contains the coordinate struct variable  of the tile that will be stored in nextMove. (Used for Validation) 
    @param gameBoard - passes the gameBoard struct from validateNextMove->nextMove->Main
*/
bool FreeOrOccupied( coordinates nextMove, board gameBoard) 
{
    /* Declare variables */
    int i;
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
    
    /* Returns true if future coordinate of selected piece is occupied else return false */
    return occupied;
}

/*  takeShaded() : 3rd and Last Validation of NextMove. Returns true if occupied tile is shaded and if tile is occupied by an enemy
    @param aTurn - indicates the player turn of each player whether it is Alpha's Turn or Beta's Turn
    @param nextMove - contains the coordinate struct variable  of the tile that will be stored in nextMove. (Used for Validation) 
    @param gameBoard - passes the gameBoard struct from validateNextMove->nextMove->Main
*/
bool takeShaded(bool aTurn, coordinates nextMove, board gameBoard)
{
    // declare variables
    bool eatEnemy = false; 
    int i;

    if (aTurn == true) // Alpha Turn
    { 
        // For Alpha's Turn,  check if enemy piece is in shaded tile
        for (i = 0; i < MAX_PIECES; i++) 
        {
            if (gameBoard.shadedTile[nextMove.nRow][nextMove.nCol] == true && gameBoard.beta[i].nRow == nextMove.nRow && gameBoard.beta[i].nCol == nextMove.nCol)
            {
                eatEnemy = true;
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
                    eatEnemy = true;
                }
            }
    }
    
    /* Returns true if attacking of piece is valid else return false */
    return eatEnemy;
}

/*  gameOver() : Function that returns true if the state of the game has reached its conclusion 
    @param gameBoard - Takes all the required values from its struct variable for validation purposes
*/
bool gameOver(board gameBoard)
{   
    /* Variables */
    bool over = false;
    int i, j, k;
    /* Counter for Eaten Pieces */
    int alphaEaten = 0; 
    int betaEaten = 0;
    /* Variables for Teritory Conditions and Teritory Piece Counter */
    int alphaCount;
    int alphaHomerun = 0;
    int betaCount;
    int betaHomerun = 0;
    /* Counter for Remaning Pieces or Alive Pieces */
    int remainingAlpha;
    int remainingBeta;

    /* Check if there are eaten pieces after nextMove() */
    for (int i = 0; i < MAX_PIECES; i++) {
        /* Check if each Alpha Piece in the array of Alpha Pieces if there are eaten pieces */
        if (gameBoard.alpha[i].eaten == true) { 
            alphaEaten++;
        }
        /* Check if each Beta Piece in the array of Beta Pieces if there are eaten Pieces */
        if (gameBoard.beta[i].eaten == true) {
            betaEaten++;
        }
    }

    /* Check if there are player pieces that have reached the opposing player's teritory */
    for (i = 0; i < MAX_ROWS; i++) 
    {
        for (j = 0; j < MAX_COLS; j++) 
        {
            /* Only Check for Occupied Shaded Tiles in each player's Teritory (White Tiles do not count as Teritory) */
            if (gameBoard.shadedTile[i][j] == true)
            {   
                /* Check if Alpha Piece reached any shaded tile in Rows A and B (Beta Teritory) */
                if (i < 2) 
                { /* nRow[0] and nRow[1] */
                    /* Loop through each Alpha Piece if it reached Beta teritory */
                    for(alphaCount = 0; alphaCount < MAX_PIECES; alphaCount++)
                    {
                        if (gameBoard.alpha[alphaCount].nRow == i && gameBoard.alpha[alphaCount].nCol == j) 
                        {
                            alphaHomerun++;         
                            printf(" AlphaHomeRun: %d |   alphaCount: %d\n", alphaHomerun, alphaCount); //delete after debugging
                        } 
                    }
                }
                /* Check if Beta Piece reached any shaded tile in Rows F and G (Alpha Teritory) */
                else if (i > 4) 
                { /* nRow[5] and nRow[6] */
                    /* Loop through each Beta Piece if it reached Alpha teritory */
                    for(betaCount = 0; betaCount < MAX_PIECES; betaCount++)
                    {
                        if (gameBoard.beta[betaCount].nRow == i && gameBoard.beta[betaCount].nCol == j) 
                        {
                            betaHomerun++;
                            printf(" betaHomerun: %d |   betaCount: %d\n", betaHomerun, betaCount); //delete after debugging
                        }
                    }
                }

            }

        }

    }

    /*Subtract the number of eaten pieces to MAX_PIECES to get remaining alive pieces */
    remainingAlpha = MAX_PIECES - alphaEaten;
    remainingBeta = MAX_PIECES - betaEaten;

    //Debugger
    printf("\n\n\n AlphaEaten: %d |  BetaEaten: %d\n", alphaEaten, betaEaten);
    printf("remainAlpha: %d | remainBeta: %d\n", remainingAlpha, remainingBeta);
    printf("AlpaHomeRun: %d | BetaHomeRun: %d\n", alphaHomerun, betaHomerun);
    printf(" AlphaCount: %d |   BetaCount: %d\n", alphaCount, betaCount);
    sleep(2);
    // Delete Later

    /* Check if we should stop the game since a winner has been decided */

    /* First Option: Win the Game by eating all enemy Pieces */
    if (alphaEaten == MAX_PIECES || betaEaten == MAX_PIECES) 
    {                                                        
        over = true; //Set Return Value
        if (alphaEaten == MAX_PIECES) {
            printf("CLS Here Over Validation 1\n");
            sleep(2);
            system("cls");
            congratulationsBeta();
        } else if (betaEaten == MAX_PIECES){
            printf("CLS Here Over Validation 1\n");
            sleep(2);
            system("cls");
            congratulationsAlpha();
        }
    } 
    /* Second Option: Win the Game by invading and occupying enemy teritory depending on the number of alive pieces */
    else if (alphaHomerun == remainingAlpha || betaHomerun == remainingBeta) 
    {                                                                        
        over = true; //Set Return Value                                              
        if (alphaHomerun == remainingAlpha) {
            printf("CLS Here Over Validation 2\n");
            sleep(2);
            system("cls");
            congratulationsAlpha();
            sleep(1);
        } else if(betaHomerun == remainingBeta) {
            printf("CLS Here Over Validation 2\n");
            sleep(2);
            system("cls");
            congratulationsBeta();
            sleep(1);
        }
    }

    /* Returns true if a winner has been decided */
    return over;
}

/*  boxMaker() : Creates a 2d Box 
    
*/
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

/*  gotoxy() : moves the printing at the start of the (x,y) coordinate
    @param x - x-coordinate or horizontal axis
    @param y - y-coordinate or vertical axis
    Note: This function is to be used for front-end development. 
*/
void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}

/*  nullBoard() : Declares and prints out the board. Prints the whole board depending on the gotoxy(x,y) values 
    @param gameBaord - gets all necessary values from the struct gameBoard for proper declaration of the (visual) game board
    Note: This function is to be used for front-end development. 
*/
void nullBoard(board gameBoard)
{

    int i, j;
    int a, b;

    char col[7] = {'A','B','C','D','E','F','G'};
    int row[5] = {1, 2, 3, 4, 5};
    system ("cls"); // clear screen

    // tbLeft, tbRight, yTopHor, yBotHor, iLHor(tbleft+1), iRHor(tbRight-1), iLVer(yTopHor+1), iRVer(yBotHor-1)
    printf("\e[4;36m"); boxMaker(3, 32, 2, 22, 4, 31, 3, 21); printf("\e[0m");
    gotoxy(8, 4); printf("\e[46m"); printf("COMMAND AND CONQUER!"); printf("\e[0m"); // title

    /* Column Legend Section */
    gotoxy(7, 6); printf("\e[4;0m"); printf("C"); printf("\e[0m"); 
    gotoxy(11, 6); printf("\e[38;5;208m"); printf("1"); printf("\e[0m");
    gotoxy(15, 6); printf("\e[38;5;208m"); printf("2"); printf("\e[0m");
    gotoxy(19, 6); printf("\e[38;5;208m"); printf("3"); printf("\e[0m");
    gotoxy(23, 6); printf("\e[38;5;208m"); printf("4"); printf("\e[0m");
    gotoxy(27, 6); printf("\e[38;5;208m"); printf("5"); printf("\e[0m");

    /* Row Legend Section */
    printf("\e[4;33m"); 
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

    /* Print Shaded Tiles Section */
    for(j = 0; j < 5; j++) 
    {
        for (i = 0; i < 7; i++)
            if (gameBoard.shadedTile[i][j])
            {
                gotoxy(( 11 + (j) * 4) , (8+(i)*2)); 
                printf("\e[1;97m"); printf("%c", 240); printf("\e[0m"); //since we are starting at (0,0), we remove -1 in an=a1+(n-1)d
            }
    }
    
    /* Print Beta Pieces Section */
    for(j = 0; j < 5; j++)
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
    
    /* Print Alpha Pieces Section */
    for(j = 0; j < 5; j++) 
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

/* Function for printing the Welcome Menu */
void welcomeMenu()
{
    system ("cls");
    // tbLeft, tbRight, yTopHor, yBotHor, iLHor(tbleft+1), iRHor(tbRight-1), iLVer(yTopHor+1), iRVer(yBotHor-1)
    printf("\e[4;36m"); boxMaker(3, 32, 2, 13, 4, 31, 3, 12); printf("\e[0m");

    gotoxy(8, 4); printf("\e[46m"); printf("COMMAND AND CONQUER!"); printf("\e[0m"); // title

    printf("\e[48;5;7m\e[38;5;0m"); gotoxy(12, 6); printf("DEVELOPERS:"); printf("\e[0m");
    printf("\e[48;5;7m\e[38;5;0m"); gotoxy(8, 8); printf("BACCAY, Dominic Luis");
    gotoxy(8, 9); printf("DIGNO, Kenneth Clark");
    gotoxy(8, 10); printf("MIRANDA, Bien Aaron ");
    gotoxy(8, 11); printf("RANA, Luis Miguel   "); printf("\e[0m");
    getch();
}

/* Function for printing the Home Menu */
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


//Function for Printing the Rules and Regulations Menu
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

}

//Function for Printing the Congratulations for Beta
void congratulationsBeta()
{
    system("cls");
    gotoxy(32,4); printf("\e[0;32m"); printf("                          ______              \n");
    gotoxy(32,5); printf("\e[0;32m"); printf("  _________        .---ooo      ooo----.    \n");
    gotoxy(32,6); printf("\e[0;32m"); printf(" :______.-':      :  .---------------.  :                       \n");
    gotoxy(32,7); printf("\e[0;32m"); printf(" | ______  |      | :                 : |   \n");
    gotoxy(32,8); printf("\e[0;32m"); printf(" |:______B:|      | |                 | |     \n");
    gotoxy(32,9); printf("\e[0;32m"); printf(" |:______B:|      | |                 | |        \n");
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
    gotoxy(32,4); printf("\e[0;32m"); printf("                          ______             \n");
    gotoxy(32,5); printf("\e[0;32m"); printf("  _________        .---ooo      ooo----.    \n");
    gotoxy(32,6); printf("\e[0;32m"); printf(" :______.-':      :  .---------------.  :                       \n");
    gotoxy(32,7); printf("\e[0;32m"); printf(" | ______  |      | :                 : |   \n");
    gotoxy(32,8); printf("\e[0;32m"); printf(" |:______B:|      | |                 | |     \n");
    gotoxy(32,9); printf("\e[0;32m"); printf(" |:______B:|      | |                 | |        \n");
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