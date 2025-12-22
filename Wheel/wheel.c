#include <stdio.h>  /* Printf and File stuff                          */
#include <stdlib.h> /* Malloc and free                                */
#include <ctype.h>  /* To lower                                       */
#include <unistd.h> /* Sleep (Pauses program)                         */
#include <time.h>   /* Random number using time                       */

/**********************************************************************/
/*                         Symbolic Constants                         */
/**********************************************************************/
#define PROGRAM_NAME      "Wheel"  /* The program's name              */
#define PROGRAMER_NAME    "Dudwen" /* The programers's name           */
#define MAX_GAMES         75       /* Max amount of games in a list   */
#define MAX_PLAYERS       10       /* Max amount of players in a list */
#define MAX_GAME_NAME     25       /* Max length of a game's name     */
#define MAX_PLAYER_NAME   20       /* Max length of a players's name  */
#define INSERT_ALLOC_ERR  1        /* Data memory allocation error    */
                                   /* inserting a new game            */
#define NO_LIST_ERR       2        /* No list for the wheel error     */
#define QUIT              0        /* Party select exit value         */

/**********************************************************************/
/*                         Program Structures                         */
/**********************************************************************/
/* Game info                                                          */
struct game
{
   int  player_limit,
        wheel_approved;
   char game_name[MAX_GAME_NAME];     
   char game_status[MAX_GAMES];  
};
typedef struct game GAME;

/* Player info                                                        */
struct player
{
   int  player_id,
        party_status;   
   char player_name[MAX_PLAYER_NAME];   
};
typedef struct player PLAYER;

/* Wheel                                                              */
struct wheel
{
   char         game_name[MAX_GAME_NAME];   
   struct wheel *p_next_game;    
};
typedef struct wheel WHEEL;

/**********************************************************************/
/*                        Function Prototypes                         */
/**********************************************************************/
void print_heading();
   /* Print the program heading                                       */
void print_instructions();
   /* Print the instructions                                          */
void clear_screen();
   /* Clear the screen                                                */
void load_data_file(GAME   game_list[MAX_GAMES],
                    PLAYER player_list[MAX_PLAYERS],  
                    int    *p_amount_of_games, 
                    int    *p_amount_of_players);
   /* Read in the data from the wheelfile.txt                         */
char get_response();
   /* Get a yes or no response                                        */
void print_players(PLAYER player_list[MAX_PLAYERS], 
                   int    amount_of_players);
   /* Print the list of players                                       */
void party_control(PLAYER player_list[MAX_PLAYERS], 
                   int    amount_of_players, 
                   int    player_id, 
                   int    *p_party_count);
   /* Add, drop, and count members in the party                       */
WHEEL *filter_list(GAME   game_list[MAX_GAMES], 
                   PLAYER player_list[MAX_PLAYERS], 
                   int    amount_of_games, 
                   int    amount_of_players, 
                   int    party_count);
   /* Filter the list to games members in the party want to play      */
WHEEL *create_game_node(char game_name[MAX_GAME_NAME]);
   /* Create a new game into the wheel list                           */
WHEEL *insert_game(WHEEL *p_insert_game, char game_name[MAX_GAME_NAME]);
   /* Insert a game into the wheel list                               */
int   get_game_count(WHEEL *p_wheel_list);
   /* Get the amount of games in the wheel list                       */
void  wheel(WHEEL **p_current_game, int amount_of_games);
   /* Spin the wheel and pick a game                                  */
void selected_game(char previous_game[MAX_GAME_NAME],
                   char selected_game[MAX_GAME_NAME],
                   char next_game[MAX_GAME_NAME],
                   int  game_count);
   /* Display the selected game from the wheel                        */
void  remove_game(WHEEL *p_wheel_list);
   /* Remove a game from the wheel list                               */
void  reset(PLAYER player_list[MAX_PLAYERS], 
            WHEEL  **p_wheel_list, 
            int    amount_of_games, 
            int    amount_of_players, 
            int    *p_party_count,
            char   *p_remove_game_check);
   /* Reset all data except the game file                             */
WHEEL  *load_data_manual();
   /* Load a presaved version of the wheelfile if there is none       */

