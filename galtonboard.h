
#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>
#include<math.h>

// --------------------------------------------------------
// STRUCT galtonboard
// --------------------------------------------------------
typedef struct galtonboard
{
    char** board;
    int rowcount;
    int ballcount;
    int* totals;
    int gridrows;
    int gridcolumns;
    int ballx;
    int bally;
    int prevballx;
    int prevbally;
    int pause_ms;
} galtonboard;

// --------------------------------------------------------
// FUNCTION PROTOTYPES
// --------------------------------------------------------
galtonboard* galtonboard_create(int rowcount, int ballcount, int pause_ms, void(*oncreate)(galtonboard* gb));
void galtonboard_start(galtonboard* gb, void(*onballmoved)(galtonboard* gb), void(*ontotalchanged)(galtonboard* gb, int index, int count));
void galtonboard_free(galtonboard* gb);
