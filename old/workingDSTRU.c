#include <stdio.h>
#include <strings.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

// REMOVE GAMEPIECE PARAMETER ON FUNCTION FREEOROCCUPIED

//Numerical Constants
#define MAX_ROWS 7
#define MAX_COLS 5
#define MAX_PIECES 5

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
    int alphaAlive;
    int betaAlive;
} board;

// new functions
void gotoxy(int x, int y);
void boxMaker(int tbLeft, int tbRight, int yTopHor, int yBotHor, int iLHor, int iRHor, int iLVer, int iRVer);
void nullBoard(board gameBoard);

board intilializeGameBoard(board freshGameBoard);
void printGameBoard(board gameBoard);

coordinates getPlayerPiece(bool aTurn, coordinates gamePiece);
coordinates getNextMove(bool aTurn, coordinates gamePiece, coordinates nextMove);
int getTokenIndex(bool aTurn, coordinates gamePiece, board gameBoard);
board movePiece(bool aTurn, coordinates gamePiece, coordinates nextMove, board gameBoard);
// board sortPieces(board gameBoard);
bool validatePiece(coordinates gamePiece, bool aTurn, board gameBoard); // utilized by getPlayerPiece()
bool validateNextMove(bool aTurn, coordinates gamePiece, coordinates nextMove, board gameBoard); // utilized by getNextMove()

// Validation 
bool checkMove(bool aTurn, coordinates gamePiece, coordinates nextMove, board gameBoard); // 1st Validation for Moving Forward or Diagonal but not backward or forward
bool FreeOrOccupied(bool aTurn, coordinates gamePiece, coordinates nextMove, board gameBoard); // 2nd Validation 
bool takeShaded(bool aTurn, coordinates nextMove, board gameBoard); // 3rd Validation for Eating Chess Pieces

int main(void){
    int l, k;
    int test = 0;
    
    //System Initilizations
    bool aTurn = true;
    bool over = false;

    // do-while loop initializations
    bool validPiece = false;
    bool validNextPiece = false;

    board gameBoard;

    gameBoard.alphaAlive = 5;
    gameBoard.betaAlive = 5;

    coordinates gamePiece;
    coordinates nextMove;

    printf("\n\t\t  -Battleships-\n\n");

    //Initilize a fresh gameboard with initial positions
    gameBoard = intilializeGameBoard(gameBoard);

    do
    {
        system ("cls"); // clear screen

        nullBoard(gameBoard);

        //Get Input from User
        if (aTurn) {
            printf("\n!!! Player Alpha's turn. !!!\n");
        } else {
            printf("\n!!! Player Beta's turn. !!!\n");
        }

        // This is for checking if we need to get the piece of the player
        do
        { 
            gamePiece = getPlayerPiece(aTurn, gamePiece);
            validPiece = validatePiece(gamePiece, aTurn, gameBoard);
            if (validPiece == false) {
                printf("\n||Invalid selection. Please select a piece again.||\n");
            }
            //validate game piece
        } while (validPiece == false);

        do 
        {
            nextMove = getNextMove(aTurn, gamePiece, nextMove);
            validNextPiece = validateNextMove(aTurn, gamePiece, nextMove, gameBoard);
            if (validNextPiece == false) {
                printf("\n||Invalid move. Please select a piece again.||\n");  
            }
        } while(validNextPiece == false); 
        
        gameBoard = movePiece(aTurn, gamePiece, nextMove, gameBoard);
        aTurn = !aTurn;
        test++;
        printf("\n");

        // over = gameOver(over, gameBoard);

        //nextMove = getNextMove(aTurn, gamePiece, nextMove);
        
        // // gameBoard = sortPieces(gameBoard);
        // for(k = 0; k < MAX_PIECES; k++){
        //     printf("Alpha %d:%d\n", k+1, gameBoard.alpha[k].eaten);
        // }
        // for(k = 0; k < MAX_PIECES; k++){
        //     printf("Beta %d:%d\n", k+1, gameBoard.beta[k].eaten);
        // }
        
        // //printGameBoard(gameBoard);
        // ///* For Debugging Purposes
        //     printf("Player Piece: (%d,%d)\n", gamePiece.nRow, gamePiece.nCol);
        //     printf("Piece Destination: (%d,%d)\n", nextMove.nRow, nextMove.nCol);
        // //*/
        //For Debugging Purposes
        printf("Beta Pieces:\n");
        for(l = 0; l < 5; l++){
            printf("Coordinate: (%d,%d)\n", gameBoard.beta[l].nRow, gameBoard.beta[l].nCol);
        }
        //For Debugging Purposes
        printf("Alpha Pieces:\n");
        for(k = 0; k < 5; k++){
            printf("Coordinate: (%d,%d) :)\n", gameBoard.alpha[k].nRow, gameBoard.alpha[k].nCol);
        }
        getch();
        
    } while(test != 100);
    // while(over == true)  

    // congratulation function
}

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