/**********************************************************************/
/*                           Main Function                            */
/**********************************************************************/
int main()
{
   GAME   game_list[MAX_GAMES];
   PLAYER player_list[MAX_PLAYERS];
   WHEEL  *p_wheel_list             = NULL;
   FILE   *p_game_file;
   char   remove_game_check         = 'y';
   int    amount_of_games,
          amount_of_players,
          party_count               = 0,
          player_id;

   /* Print the program heading                                       */
   printf("\n\n\n\n\n\n");
   print_heading();
   print_instructions();

   /* Loop processesing a game to play until the user says to quit    */
   while (get_response(1) == 'y')
   {
      clear_screen();
      p_game_file = fopen("wheelfile.txt", "r");

      if (p_game_file != NULL) 
      {
         load_data_file(game_list, player_list, &amount_of_games, 
                                                    &amount_of_players);

         if (amount_of_games > 0 && amount_of_players > 0)
         {
            /* Loop processesing party until the user says to quit   */
            while (print_players(player_list, amount_of_players),
                  printf("\nWho do you want to add or remove from the party (%d to quit): ", QUIT),
                  scanf ("%d", &player_id), player_id > QUIT)
            {
               clear_screen();
               party_control(player_list, amount_of_players,
                             player_id,   &party_count);
            }
               if (party_count > 0)
               {
               /* Filter the game list into the wheel list            */
               p_wheel_list = filter_list(game_list,       player_list, 
                                          amount_of_games, amount_of_players,
                                          party_count);
               }
               else
                  printf("\nNo members in the party");
               printf("\n");
         }
         else
         {
            printf("\nNo file or list??");
            exit  (NO_LIST_ERR);
         }
      }
      else
      {
         printf(  "No wheelfile.txt found.");
         printf("\nLoading data manually...");
         p_wheel_list = load_data_manual();
      }

      if (p_wheel_list != NULL)
      {
         /* Spin the wheel list and pick a game                 */
         while (remove_game_check == 'y'  && 
                  p_wheel_list->p_next_game != p_wheel_list)
         {
            wheel(&p_wheel_list, get_game_count(p_wheel_list)); 
            selected_game(p_wheel_list->game_name,
                           p_wheel_list->p_next_game->game_name,
                           p_wheel_list->p_next_game->p_next_game->game_name,
                           get_game_count(p_wheel_list));
            remove_game_check = get_response(3);
            if ((p_wheel_list != p_wheel_list->p_next_game) &
               (remove_game_check == 'y'))
               remove_game(p_wheel_list);
         }
         selected_game(p_wheel_list->game_name,
                        p_wheel_list->p_next_game->game_name,
                        p_wheel_list->p_next_game->p_next_game->game_name,
                        get_game_count(p_wheel_list));
      }
      else
         printf("\nNo games in this list");

      reset(player_list,       &p_wheel_list, amount_of_games,
            amount_of_players, &party_count,  &remove_game_check);
      
      printf("\n\n");
   }
   
   /* Print a goodbye message and terminate the program               */
   printf("Thank you for using wheel. Have a nice day! :>");
   printf("\n\n\n\n\n\n");
   return 0;
}

/**********************************************************************/
/*                     Print the program heading                      */
/**********************************************************************/
void print_heading()
{
   printf("\n========================================================");
   printf("\n            Program:   %s", PROGRAM_NAME                 );
   printf("\n            Programer: %s", PROGRAMER_NAME               );
   printf("\n========================================================");

   return;
}

/**********************************************************************/
/*                       Print the instructions                       */
/**********************************************************************/
void print_instructions()
{
   printf("\nThis program picks a game to play."                      );
   printf("\n              _________________________"                 );
   printf("\n             /                         \\"               );
   printf("\n             |   !! FLASH WARNING !!   |"                );
   printf("\n             \\_________________________/"               );
   printf("\nDo not use this program if you are sensitive to flashing");
   return;
}

/**********************************************************************/
/*                          Clear the screen                          */
/**********************************************************************/
void clear_screen()
{
   #ifdef _WIN32
      system("cls");
   #else
      system("clear");
   #endif
   return;
}

