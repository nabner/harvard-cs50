/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

// variables to keep track of blank tile
int blankRow;
int blankCol;
int userTileRow;
int userTileCol;


int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int(); 
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // initialize blankspace variables to be used in move function
    // blankspace originally stored in bottom right corner
    blankRow = d - 1;
    blankCol = d - 1;
    
    //initiate counter at 1 to count down from d*d
    int counter = 1;

    // iterate over the rows
    for (int row = 0; row < d; row++) {
        
        // iterate over the columns
        for (int col = 0; col < d; col++) {

            //set tile's board value and increment counter
            // at [0][0] board value will start at (d*d - 1)
            board[row][col] = (d*d - counter);
            counter++;

            // if d is even, swap tile values 2 and 1
            if (d % 2 == 0) {
                board[d-1][d-2] = 2;
                board[d-1][d-3] = 1;
            }
            
            //printf used to debug
            //printf("[%i] [%i] = %i\n", row, col, board[row][col]);
        
        } // end of iterating over columns
        
    } // end of iterating over row
    
} // end of init function

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // iterate over the rows
    for (int row = 0; row < d; row++) {
        
        // iterate over the columns
        for (int col = 0; col < d; col++) {
            
            
            // print an underscore instead of a 0 on the board
            if (board[row][col] == 0) {
                printf("__");
            }
            
            // print each number a row at a time
            // %2i prints a space before single digit numbers
            else {
                printf("%2i ", board[row][col]); 
            }
        
        } // end of iterating over columns
        
        // print 2 new lines after each row is complete
        // to create the visual look of a board
        printf("\n\n");
        
    } // end of iterating over row
    
} // end of draw function

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    
    // first, make sure the tile entered is
    // on the board (i.e. within the dimensions)
    if (tile < 1 || tile > (d*d)-1) {
        return false;
    }
    
    // iterate over the row
    for (int row = 0; row < d; row++) {
        
        // iterate over the column
        for (int col = 0; col < d; col++) {
            
            // find the user's tile on the board
            if (tile == board[row][col]) {
                
                    // assign the user tile positions
                    // to 2 temporary variables
                    userTileRow = row; 
                    userTileCol = col;
                
                // printf used to debug
                //printf("tile location is row %i, column %i\n", row, col);
                //printf("blank tile location is row %i, column %i\n", blankRow, blankCol);
                
                // Reminder: blankRow and blankCol initialized in init function
                
                // check if user tile is adjacent to blank tile
                if  ((row == blankRow && col == blankCol + 1) ||
                    (row == blankRow && col == blankCol - 1) ||
                    (row == blankRow + 1 && col == blankCol) ||
                    (row == blankRow - 1 && col == blankCol)) {
                        
                        // create a temporary variable to house the user tile value
                        // change the user tile value to the blank tile value
                        // use the temp variable to reassign the blank tile value
                        int temp = board[userTileRow][userTileCol];
                        board[userTileRow][userTileCol] = board[blankRow][blankCol];
                        board[blankRow][blankCol] = temp;
                        
                        // update blank tile location using the user tile positions
                        blankRow = userTileRow;
                        blankCol = userTileCol;

                        // printf used to debug
                        //printf("New user tile location is row %i, column %i\n", row, col);
                        //printf("New blank tile location is row %i, column %i\n", blankRow, blankCol);
                        
                        return true;
                        
                } // end of checking if tile is adjacent to blank tile
            
            } // end of ensuring user's tile is on the board
            
        } // end of iterating over columns
    
    } //end of iterating over rows
    
    return false; 
    
} // end of move function



/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 * 
 * the game is won when the tiles are at increasing order
 * left to right, top to bottom
 * the blank tile should end at the bottom right where it started
 */
bool won(void)
{
    // initiate counter to check tile values
    int counter = 1;
    
    // iterate over the row
    for (int row = 0; row < d; row++) {
        
        // iterate over the column
        for (int col = 0; col < d; col++) {
            
            // check the values are increasing left to right, top to bottom
            // i.e. at board[0][0] the value should be 1, then increase the counter
            // and check that the next location's value is 2, and so on
            if (board[row][col] == counter) {
                counter++;
            }
        
        } // end of iterating over columns
     
    } //end of iterating over rows 
        
    // if the program loops through all tiles
    // and the final value of the counter is d*d
    // and the blank tile is in the bottom right corner,
    // the game is won! - return true
    if ((counter == d*d) && (board[d-1][d-1] == 0)) {
        return true;
    }   
    
    // if game is not yet won, return false
    else {
        return false;
    }
    
} // end of won function
