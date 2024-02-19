#include <stdio.h>
#include <stdlib.h> // atoi, srandom, random
#include <termcap.h>
// add here other necessary include directives
#include <time.h>

#define DEAD '.'
#define ALIVE 'o'
#define DYING '+'

#define BLINKER 0
#define TOAD 1
#define BEACON 2
#define PULSAR 3
#define PENTADECATHLON 4
#define RANDOM 5

#define NLINES_RANDOM 50
#define NCOLUMNS_RANDOM 200

unsigned int GLOBAL_nlines, GLOBAL_ncolumns;
char *GLOBAL_grid_A = NULL, *GLOBAL_grid_B = NULL;

// -1 for INFINITE
// #define NITERS -1
#define NITERS 2000

//////////////////////////////////////////////////////////////////////
void clearScreen()
{
  char buf[1024];
  char *str;

  tgetent(buf, getenv("TERM"));
  str = tgetstr("cl", NULL);
  fputs(str, stdout);
}

//////////////////////////////////////////////////////////////////////
void gridShow(char *grid)
{
  // shows the grid in the screen, one grid line per screen line;
  // do not forget to call clearScreen before calling gridShow
  clearScreen();
  char *ptr;
  int i = 0;
  for (ptr = grid; ptr <= grid + ((GLOBAL_nlines * GLOBAL_ncolumns) - 1);
       ptr++)
  {
    printf("%c", *ptr);
    if (i % GLOBAL_ncolumns == GLOBAL_ncolumns - 1)
    {
      putchar('\n');
    }
    i++;
  }
}

////////////////////////////////////////////////////////////////////
void defineDimensions(int pattern)
{
  // define number of lines (GLOBAL_nlines) and columns (GLOBAL_coluns)
  // of the grids, depending on the pattern (BLINKER, TOAD, ..., RANDOM);
  // for the specific number of lines and columns of each pattern see
  // https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life ;
  // if pattern is RANDOM you must use NLINES_RANDOM and NCOLUMNS_RANDOM
  switch (pattern)
  {
  case 0:
    GLOBAL_nlines = 5;
    GLOBAL_ncolumns = 5;
    break;
  case 1:
    GLOBAL_nlines = 6;
    GLOBAL_ncolumns = 6;
    break;
  case 2:
    GLOBAL_nlines = 6;
    GLOBAL_ncolumns = 6;
    break;
  case 3:
    GLOBAL_nlines = 17;
    GLOBAL_ncolumns = 17;
    break;
  case 4:
    GLOBAL_nlines = 18;
    GLOBAL_ncolumns = 11;
    break;
  case 5:
    GLOBAL_nlines = NLINES_RANDOM;
    GLOBAL_ncolumns = NCOLUMNS_RANDOM;
    break;
  default:
    printf("!!! ERROR : INVALID PATTERN !!!\n");
    exit(404);
    break;
  }
}

////////////////////////////////////////////////////////////////////
void gridInit(char **grid, int pattern)
{
  srand(time(NULL));
  // initializes the starting grid depending on pattern (BLINKER,...);
  // for the initial state (dead/alive) of each cell see
  // https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life ;
  // if pattern is RANDOM you must define the state of the grid cells
  // in a random way, ensuring only a fraction of 10% are alive cells
  *grid = (char *)malloc(GLOBAL_nlines * GLOBAL_ncolumns * sizeof(char));
  char *ptr;
  for (ptr = *grid; ptr <= *grid + ((GLOBAL_nlines * GLOBAL_ncolumns) - 1);
       ptr++)
  {
    *ptr = DEAD;
  }
  if (pattern == 5)
  {
    int num_alive = 0;
    while (num_alive < ((GLOBAL_nlines * GLOBAL_ncolumns) / 10))
    {
      ptr = *grid + (rand() % (GLOBAL_nlines * GLOBAL_ncolumns));
      if (*ptr == DEAD)
      {
        *ptr = ALIVE;
        num_alive++;
      }
    }
  }
  else
  {
    FILE *fp;
    int buffer;
    switch (pattern)
    {
    case 0:
      fp = fopen("./patterns/BLINKER.bin", "rb");
      break;
    case 1:
      fp = fopen("./patterns/TOAD.bin", "rb");
      break;
    case 2:
      fp = fopen("./patterns/BEACON.bin", "rb");
      break;
    case 3:
      fp = fopen("./patterns/PULSAR.bin", "rb");
      break;
    case 4:
      fp = fopen("./patterns/PENTADECATHLON.bin", "rb");
      break;
    default:
      printf("!!! ERROR : INVALID PATTERN !!!\n");
      exit(404);
      break;
    }

    if (fp == NULL)
    {
      printf("!!! ERROR : MISSING IMPORTANT FILE (%d)!!!\n", pattern);
      exit(410);
    }

    while (fread(&buffer, sizeof(int), 1, fp) == 1)
    {
      *(*grid + buffer) = ALIVE;
    }

    fclose(fp);
  }
}