/**********************************************************************/
/*                        Load data from file                         */
/**********************************************************************/
void load_data_file(GAME   game_list[MAX_GAMES],
                    PLAYER player_list[MAX_PLAYERS],
                    int    *p_amount_of_games, 
                    int    *p_amount_of_players)
{
   FILE *p_game_file;  /* Points to file containing game and player   */
                       /* information                                 */
   int player_counter, /* Count through each player in it's list      */
       game_counter;   /* Count through each game  in it's list       */

   /* Get info from wheelfile.txt                                     */
   p_game_file = fopen("wheelfile.txt", "r");

   /* Get amount of players and amount of games                    */
   fscanf(p_game_file, "%d %d", 
            p_amount_of_players, p_amount_of_games);

   /* Get list of players                                          */
   for (player_counter = 0; 
         player_counter < *p_amount_of_players; 
         player_counter++)
      fscanf(p_game_file, "%s", player_list[player_counter].player_name);

   /* Get list of games                                            */
   for (game_counter = 0; 
         game_counter < *p_amount_of_games; 
         game_counter++)
      fscanf(p_game_file, "%d %s %s", 
         &game_list[game_counter].player_limit, 
         game_list[game_counter].game_name, 
         game_list[game_counter].game_status);

   fclose(p_game_file);

   return;
}

/**********************************************************************/
/*                      Get a yes or no response                      */
/**********************************************************************/
char get_response(int response_type)
{
   char response[2]; /* Yes or no response                            */

   printf("\n");
   do
   {
      switch(response_type)
      {
         case 1:
            printf("Spin for game? (y or n): ");
            break;
         case 2:
            printf("Include games that need updates or downloads? (y or n): ");
            break;
         case 3:
            printf("Remove and reroll? (y or n): ");
      }
         
      scanf ("%1s", response);
      response[0] = tolower(response[0]);
   } 
   while (response[0] != 'y' && response[0] != 'n');
   
   return response[0];
}

/**********************************************************************/
/*                     Print the list of players                      */
/**********************************************************************/
void print_players(PLAYER player_list[MAX_PLAYERS], 
                   int    amount_of_players)
{
   int player_counter; /* Count through each player in it's list       */

   printf("Players:");
   if (amount_of_players > 0)
      for (player_counter = 0; player_counter < amount_of_players; player_counter++)
         printf("\n  %d. %s", player_counter + 1, 
            player_list[player_counter].player_name);
   else
      printf("\n   Empty!");

   return;
}

/**********************************************************************/
/*             Add, drop, and count members in the party              */
/**********************************************************************/
void party_control(PLAYER player_list[MAX_PLAYERS], 
                   int    amount_of_players, 
                   int    player_id, 
                   int    *p_party_count)
{
   int player_counter; /* Count through each player in it's list      */
   
   /* Update player status and party count                            */
   if (player_id <= amount_of_players)
   {
      if (player_list[player_id - 1].party_status != 1)
      {
         player_list[player_id - 1].party_status = 1;
         *p_party_count += 1;
      }
      else
      {
         player_list[player_id - 1].party_status = 0;
         *p_party_count -= 1;
      }
   }
   else
   {
      printf("!!PLAYER NOT IN LIST!!");
      printf("\n");
   }

   /* Display party count and players                                 */
   printf("Party (%d):", *p_party_count);
   for (player_counter = 0; 
        player_counter < amount_of_players; 
        player_counter++)
      if (player_list[player_counter].party_status == 1)
         printf(" %s", player_list[player_counter].player_name);
   printf("\n");
   
   return;
}