void printGameBoard(board gameBoard){

    int i, j, k, l;
    int alphaToken = 0, betaToken = 0;
    char cRowNumber[MAX_ROWS] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
    char cColNumber[MAX_COLS] = {'1', '2', '3', '4', '5'};

    /* ASCI Value for Special Symbols */
    int alpha = 254, beta = 254, shaded = 240;
    char shadedTile[MAX_PIECES] = "[ ]";
    char whiteTile[MAX_PIECES] = "[ ]";

    /*Print Column Number */
    printf("\t");
    for(k = 1; k <=MAX_COLS; k++){
        printf("     %c", cColNumber[k-1]);
    }
    printf("\n\n");
    
    /* Validation for Eaten Pieces, if found, ignore that piece */

    for(i = 0; i < MAX_ROWS; i++){

        /* Print nRow Letter */
        printf("\t%c", cRowNumber[i]);
        
        for (j = 0; j < MAX_COLS; j++){
            printf("   ");

            /*Initilize Alpha, Beta and Shaded on Shaded Parts on the Board */

                /* Initilize Beta (Top Part of the Board) */
                if (gameBoard.beta[betaToken].nRow == i && gameBoard.beta[betaToken].nCol == j){
                    if(gameBoard.beta[betaToken].eaten == false){
                        printf("\033[30;47m"); printf("[");  printf("\033[0;37m");
                        printf("\033[31;47m"); printf("%c", beta); 
                        printf("\033[30;47m"); printf("]"); printf("\033[0;37m");
                        betaToken++;
                    }
                }
                /* Initilize Alpha (Bottom Part of the Board) */
                else if (gameBoard.alpha[alphaToken].nRow == i && gameBoard.alpha[alphaToken].nCol == j){
                    if(gameBoard.alpha[alphaToken].eaten == false){
                        printf("\033[30;47m"); printf("[");  printf("\033[0;37m");
                        printf("\033[34;47m"); printf("%c", alpha); 
                        printf("\033[30;47m"); printf("]"); printf("\033[0;37m");
                        alphaToken++;
                    }
                }
                /* Initilize All Shaded Tiles (Across the Board) */
                else if(gameBoard.shadedTile[i][j] == true){
                    printf("\033[30;47m"); printf("[");  printf("\033[0;37m");
                    printf("\033[30;47m"); printf("%c", shaded); 
                    printf("\033[30;47m"); printf("]"); printf("\033[0;37m");
                } 
                else{
                    /* Initilize Whitetiles (Across the Board */
                    printf("\033[30;47m"); printf("%s", whiteTile); printf("\033[0;37m");
                    }
        }
        printf("\n\n"); 
    }

}

