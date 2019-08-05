
#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>

#include"galtonboard.h"

// --------------------------------------------------------
// FUNCTION PROTOTYPES
// --------------------------------------------------------
static int left_or_right();

//-------------------------------------------------------------------------
// FUNCTION galtonboard_create
//-------------------------------------------------------------------------
galtonboard* galtonboard_create(int rowcount, int ballcount, int pause_ms, void(*oncreate)(galtonboard* gb))
{
    int gridrows = ((rowcount * 2) - 1) + 3;
    int gridcolumns = (rowcount * 2) + 1;

    int rowpegcount = 1;
    int pegsdrawn = 0;
    int firstpegx = floor(gridcolumns / 2);
    int pegx = firstpegx;
    int pegy = 3;

    galtonboard* gb = malloc(sizeof(galtonboard));

    if(gb != NULL)
    {
        // create struct
        *gb = (galtonboard){.board = malloc(gridrows * sizeof(char*)),
                            .rowcount = rowcount,
                            .ballcount = ballcount,
                            .pause_ms = pause_ms,
                            .totals = malloc( (rowcount + 1) * sizeof(int) ),
                            .gridrows = gridrows,
                            .gridcolumns = gridcolumns,
                            .prevballx = -1,
                            .prevbally = -1};

        // if struct created, create 2D array of pegs using letter 'O' to indicate a peg
        if(gb->board != NULL && gb->totals != NULL)
        {
            for(int r = 0; r < gridrows; r++)
            {
                gb->board[r] = malloc(gridcolumns * sizeof(char));

                if(gb->board[r] != NULL)
                {
                    for(int c = 0; c < gridcolumns; c++)
                    {
                        if(r == pegy && c == pegx && pegsdrawn < rowpegcount)
                        {
                            gb->board[r][c] = 'O';

                            pegsdrawn++;

                            pegx+= 2;
                        }
                        else
                        {
                            gb->board[r][c] = ' ';
                        }
                    }

                    if(r > 2 && (r%2) == 0)
                    {
                        rowpegcount++;
                        pegsdrawn = 0;
                        firstpegx--;
                        pegx = firstpegx;
                        pegy+= 2;
                    }
                }
                else
                {
                    galtonboard_free(gb);

                    return NULL;
                }
            }

            // set all totals to 0
            for(int t = 0; t < rowcount + 1; t++)
            {
                gb->totals[t] = 0;
            }

            oncreate(gb);

            return gb;
        }
        else
        {
            galtonboard_free(gb);

            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

//-------------------------------------------------------------------------
// FUNCTION galtonboard_start
//-------------------------------------------------------------------------
void galtonboard_start(galtonboard* gb, void(*onballmoved)(galtonboard* gb), void(*ontotalchanged)(galtonboard* gb, int index, int count))
{
    struct timespec t;
    t.tv_sec = 0;
    t.tv_nsec = 1000000 * gb->pause_ms;

    int totalindex;

    srand(time(NULL));

    // loop for number of balls
    for(int b = 1; b <= gb->ballcount; b++)
    {
        // set ball horizontal position to centre
        gb->ballx = floor(gb->gridcolumns / 2);

        // loop through rows of grid
        for(int r = 0; r < gb->gridrows; r++)
        {
            gb->bally = r;

            // if we hit a peg move left or right
            if(gb->board[r][gb->ballx] == 'O')
            {
                gb->ballx += left_or_right();
            }

            onballmoved(gb);

            gb->prevballx = gb->ballx;
            gb->prevbally = gb->bally;

            if(r < gb->gridrows - 1)
            {
                nanosleep(&t, NULL);
            }
        }

        // calculate index of totals for current ball position
        if(gb->ballx == 0)
        {
            totalindex = 0;
        }
        else
        {
            totalindex = gb->ballx / 2;
        }

        gb->totals[totalindex] += 1;

        ontotalchanged(gb, totalindex, b);
    }
}

//-------------------------------------------------------------------------
// FUNCTION galtonboard_free
//-------------------------------------------------------------------------
void galtonboard_free(galtonboard* gb)
{
    if(gb != NULL)
    {
        if(gb->board != NULL)
        {
            for(int r = 0; r < gb->gridrows; r++)
            {
                free(gb->board[r]);
            }

            free(gb->board);
        }

        free(gb->totals);

        free(gb);
    }
}

// --------------------------------------------------------
// FUNCTION left_or_right
// --------------------------------------------------------
static int left_or_right()
{
    if((rand() % 2) == 0)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}
