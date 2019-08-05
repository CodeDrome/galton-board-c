
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<locale.h>
#include<stdlib.h>

#include"galtonboardview.h"

//--------------------------------------------------------
// FUNCTION main
//--------------------------------------------------------
int main(int argc, char* argv[])
{
    galtonboard* gb = galtonboard_create(7, 40, 100, galtonboardview_oncreate);

    if(gb != NULL)
    {
        // hide cursor
        printf("\e[?25l");
        fflush(stdout);

        galtonboard_start(gb, galtonboardview_onballmoved, galtonboardview_ontotalchanged);

        galtonboard_free(gb);

        // unhide cursor
   	    printf("\e[?25h");
   	    fflush(stdout);

        return EXIT_SUCCESS;
    }
    else
    {
        puts("Cannot create board");

        return EXIT_FAILURE;
    }
}