coordinates getPlayerPiece(bool aTurn, coordinates gamePiece){
    char xLetter;

    /* Get the Piece-Index from the Player that will be stored in gamePiece */
    printf("Select the Piece that you wish to move\n");
    do{
        printf("Row: "); scanf(" %c", &xLetter);
        xLetter = toupper(xLetter);
    }while(xLetter != 'A' && xLetter != 'B' && xLetter != 'C' && xLetter != 'D' && xLetter != 'E' && xLetter != 'F' && xLetter != 'G');
    do{
        printf("Column: "); scanf("%d", &gamePiece.nCol);
    }while(gamePiece.nCol < 1 || gamePiece.nCol > 5);
    
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

coordinates getNextMove(bool aTurn, coordinates gamePiece, coordinates nextMove){
    
    //todo: DO DEBUGGING

    char xLetter;
    //Debugger
    //printf("nRow: %d\nCol: %d\n", gamePiece.nRow, gamePiece.nCol);

    /* Get Coordinates from Player on where the next-move will be, stored in nextMove */

    printf("Select the Tile that you wish to go\n");
    do{
        printf("Row: "); scanf(" %c", &xLetter);
        xLetter = toupper(xLetter);
    }while(xLetter != 'A' && xLetter != 'B' && xLetter != 'C' && xLetter != 'D' && xLetter != 'E' && xLetter != 'F' && xLetter != 'G');
    do{
        printf("Column: "); scanf("%d", &nextMove.nCol);
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

int getTokenIndex(bool aTurn, coordinates gamePiece, board gameBoard)
{
    // declare variables
    int i = 0, j = 0;
    int tokenIndex;
    int found = 0;

    if(aTurn){
        // Get the (x,y) of alpha piece
        while(found == 0){
            if (gameBoard.alpha[i].nRow == gamePiece.nRow && gameBoard.alpha[i].nCol == gamePiece.nCol) {
                tokenIndex = i; // 0,0 
                found = 1;
            }
            else{
                i++;
            }
        }
    } 
    else{
        // Get the (x,y) of beta piece
        while(found == 0){
            if (gameBoard.beta[j].nRow == gamePiece.nRow && gameBoard.beta[j].nCol == gamePiece.nCol) {
                tokenIndex = j;
                found = 1;
            }
            else{
                j++; 
            }
        }
    }
    tokenIndex;
    return tokenIndex;
}

board movePiece(bool aTurn, coordinates gamePiece, coordinates nextMove, board gameBoard){

    bool isOccupied = false, canAttack = false; 
    bool validNextMove = false;
    bool checkOccupied = false;
    int tokenIndex = getTokenIndex(aTurn, gamePiece, gameBoard);
    int i, j, k;

    printf("Token Index: %d\n", tokenIndex);
    printf("Piece Coordinate: (%d,%d)\n", gameBoard.alpha[tokenIndex].nRow, gameBoard.alpha[tokenIndex].nCol);

    
    /* If aTurn is False, then it is Alpha's Turn to Play */
    if(aTurn){
        
        // validNextMove = Validation 1
        validNextMove = checkMove(aTurn, gamePiece, nextMove, gameBoard);

        // validNextMove = Validation 2
        checkOccupied = FreeOrOccupied(aTurn, gamePiece, nextMove, gameBoard);

        // if (checkOccupied == true) //backend test
        // {
        //     printf("it is occupied. \n");
        // } else
        // {
        //     printf("it is not occupied. \n");
        // }

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
                        printf("gameboard.beta[%d].eaten is true!\n", gameBoard.beta[i]);
                        gameBoard.betaAlive -= 1;
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
        checkOccupied = FreeOrOccupied(aTurn, gamePiece, nextMove, gameBoard);

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
            printf("ValidNextMove is true!\n");
            for(i = 0; i < MAX_PIECES; i++)
            {
                if(gameBoard.alpha[i].nRow == nextMove.nRow && gameBoard.alpha[i].nCol == nextMove.nCol){
                    gameBoard.alpha[i].eaten = true;
                    printf("gameboard.alpha[%d].eaten is true!\n", gameBoard.alpha[i]);
                    gameBoard.alphaAlive -= 1;
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
                gameBoard.alpha[k].nRow = 8;

            }
        }
    }
    return gameBoard;
}

// board sortPieces(board gameBoard)
// {

//     int i, j, k;
//     coordinates temp;
//     board assortedPieces;

//     for (i = 0; i < MAX_PIECES - 1; i++){
//         for (j = i + 1; j < MAX_PIECES; j++){

//             /* Sort all ALPHA Pieces based on nRow and nCol */
//             if (gameBoard.alpha[i].nRow > gameBoard.alpha[j].nRow){
//                 temp = gameBoard.alpha[i];
//                 gameBoard.alpha[i] = gameBoard.alpha[j]; 
//                 gameBoard.alpha[j] = temp;
//             } 
//             else if(gameBoard.alpha[i].nRow == gameBoard.alpha[j].nRow){
//                 if (gameBoard.alpha[i].nCol > gameBoard.alpha[j].nCol){
//                     temp = gameBoard.alpha[i];
//                     gameBoard.alpha[i] = gameBoard.alpha[j];
//                     gameBoard.alpha[j] = temp;
//                 }
//             }

//             /* Sort all BETA Pieces based on nRow and nCol */
//             if (gameBoard.beta[i].nRow > gameBoard.beta[j].nRow) {
//                 temp = gameBoard.beta[i];
//                 gameBoard.beta[i] = gameBoard.beta[j];
//                 gameBoard.beta[j] = temp;
//             } else if (gameBoard.beta[i].nRow == gameBoard.beta[j].nRow) {
//                 if (gameBoard.beta[i].nCol > gameBoard.beta[j].nCol){
//                     temp = gameBoard.beta[i];
//                     gameBoard.beta[i] = gameBoard.beta[j];
//                     gameBoard.beta[j] = temp;
//                 }
//             }
//         }
//     }
    
//     for(k = 0; k < MAX_PIECES; k++){
//         if(gameBoard.beta[k].eaten == true){
//             gameBoard.beta[k].nRow = 8;
//             gameBoard.beta[k].nCol = 8;

//         }
//         if(gameBoard.alpha[k].eaten == true){
//             gameBoard.alpha[k].nRow = 8;
//             gameBoard.alpha[k].nRow = 8;

//         }
//     }

//     //For readability
//     assortedPieces = gameBoard;
//     return assortedPieces;
// }

bool validatePiece(coordinates gamePiece, bool aTurn, board gameBoard)
{
    // declare variables
    int i;
    bool valid = false;

        if (aTurn) {
            // check if piece is in alpha array
            for (i = 0; i < MAX_PIECES; i++) {
                if (gameBoard.alpha[i].nRow == gamePiece.nRow && gameBoard.alpha[i].nCol == gamePiece.nCol) {
                    valid = true;
                    break;
                }
            }
        } else {
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

    // validNextMove = Validation 2 !! bugged !!
    checkOccupied = FreeOrOccupied(aTurn, gamePiece, nextMove, gameBoard);

    if (checkOccupied == true) //backend test
    {
        printf("it is occupied. \n");
    } 
    else
    {
        printf("it is not occupied. \n");
    }

    //Validation 3
    if(checkOccupied == true) //will run if occupied ang tiles, no matter if may piece alpha piece or beta piece na occupied
    {
        validNextMove = takeShaded( aTurn, nextMove, gameBoard);
    }

    return validNextMove;
}
//

// 1st Validation for Alpha and Beta Pieces (~~ ok ∧ aTurn ∧ next ∈ Free)
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
                printf("\n||The tile you selected is out of bounds. \nPlease make a new move.||\n"); //CHANGEABLE
            } else {
                printf("\n||Piece can only one tile move forward and diagonal but not backwards and sideways.||"); //CHANGEABLE
                printf("\n||Please make a new move.||\n"); //CHANGEABLE
            }
        }   
        
    return valid;
}

// Validation 2 || chesskers, position, and rows/cols are modified to dstru7.c variables, function name also changed.
//Note: di nagamit yung aTurn variable sa validation 2, we may have to remove it.
bool FreeOrOccupied(bool aTurn, coordinates gamePiece, coordinates nextMove, board gameBoard) 
{
    // declare variables
    int i;
    // int countAlpha = 0;
    // int countBeta = 0; 
    bool occupied = false;

    // // ignore eaten piece indexes, EXCCLUSIVE CODE FOR IGNORING PIECES KAY MARK
    // // for decrementing kung may nakain na na piece, cant remove
    // for (i = 0; i < MAX_PIECES; i++) {
    //     if (gameBoard.alpha[i].nRow == -1) {
    //         countAlpha++;
    //     }
    //     if (gameBoard.beta[i].nRow == -1) {
    //         countBeta++;
    //     }
    // }
    
    // check if tile is occupied
    // check if tile is occupied
    // for (i = 0; i < MAX_ROWS; i++) {
    //     for (j = 0; j < MAX_COLS; j++) {
    //         // check if tile is either occupied by alpha or beta
    //         if (gameBoard.alpha[countAlpha].nRow == i && gameBoard.alpha[countAlpha].nCol == j) 
    //         {
    //             countAlpha++;
    //             // compare next move to alpha piece
    //             if (nextMove.nRow == i && nextMove.nCol == j) 
    //             {
    //                 occupied = true;
    //                 // break; shoule we remove the break?
    //             }
    //         } 
    //         else if (gameBoard.beta[countBeta].nRow == i && gameBoard.beta[countBeta].nCol == j) 
    //         {
    //             countBeta++;
    //             // compare next move to beta piece
    //             if (nextMove.nRow == i && nextMove.nCol == j) 
    //             {
    //                 occupied = true;
    //                 // break; should we remove break?
    //             }
    //         }
    //     }

    //     if (occupied == true) 
    //     {
    //         break;
    //     }
    // }

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
    gotoxy(tbLeft, yTopHor); printf("%c", 201); // top-left '╔'
    gotoxy(tbRight, yTopHor); printf("%c", 187); // top-right '╗'
    gotoxy(tbLeft, yBotHor); printf("%c", 200); // bottom-left '╚'
    gotoxy(tbRight, yBotHor); printf("%c", 188); // bottom-right '╝'
    
    for (int i = iLHor; i <= iRHor; i++) 
    { 
        gotoxy(i, yTopHor); printf("%c", 205); // top horizontals '═'
        gotoxy(i, yBotHor); printf("%c", 205); // bottom horizontals '═'
    }

    for (int i = iLVer; i <= iRVer; i++) 
    {
        gotoxy(tbLeft, i); printf("%c", 186); // left side vertical '║'
        gotoxy(tbRight, i); printf("%c", 186); // right side vertical '║'
    }
}

void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}

