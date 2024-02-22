// Programmer: Jake Isom
// Class: SD 247 OL
// Date: 2/14/2024
// Assignment: Lab 3 Battle Ship  
// Reference Material: Weeks one - six canvas pages, battleship.c / provided file
// Purpose: To practice writing programs that use of allocated memory, loops, MD Arrays, and C lang

// Including libraries for c programs
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Define constants (avoiding magic numbers)
#define BOARD_SIZE 10
#define NUM_SHIPS 5
#define SHIP_LENGTH_1 2
#define SHIP_LENGTH_2 3
#define SHIP_LENGTH_3 3
#define SHIP_LENGTH_4 4
#define SHIP_LENGTH_5 5
#define MAX_PLACEMENT_ATTEMPTS 1000
#define HORIZONTAL_DIRECTION 0
#define VERTICAL_DIRECTION 1

// Function prototypes
void initializeBoard(char ***board);
void freeBoard(char **board);
void printBoards(char **userBoard, char **compBoard);
void printBoard(char **board, const char *label, int hideShips);
int isValidCoordinate(int row, int col);
int isHit(char **board, int row, int col);
void placeShips(char **board);
int isAllShipsSunk(char **board);
int alreadyGuessed(char **board, int row, int col);

int main()
{
    char **userBoard;  // Declare pointer to hold the user's board
    char **compBoard;  // Declare pointer to hold the computer's board

    srand(time(NULL)); // Seed for randomization

    // Initialize user and computer boards
    initializeBoard(&userBoard);
    initializeBoard(&compBoard);

    // Place ships on user and computer boards
    placeShips(userBoard);
    placeShips(compBoard);

    printf("Welcome to the Battleship Game!\n");

    printBoards(userBoard, compBoard);

    while (1)// Creates a infinite loop to run until all ships are sunk
    {
        int userRow, userCol;
        char comma; // Variable to capture the comma in the input

        // Prompt user for input
        printf("\nYour Turn - Enter coordinates to hit (row, column): ");
        int inputStatus = scanf("%d , %d", &userRow, &userCol); // Try to read both numbers & comma

        // Validate user input
        if (inputStatus != 2 || getchar() != '\n') // Check if exactly 2 numbers were read 
        {
            // If input is not in the expected format, display error message and clear input buffer
            printf("Invalid input, Please enter two numbers separated by a comma. EX: 0,1\n");
            while (getchar() != '\n')
                ;
            continue; // Restart the loop
        }

        // Validate user's guessed coordinates
        if (!isValidCoordinate(userRow, userCol))
        {
            printf("Invalid coordinates. Try again.\n");
            continue;
        }

        // Check if user has already guessed the spot
        if (alreadyGuessed(compBoard, userRow, userCol))
        {
            printf("You've already guessed this spot. Try again.\n");
            continue;
        }

        // Check if user's guess is a hit or a miss
        if (isHit(compBoard, userRow, userCol))
        {
            printf("Hit!\n");
            compBoard[userRow][userCol] = 'H'; // Update computer's board
        }
        else
        {
            printf("Miss!\n");
            compBoard[userRow][userCol] = '0'; // Update computer's board with miss
        }

        // Print boards after user's turn
        printBoards(userBoard, compBoard);

        // Check if all computer's ships are sunk
        if (isAllShipsSunk(compBoard))
        {
            printf("Congratulations! You sunk all the computer's ships. You win!\n");
            break;
        }

        // Computer's randomly chooses
        int compRow = rand() % BOARD_SIZE;
        int compCol = rand() % BOARD_SIZE;

        // Check if computer has already guessed the spot
        if (alreadyGuessed(userBoard, compRow, compCol))
        {
            // Find new coordinates if the spot has already been guessed
            do
            {
                compRow = rand() % BOARD_SIZE;
                compCol = rand() % BOARD_SIZE;
            } while (alreadyGuessed(userBoard, compRow, compCol));
        }

        // Check if computer's guess is a hit or a miss
        if (isHit(userBoard, compRow, compCol))
        {
            printf("Computer hit at %d %d!\n", compRow, compCol);
            userBoard[compRow][compCol] = 'H'; 
        }
        else
        {
            printf("Computer missed at %d %d!\n", compRow, compCol);
            userBoard[compRow][compCol] = '0'; 
        }

        // Print boards after computer's turn
        printBoards(userBoard, compBoard);

        // Check if all user's ships are sunk
        if (isAllShipsSunk(userBoard))
        {
            printf("Computer sunk all your ships. You lose!\n");
            break;
        }
    }

    // Free memory allocated for boards
    freeBoard(userBoard);
    freeBoard(compBoard);

    return 0;
}

