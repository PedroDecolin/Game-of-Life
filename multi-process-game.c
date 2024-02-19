#include <stdio.h>
#include <stdlib.h> // atoi, srandom, random
#include <unistd.h>
#include <termcap.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <time.h>
#include <sys/ipc.h>

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
char *GLOBAL_grid_A, *GLOBAL_grid_B;

// definition of number of children
#define NWORKERS 25
short create_children;
int shmid_kill_children, *kill_children;
int ret_children;

// shared memories and parent semaphore variables
int shmid_grid_A, shmid_grid_B, semid_parent;
struct sembuf sop_parent[1];
struct sembuf sop_children[1];
union semun
{
  int val;
} arg;

// children semaphore variables
int semid_children;

// cell range definitions
typedef struct
{
  int core;
  int offset;
} cell_range;
int extra, std, shmid_grid_cells;
cell_range *grid_cells;

// -1 for INFINITEhar **grid,
// #define NITERS -1
#define NITERS 2000

// children's pids array
pid_t children_pids[NWORKERS];

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
  extra = (GLOBAL_nlines * GLOBAL_ncolumns) % NWORKERS;
  std = (GLOBAL_nlines * GLOBAL_ncolumns - extra) / NWORKERS;

  shmid_grid_cells = shmget(IPC_PRIVATE, NWORKERS * sizeof(cell_range), 00600);
  grid_cells = (cell_range *)shmat(shmid_grid_cells, NULL, 0);

  // first position of array definition
  grid_cells[0].core = 0;
  grid_cells[0].offset = std;
  if (extra > 0)
  {
    grid_cells[0].offset++;
    extra--;
  }

  // definition of NWORKERS - 1 positions of the array
  for (int i = 1; i < NWORKERS; i++)
  {
    grid_cells[i].core = grid_cells[i - 1].core + grid_cells[i - 1].offset;
    grid_cells[i].offset = std;
    if (extra > 0)
    {
      grid_cells[i].offset++;
      extra--;
    }
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
  int k, j, border_north = 0, border_east = 0, border_south = 0, border_west = 0, n_neighbours = 0;
  pid_t parent_pid = getpid();

  // unblock every children
  for (k = 0; k < NWORKERS; k++)
  {
    sop_children->sem_num = k;
    sop_children->sem_op = 1;
    semop(semid_children, sop_children, 1);
  }

  for (j = 0; j < NWORKERS; j++)
  {
    if (parent_pid == getpid() && create_children-- > 0)
    {
      // create children only if is the parent and if there are children to be created
      ret_children = fork();
    }
    if (ret_children == 0)
    { // child
      // calculate his own portion of the grid, defined by grid_cells
      char *aux;
      do
      {
        sop_children->sem_num = j;
        sop_children->sem_op = -1;
        semop(semid_children, sop_children, 1); // block i-child
        for (int i = grid_cells[j].core; i < (grid_cells[j].core + grid_cells[j].offset); i++)
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
        sop_parent->sem_op = -1;
        semop(semid_parent, sop_parent, 1); // decrement parent semaphore
        aux = current;
        current = next;
        next = aux;
      } while ((*kill_children) == 0);
      exit(0);
    }
  }
  sop_parent->sem_op = 0;
  semop(semid_parent, sop_parent, 1); // block parent until every child ends
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
  if (NWORKERS <= 0)
  {
    printf("!!! ERROR : NO WORKERS !!!\n");
    exit(405);
  }
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

  // parent semaphore initialization (starts with NWORKERS)
  semid_parent = semget(IPC_PRIVATE, 1, 00600);
  arg.val = NWORKERS;
  sop_parent->sem_flg = 0;
  sop_parent->sem_num = 0;
  semctl(semid_parent, 0, SETVAL, arg);

  // children semaphores initialization
  semid_children = semget(IPC_PRIVATE, NWORKERS, 00600);
  arg.val = 0;
  sop_children->sem_flg = 0;
  for (int i = 0; i < NWORKERS; i++)
    semctl(semid_children, i, SETVAL, arg);

  // creation of children
  create_children = NWORKERS;
  shmid_kill_children = shmget(IPC_PRIVATE, sizeof(int), 00600);
  kill_children = (int *)shmat(shmid_kill_children, NULL, 0);
  *kill_children = 0;

  // define grids dimensions (GLOBAL_nlines and GLOBAL_ncolumns) depending on
  // pattern
  defineDimensions(pattern);
  if (GLOBAL_nlines * GLOBAL_ncolumns < NWORKERS)
  {
    printf("!!! ERROR : MORE WORKERS THAN CELLS (%d useless workers) !!!\n", NWORKERS - (GLOBAL_nlines * GLOBAL_ncolumns));
    exit(409);
  }

  // alloc the working grids (GLOBAL_grid_A and GLOBAL_grid_B) in shared memory
  shmid_grid_A = shmget(IPC_PRIVATE, (GLOBAL_nlines * GLOBAL_ncolumns * sizeof(char)), 0666);
  GLOBAL_grid_A = (char *)shmat(shmid_grid_A, NULL, 0);
  shmid_grid_B = shmget(IPC_PRIVATE, (GLOBAL_nlines * GLOBAL_ncolumns * sizeof(char)), 0666);
  GLOBAL_grid_B = (char *)shmat(shmid_grid_B, NULL, 0);

  gridInit(&GLOBAL_grid_A, pattern);
  gridInit(&GLOBAL_grid_B, pattern);

  // alloc the start grid (start) in dynamic memory
  char *start = (char *)malloc(GLOBAL_nlines * GLOBAL_ncolumns * sizeof(char));
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
    sop_parent->sem_op = NWORKERS;
    semop(semid_parent, sop_parent, 1); // set again the semaphore to NWORKERS
  } while (niters != NITERS);

  printf("\nFINISHED all iterations\n");
  *kill_children = 1;
  // unblock every children
  for (int i = 0; i < NWORKERS; i++)
  {
    sop_children->sem_num = i;
    sop_children->sem_op = 1;
    semop(semid_children, sop_children, 1);
  }

  // free shared and dynamic memory
  shmctl(shmid_grid_A, IPC_RMID, 0);
  shmctl(shmid_grid_B, IPC_RMID, 0);
  shmctl(shmid_grid_cells, IPC_RMID, 0);
  shmctl(shmid_kill_children, IPC_RMID, 0);
  semctl(semid_parent, 0, IPC_RMID, NULL);
  semctl(semid_children, 0, IPC_RMID, NULL);
  free(start);
  return (0);
}
