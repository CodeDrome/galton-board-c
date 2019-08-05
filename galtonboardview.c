
#include<stdio.h>
#include<time.h>

#include"galtonboardview.h"

#define RED   "\x1B[31m"
#define GREEN "\x1B[32m"
#define RESET "\x1B[0m"

#define X_OFFSET 2
#define Y_OFFSET 6

//-------------------------------------------------------------------------
// FUNCTION gotoxy
//-------------------------------------------------------------------------
static void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}

//--------------------------------------------------------
// FUNCTION galtonboardview_oncreate
//--------------------------------------------------------
void galtonboardview_oncreate(galtonboard* gb)
{
    // clear screen
    printf("\e[1;1H\e[2J");

    puts("-----------------");
    puts("| codedrome.com |");
    puts("| Galton Board  |");
    puts("-----------------\n");

    // iterate rows and columns to draw board
    for(int r = 0; r < gb->gridrows; r++)
    {
        putchar(' ');

        for(int c = 0; c < gb->gridcolumns; c++)
        {
            putchar(gb->board[r][c]);
        }

        puts("");
    }

    puts("");

    // draw buckets
    for(int r = 0; r < 16; r++)
    {
        for(int c = 0; c <= gb->rowcount + 1; c++)
        {
            printf(GREEN "| " RESET);
        }

        printf(GREEN "%d\n" RESET, abs(r - 16));
    }
}

//--------------------------------------------------------
// FUNCTION galtonboardview_onballmoved
//--------------------------------------------------------
void galtonboardview_onballmoved(galtonboard* gb)
{
    // delete ball if it has a previous position
    if(gb->prevballx >= 0 && gb->prevbally >= 0)
    {
        gotoxy(gb->prevballx + X_OFFSET, gb->prevbally + Y_OFFSET);
	    printf(" ");
	    fflush(stdout);
    }

    // draw ball in new position
	gotoxy(gb->ballx + X_OFFSET, gb->bally + Y_OFFSET);
	printf(RED "o" RESET);
	fflush(stdout);
}

//--------------------------------------------------------
// FUNCTION galtonboardview_ontotalchanged
//--------------------------------------------------------
void galtonboardview_ontotalchanged(galtonboard* gb, int index, int count)
{
    struct timespec t;
    t.tv_sec = 0;
    t.tv_nsec = 1000000 * gb->pause_ms;

    int bottom_of_bucket = 4 + gb->gridrows + 19;
    int bucketx;

    if(index == 0)
    {
        bucketx = 2;
    }
    else
    {
        bucketx = (index + 1) * 2;
    }

    // animate ball into bucket
    int starty = bottom_of_bucket - 17;
    int end_y = bottom_of_bucket - gb->totals[index];

    for(int y = starty; y <= end_y; y++)
    {
        nanosleep(&t, NULL);

        gotoxy(bucketx, y-1);
	    printf(" ");
	    fflush(stdout);

        gotoxy(bucketx, y);
	    printf(RED "o" RESET);
	    fflush(stdout);
    }

    // show totals vertically
    char totalstr[8];
    int total_y = bottom_of_bucket + 1;
    int total_x = 2;

    for(int t = 0; t < gb->rowcount + 1; t++)
    {
        sprintf(totalstr, "%d", gb->totals[t]);

        for(int c = 0; c < 7; c++)
        {
            gotoxy(total_x, total_y + c);
            printf("%c", totalstr[c]);
        }

        total_x += 2;
    }

    // show ball count
    gotoxy(2, bottom_of_bucket + 4);
    printf("Ball %d of %d\n", count, gb->ballcount);
}