// Function to initialize the game board
void initializeBoard(char ***board)
{
    *board = (char **)malloc(BOARD_SIZE * sizeof(char *));
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        (*board)[i] = (char *)malloc(BOARD_SIZE * sizeof(char));
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            (*board)[i][j] = '.';
        }
    }// Using a for loop to iterate through the board 
}// Use of malloc and pointers to alloacate needed space to make board

// Function to free memory allocated for the game board
void freeBoard(char **board)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        free(board[i]);
    }
    free(board);
}

// Function to print both boards, this is the look of the game
void printBoards(char **userBoard, char **compBoard)
{
    printf("\n");
    printBoard(compBoard, "Computer's Board", true); // Hide ships on computer's board
    printf("\t\t\t");
    printBoard(userBoard, "Player's Board", false); // Show all ships on player's board
    printf("\n");
}

// Function to print a single board
void printBoard(char **board, const char *label, int hideShips)
{
    printf("\n%s\n", label);
    printf("   ");
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        printf("%d ", i);
    }
    printf("\n");

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        printf("%d  ", i);
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (hideShips && board[i][j] != 'H' && board[i][j] != '0')
            { // Hide ships on computer's board
                printf(". ");
            }
            else
            {
                printf("%c ", board[i][j]);
            }
        }
        printf("\n");
    }
}// Function to also format the grid layout

int isValidCoordinate(int row, int col)
{
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}// Seing if valid and not hit then hit becomes a 0


int isHit(char **board, int row, int col)
{
    return board[row][col] == 'S';
}// Checking if coordinate is a hit

void placeShips(char **board)
{
    int shipLengths[NUM_SHIPS] = {SHIP_LENGTH_1, SHIP_LENGTH_2, SHIP_LENGTH_3,
     SHIP_LENGTH_4, SHIP_LENGTH_5};// Declaring all ships and their lengths

    for (int i = 0; i < NUM_SHIPS; i++)// Loop will run 5 times 1 for each ship
    {
        int attempts = 0;
        while (attempts < MAX_PLACEMENT_ATTEMPTS)
        { // Limit the number of attempts
            int row = rand() % BOARD_SIZE;
            int col = rand() % BOARD_SIZE;
            int direction = rand() % 2; // Random between horizonatal or vertical

            if (direction == HORIZONTAL_DIRECTION)
            { // Horizontal placement
                if (col + shipLengths[i] <= BOARD_SIZE)// Starting at col and moving to the right
                {
                    int valid = 1;
                    for (int k = col; k < col + shipLengths[i]; k++)
                    {
                        if (board[row][k] != '.')
                        {
                            valid = 0;
                            break;
                        }// Checking if it empty this keeps from overlapping
                    }
                    if (valid)
                    {
                        for (int k = col; k < col + shipLengths[i]; k++)
                        {
                            board[row][k] = 'S';
                        }
                        break; // Exit the loop once ship is placed
                    }
                }
            }
            else
            { // Vertical placement
                if (row + shipLengths[i] <= BOARD_SIZE)
                {
                    int valid = 1;
                    for (int k = row; k < row + shipLengths[i]; k++)
                    {
                        if (board[k][col] != '.')
                        {
                            valid = 0;
                            break;
                        }// Same idea as before now it is moving down instead of right
                    }
                    if (valid)
                    {
                        for (int k = row; k < row + shipLengths[i]; k++)
                        {
                            board[k][col] = 'S';
                        }
                        break; // Exit the loop once ship is placed
                    }
                }
            }
            attempts++;
        }// End of wile loop to try 1000 times to place
        if (attempts == MAX_PLACEMENT_ATTEMPTS)
        { // Print error message if ships cannot be placed
            printf("Error: Unable to place all ships. Please try again.\n");
            exit(1);
        }// If the while loop finishs after 1000 placement attemtps 
    }
}

// Function to check if all ships are sunk
int isAllShipsSunk(char **board)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == 'S')
            {
                return 0; // Not all ships are sunk
            }// When all S are H then all the ships are sank
        }
    }
    return 1; // All ships are sunk
}// Nested for loop to travel through all spaces on the board to check for S

// Function to check if a spot has already been guessed
int alreadyGuessed(char **board, int row, int col)
{
    return board[row][col] == 'H' || board[row][col] == '0';
}// Checking to See if spot is a hit or miss if neither then it is not guessed