/**********************************************************************/
/*              Filter the game list into the wheel list              */
/**********************************************************************/
WHEEL  *filter_list(GAME   game_list[MAX_GAMES], 
                    PLAYER player_list[MAX_PLAYERS], 
                    int    amount_of_games, 
                    int    amount_of_players, 
                    int    party_count)
{
   WHEEL *p_new_game;     /* New game to add to the wheel list        */
   int   game_counter,    /* Count through each game in it's list     */
         player_counter;  /* Count through each player in it's list   */
   char  willing_to_wait; /* Check if players are will to wait for    */
                          /* updates and downloads                    */

   willing_to_wait = get_response(2);

   /* Set every game to approved                                      */
   for (game_counter =  0;
        game_counter < amount_of_games; 
        game_counter++)
      game_list[game_counter].wheel_approved = 1;

   /* Filter out games                                                */
   for (game_counter =  0;
        game_counter < amount_of_games; 
        game_counter++)
      if (party_count <= game_list[game_counter].player_limit || 
          game_list[game_counter].player_limit == 0)
      {
         for (player_counter = 0; 
              player_counter < amount_of_players; 
              player_counter++)
            if (player_list[player_counter].party_status == 1 &&
                game_list[game_counter].wheel_approved   != 0)
            {
               if (game_list[game_counter].game_status[player_counter] == 'y' ||
                  (game_list[game_counter].game_status[player_counter] == 'd' &&
                   willing_to_wait == 'y'))
                  game_list[game_counter].wheel_approved = 1;
               else
                  game_list[game_counter].wheel_approved = 0;
            }
      }
      else
         game_list[game_counter].wheel_approved = 0;

   /* Insert the filtered games into a wheel list                     */
   for (game_counter = 0; 
        game_counter < amount_of_games; 
        game_counter++)
      if (game_list[game_counter].wheel_approved == 1)
      {
         printf("%s ", game_list[game_counter].game_name);
         p_new_game = insert_game(p_new_game, 
                                    game_list[game_counter].game_name);
      }

   return p_new_game;
}

/**********************************************************************/
/*          Create a new game node to add to the wheel list           */
/**********************************************************************/
WHEEL *create_game_node(char game_name[MAX_GAME_NAME])
{
   WHEEL *new_game; /* New game to add to the list                    */

   if ((new_game = (WHEEL*) malloc(sizeof(WHEEL))) == NULL)
   {
      printf("\nError #%d occurred in create_game_node.", 
                                                      INSERT_ALLOC_ERR);
      printf("\nCannot allocate memory for a new game.");
      printf("\nThe program is aborting");
      exit  (INSERT_ALLOC_ERR);
   }
   strcpy(new_game->game_name, game_name);
   new_game->p_next_game = NULL;

   return new_game;
}

/**********************************************************************/
/*                 Insert a game into the wheel list                  */
/**********************************************************************/
WHEEL *insert_game(WHEEL *p_insert_game, char game_name[MAX_GAME_NAME])
{
   WHEEL *new_game = create_game_node(game_name); /* New game to add  */
                                                  /* to the list      */
   
   printf("inserting game %s\n", game_name);

   if (p_insert_game == NULL) 
       new_game->p_next_game = new_game;
   else
   {
      new_game->p_next_game      = p_insert_game->p_next_game;
      p_insert_game->p_next_game = new_game;
   }

   printf("inserted game %s\n", game_name);

   return new_game; /* Return the new tail */
}

/**********************************************************************/
/*                         Load data manually                         */
/**********************************************************************/
WHEEL *load_data_manual()
{
   WHEEL *p_new_game = NULL; /* New game to add to the wheel list     */
   int   game_counter,       /* Count through each game               */
         amount_of_games;    /* Amount of games in the wheel list     */
   char  *token,             /* Pointer for tokenized strings         */
         game_line[MAX_GAMES * MAX_GAME_NAME];

   /* Initialize hardcoded lines                                      */
   amount_of_games = 47;
   strcpy(game_line, "Abort Among_Us Apex Ark Astroneer Bluestacks Brawlhalla Business_Tour Crab_Gey Cuminme Darza Destiny_2 Diep Drunk_Wrest_2 E_Od_Oder FPS_Chess Garrys_Mod Godspeed Grabity Ight Itchi Leg Lethal Marvel_Rivals Maunt Meager Mince Moomoo Mope Muck One_Arm_Robber Osu Overwatch Party_Games Pixel_Gun PP R6 Rain_world Ranch Roblox Rounds Shellshock Spacewar Splitgate Terererer UCH Wargey                                                                                                                      ");

   /* Parse game line to create wheel list                            */
   token = strtok(game_line, " ");
   for (game_counter = 0; 
        game_counter < amount_of_games;
        game_counter++)
   {
      p_new_game = insert_game(p_new_game, token);
      token = strtok(NULL, " ");
   }

   return p_new_game;
}