////////////////////////////////////////////////////////////////////////
void gridEvolve(char *current, char *next)
{
  // generates the next grid depending on the content of the current grid,
  // by applying the game of life rules to each cell of the current grid
  int i, border_north = 0, border_east = 0, border_south = 0, border_west = 0,
         n_neighbours = 0;
  for (i = 0; i < (GLOBAL_nlines * GLOBAL_ncolumns); i++)
  {
    border_north = i < GLOBAL_ncolumns ? 1 : 0;
    border_east = i % GLOBAL_ncolumns == GLOBAL_ncolumns - 1 ? 1 : 0;
    border_south = i >= GLOBAL_ncolumns * (GLOBAL_nlines - 1) ? 1 : 0;
    border_west = i % GLOBAL_ncolumns == 0 ? 1 : 0;
    if (!border_north && !border_west)
    {
      // avalia o vizinho NW
      if (*(current + (i - GLOBAL_ncolumns - 1)) != DEAD)
      {
        n_neighbours++;
      }
    }
    if (!border_north)
    {
      // avalia o vizinho N
      if (*(current + (i - GLOBAL_ncolumns)) != DEAD)
      {
        n_neighbours++;
      }
    }
    if (!border_north && !border_east)
    {
      // avalia o vizinho NE
      if (*(current + (i - GLOBAL_ncolumns + 1)) != DEAD)
      {
        n_neighbours++;
      }
    }
    if (!border_west)
    {
      // avalia o vizinho W
      if (*(current + (i - 1)) != DEAD)
      {
        n_neighbours++;
      }
    }
    if (!border_east)
    {
      // avalia o vizinho E
      if (*(current + (i + 1)) != DEAD)
      {
        n_neighbours++;
      }
    }
    if (!border_south && !border_west)
    {
      // avalia o vizinho SW
      if (*(current + (i + GLOBAL_ncolumns - 1)) != DEAD)
      {
        n_neighbours++;
      }
    }
    if (!border_south)
    {
      // avalia o vizinho S
      if (*(current + (i + GLOBAL_ncolumns)) != DEAD)
      {
        n_neighbours++;
      }
    }
    if (!border_south && !border_east)
    {
      // avalia o vizinho SE
      if (*(current + (i + GLOBAL_ncolumns + 1)) != DEAD)
      {
        n_neighbours++;
      }
    }

    switch (*(current + i))
    {
    case ALIVE:
      *(next + i) = DYING;
      break;
    case DYING:
      if ((n_neighbours == 2) || (n_neighbours == 3))
      {
        *(next + i) = DYING;
      }
      else
      {
        *(next + i) = DEAD;
      }
      break;
    case DEAD:
      if (n_neighbours == 3)
      {
        *(next + i) = ALIVE;
      }
      else
      {
        *(next + i) = DEAD;
      }
      break;
    default:
      printf("!!! ERROR : INVALID CELL STATE !!!\n");
      exit(406);
      break;
    }
    n_neighbours = 0;
  }
}

//////////////////////////////////////////////////////////////////////

int gridCompare(char *gridA, char *gridB)
{
  // compares the two input grids, returning 0 if they are equal;
  // usefull to compare the current and the next grid to see if they
  // are equal, in which case the game converged to a final state
  int i, ret = 0;
  for (i = 0; i < (GLOBAL_nlines * GLOBAL_ncolumns); i++)
  {
    if (*(gridA + i) != *(gridB + i))
    {
      ret++;
    }
  }
  return ret;
}

//////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{

  // define initial pattern depending on the command line parameters
  if (argc <= 1)
  {
    printf("!!! ERROR : NOT ENOUGH ARGUMENTS (%d)!!!\n", argc);
    exit(407);
  }
  if (argc > 2)
  {
    printf("!!! ERROR : TOO MANY ARGUMENTS !!!\n");
    exit(408);
  }

  int pattern = atoi(argv[1]), niters = 0;

  // define grids dimensions (GLOBAL_nlines and GLOBAL_ncolumns) depending on
  // pattern
  defineDimensions(pattern);

  // alloc the working grids (GLOBAL_grid_A and GLOBAL_grid_B) in dynamic memory
  gridInit(&GLOBAL_grid_A, pattern);
  gridInit(&GLOBAL_grid_B, pattern);
  char *start = NULL;
  gridInit(&start, pattern);
  // initialize the seed grid (GLOBAL_grid_A) depending on the chosen pattern

  // main loop: WHILE (maximum number of iterations not reached AND convergence
  // not achieved)
  //                 show current generation
  //                 define next generation
  //                 if (convergence not achieved)
  //                     swap pointers between current and next generation
  char *aux, *current = GLOBAL_grid_A, *next = GLOBAL_grid_B;
  do
  {
    gridShow(current);
    gridEvolve(current, next);
    if (gridCompare(next, start) == 0)
    {
      gridShow(next);
      break;
    }
    aux = current;
    current = next;
    next = aux;
    niters++;
  } while (niters != NITERS);

  printf("\nFINISHED all iterations\n");

  // free dynamic memory
  free(GLOBAL_grid_A);
  free(GLOBAL_grid_B);
  free(start);
  exit(0);
}