void nullBoard(board gameBoard){

    int i, j;
    int a, b;

    char col[7] = {'A','B','C','D','E','F','G'};
    int row[5] = {1, 2, 3, 4, 5};

    system ("cls"); // clear screen

    gotoxy(11, 4);
    for (a = 0; a < 5; a++)
    {
        printf("\e[1;93m"); printf("%d   ", row[a]); printf("\e[0m");
    }

    printf("\e[1;95m");
    gotoxy(7, 6); printf("%c", col[0]); // A   
    gotoxy(7, 8); printf("%c", col[1]); // B    
    gotoxy(7, 10); printf("%c", col[2]); // C 
    gotoxy(7, 12); printf("%c", col[3]); // D
    gotoxy(7, 14); printf("%c", col[4]); // E
    gotoxy(7, 16); printf("%c", col[5]); // F
    gotoxy(7, 18); printf("%c", col[6]); // G
    printf("\e[0m");

    // tbLeft, tbRight, yTopHor, yBotHor, iLHor(tbleft+1), iRHor(tbRight-1), iLVer(yTopHor+1), iRVer(yBotHor-1)
    printf("\e[1;96m"); boxMaker(3, 32, 2, 20, 4, 31, 3, 19); printf("\e[0m");

    for (i = 10; i < 30; i=i+4) // per row wc is 5
    {
        printf("\e[0;96m");
        gotoxy(i, 6); printf("[ ]"); // 4,4
        gotoxy(i, 8); printf("[ ]");
        gotoxy(i, 10); printf("[ ]");
        gotoxy(i, 12); printf("[ ]");
        gotoxy(i, 14); printf("[ ]");
        gotoxy(i, 16); printf("[ ]");
        gotoxy(i, 18); printf("[ ]");
        printf("\e[0m");
    }

    // added shaded tiles when bien was gone
    for(j = 0; j < 5; j++) // for printing the pieces
    {
        for (i = 0; i < 7; i++)
            if (gameBoard.shadedTile[i][j])
            {
                gotoxy( ( 11 + (j) * 4 ) , (6+(i)*2) ); 
                printf("\e[1;97m"); printf("="); printf("\e[0m");//since we are starting at (0,0), we remove -1 in an=a1+(n-1)d
            }
    }
    
    for(j = 0; j < 5; j++) // for printing the pieces
    {
        if (gameBoard.beta[j].nCol > 5 || gameBoard.beta[j].nRow > 7)
        {
            printf("\0");
        }
        else // ARITHMETIC SEQUENCE TO PRINT THE POSITIONS OF BETA CORRESPONDING TO THE GOTOXY();
        { 
            gotoxy( ( 11 + (gameBoard.beta[j].nCol) * 4 ) , (6+(gameBoard.beta[j].nRow)*2) ); 
            printf("\e[0;91m"); printf("B"); printf("\e[0m");//since we are starting at (0,0), we remove -1 in an=a1+(n-1)d
        }
    }
    
    for(j = 0; j < 5; j++) // for printing the pieces
    {
        if (gameBoard.alpha[j].nCol > 5 || gameBoard.alpha[j].nRow > 7)
        {
            printf("\0");
        }
        else // ARITHMETIC SEQUENCE TO PRINT THE POSITIONS OF ALPHA CORRESPONDING TO THE GOTOXY();
        {
            gotoxy( ( 11 + (gameBoard.alpha[j].nCol) * 4 ) , (6+(gameBoard.alpha[j].nRow)*2) ); 
            printf("\e[0;94m"); printf("A"); printf("\e[0m");//since we are starting at (0,0), we remove -1 in an=a1+(n-1)d
        }
    }

    gotoxy(1,22); printf("\0");

    // gotoxy(5,4); printf("A"); // (row,col)
    // gotoxy((5+(2-1)*4),4); printf("B");  // will be column coordinate of pieces
    
    //gotoxy((5+(5/*gameboard.alpha[j].nrow*/-1)*4),(4+(5/*gameboard.alpha[j].ncol*/-1)*2)); printf("C"); //since we are starting at (0,0), we remove -1
    /*
        x=5,y
        [?] [ ] [ ] [ ] [ ]
        [5] [ ] [ ] [ ] [ ] 
        [ ] [ ] [ ] [ ] [ ] 
        [ ] [ ] [ ] [ ] [ ]
        [ ] [ ] [ ] [ ] [ ] 
        [ ] [ ] [ ] [ ] [ ]
        [ ] [ ] [ ] [ ] [ ]
    */
    
    
    // whatever woooooooooooorks
    // (5+(/coordinate of column/-1))
    

    // arithmetic sequence: an = a1 + (n-1)d
    // a5 = 4 + (4)4

    //gotoxy(50, 50); printf(" ");
}