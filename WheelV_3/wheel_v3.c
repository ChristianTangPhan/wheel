/**********************************************************************/
/*                                                                    */
/* Program Name: Wheel - Prototype in C                               */
/* Author:       Dudwen                                               */
/* Date Written: December 10, 2024                                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/* This program is for a group of indecisive friends who can't pick a */
/* game! It reads in a text file (with data from the spreadsheet) to  */
/* figure out who is in the group and what games everyone has. Then   */
/* the list is filtered into a wheel to pick your game.               */
/*                                                                    */
/**********************************************************************/

#include <stdio.h>  /* Printf and File stuff                          */
#include <stdlib.h> /* Malloc and free                                */
#include <ctype.h>  /* To lower                                       */
#include <unistd.h> /* Sleep (Pauses program)                         */
#include <time.h>   /* Random number using time                       */
#include <string.h> /* For strcpy, strtok                             */
#include <curl/curl.h> /* For curl functions                          */
#include <ncurses/ncurses.h>  /* For ncurses functions */
#include <windows.h> /* For Windows console control                   */

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
#define HEADER_ROWS       15
#define QUIT              0        /* Party select exit value         */
#define CSV_FILE          "wheel_csv.txt"  
                                   /* Local game file to save to      */
#define GAME_FILE         "wheel_file.txt"  
                                   /* Local game file to save to      */
#define WHEEL_URL         "https://docs.google.com/spreadsheets/d/e/2PACX-1vQfm6we569iWy17cQ2V8JaFNLG-u7P7RO8nx5fH5X_HNfHgr_36yVNE47z27HFvUYiUp1QT5kS92Wzv/pub?gid=0&single=true&output=csv"
                                   /* Google Sheet CSV URL            */

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
void load_data_file(GAME   game_list[MAX_GAMES],
                    PLAYER player_list[MAX_PLAYERS],  
                    int    *p_amount_of_games, 
                    int    *p_amount_of_players);
   /* Read in the data from the game file                             */
int  download_csv(const char *url, const char *filename);

void file_converter(const char *input_file, const char *output_file);

char get_response(int response);
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
void clear_screen();

void  remove_game(WHEEL *p_wheel_list);
   /* Remove a game from the wheel list                               */
void  reset(PLAYER player_list[MAX_PLAYERS], 
            WHEEL  **p_wheel_list, 
            int    amount_of_games, 
            int    amount_of_players, 
            int    *p_party_count,
            char   *p_remove_game_check);
   /* Reset all data except the game file                             */
void  load_data_manual(GAME   game_list[MAX_GAMES],
                       PLAYER player_list[MAX_PLAYERS],  
                       int    *p_amount_of_games, 
                       int    *p_amount_of_players);
   /* Load a presaved version of the wheelfile if there is none       */

void clear_csv();
   /* Clear the data in CSV file                                      */
void clear_game_file();
   /* Clear the data in game file                                     */
void ncurses_setup();

void check_term_size();

int  get_menu_input(int *highlighted_choice, int option_count);

/**********************************************************************/
/*                            Enumerations                            */ 
/**********************************************************************/
enum 
{
    CP_BG = 1,          /* Background / normal text                   */
    CP_HEADER,          /* Headers / title                            */
    CP_NORMAL,          /* Normal menu text                           */
    CP_A,               /* Grade A                                    */
    CP_B,               /* Grade B                                    */
    CP_C,               /* Grade C / N/A                              */
    CP_D,               /* Grade D (orange)                           */
    CP_F,               /* Grade F                                    */
    CP_BORDER,          /* Border color                               */
    CP_FOOTER,          /* Footer / helper text                       */
    CP_DANGER,          /* Deletion-danger                            */
    CP_TITLE,           /* Dialog titles                              */
    CP_HIGHLIGHT_BG,    /* Highlight background                       */
    CP_SCROLL           /* Scroll indicator                           */
};

/**********************************************************************/
/*                           Main Function                            */
/**********************************************************************/
int main()
{
   GAME   game_list[MAX_GAMES];
   PLAYER player_list[MAX_PLAYERS];
   WHEEL  *p_wheel_list             = NULL;
   char   remove_game_check         = 'y';
   int    amount_of_games,
          amount_of_players,
          party_count               = 0,
          player_id;

   /* Automatically resize CMD window to required size BEFORE ncurses */
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   SMALL_RECT windowSize = {0, 0, 59, 45};  /* 120 cols x 45 rows   */
   COORD bufferSize = {120, 9000};           /* Large buffer         */
   
   SetConsoleScreenBufferSize(hConsole, bufferSize);
   SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

   /* Initialize ncurses                                              */
   ncurses_setup();

   //check_term_size();

   /* Print the program heading                                       */
   clear();
   print_heading();
   print_instructions();
   refresh();
   
   /* Loop processing a game to play until the user says to quit      */
   while (get_response(1) == 'y')
   {
      clear_screen();
      refresh();
      
      load_data_file(game_list, player_list, &amount_of_games, 
                                                    &amount_of_players);

      if (amount_of_games > 0 && amount_of_players > 0)
      {
         /* Clear and draw player list ONCE before loop               */
         clear_screen();
         move(HEADER_ROWS - 2, 0);
         clrtoeol();
         printw("Use the number and enter keys to select players");
         print_players(player_list, amount_of_players);
         refresh();

         /* Loop processing party until the user says to quit         */
         while (1)
         {
            /* Display party status (overwrites previous line)        */
            move(HEADER_ROWS + amount_of_players + 4, 0);
            clrtoeol();
            printw("Party (%d):", party_count);
            for (int i = 0; i < amount_of_players; i++)
            {
               if (player_list[i].party_status == 1)
                  printw(" %s", player_list[i].player_name);
            }
            
            /* Display input prompt                                   */
            move(HEADER_ROWS, 0);
            clrtoeol();
            printw("Who do you want to add or remove from the party\n");
            printw("(%d to quit): ", QUIT);
            refresh();
            
            echo();
            curs_set(0);
            scanw("%d", &player_id);
            curs_set(0);
            noecho();
            
            if (player_id <= QUIT)
               break;
               
            party_control(player_list, amount_of_players,
                          player_id, &party_count);
         }

         clear_screen();
         
         if (party_count > 0)
         {
            /* Filter the game list into the wheel list               */
            p_wheel_list = filter_list(game_list, player_list, 
                                       amount_of_games, amount_of_players,
                                       party_count);
            
            if (p_wheel_list != NULL)
            {
               /* Spin the wheel list and pick a game                 */
               while (remove_game_check == 'y' && 
                      p_wheel_list->p_next_game != p_wheel_list)
               {
                  wheel(&p_wheel_list, get_game_count(p_wheel_list)); 
                  selected_game(p_wheel_list->game_name,
                                p_wheel_list->p_next_game->game_name,
                                p_wheel_list->p_next_game->p_next_game->game_name,
                                get_game_count(p_wheel_list));
                  remove_game_check = get_response(3);
                  if ((p_wheel_list != p_wheel_list->p_next_game) &&
                     (remove_game_check == 'y'))
                     remove_game(p_wheel_list);
               }
               selected_game(p_wheel_list->game_name,
                             p_wheel_list->p_next_game->game_name,
                             p_wheel_list->p_next_game->p_next_game->game_name,
                             get_game_count(p_wheel_list));
            }
            else
            {
               clear_screen();
               mvprintw(15, 10, "No games in this list");
               mvprintw(16, 10, "Press any key to continue...");
               refresh();
               getch();
               clear_screen();
            }
         }
         else
         {
            clear_screen();
            mvprintw(15, 10, "No members in the party");
            mvprintw(16, 10, "Press any key to continue...");
            refresh();
            getch();
            clear_screen();
         }

         reset(player_list, &p_wheel_list, amount_of_games,
               amount_of_players, &party_count, &remove_game_check);
      }
   }
   
   /* Cleanup and print goodbye message                               */
   endwin();
   printf("\nThank you for using wheel. Have a nice day! :>\n\n");
   return 0;
}

/**********************************************************************/
/*                     Print the program heading                      */
/**********************************************************************/
void print_heading()
{
   int row = 1;  /* Starting row position */
   
   mvprintw(row++, 0, "========================================================");
   mvprintw(row++, 0, "            Program:   %s", PROGRAM_NAME);
   mvprintw(row++, 0, "            Programer: %s", PROGRAMER_NAME);
   mvprintw(row++, 0, "========================================================");
   
   refresh();  /* Update the screen */
   return;
}

/**********************************************************************/
/*                       Print the instructions                       */
/**********************************************************************/
void print_instructions()
{
   int row = 5;  /* Starting row position (after heading) */
   
   mvprintw(row++, 0, "         ________________________________________");
   mvprintw(row++, 0, "        /                                        \\");
   mvprintw(row++, 0, "        |   !! DO NOT ADJUST THE SCREEN SIZE !!  |");
   mvprintw(row++, 0, "        \\________________________________________/");
   row++;
   mvprintw(row++, 0, "If you do you can adjust it back to fit");
   row++;
   mvprintw(row++, 0, "This program picks a game to play.");
   
   refresh();  /* Update the screen */
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
   int row = 1;        /* Row for status messages                     */

   download_csv(WHEEL_URL, CSV_FILE);

   /* Parse CSV into space-separated format                           */
   file_converter(CSV_FILE, GAME_FILE);

   /* Get info from game file                                         */
   p_game_file = fopen(GAME_FILE, "r");
   if (p_game_file != NULL) 
   {
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
   }
   else
   {
      mvprintw(row++, 0, "No local game file found.");
      mvprintw(row++, 0, "Loading data manually...");
      refresh();
      load_data_manual(game_list, player_list, p_amount_of_games, 
                                                   p_amount_of_players);
   }

   return;
}

/**********************************************************************/
/*              Download CSV from URL and save to file                */
/**********************************************************************/
int download_csv(const char *url, const char *filename)
{
   CURL     *curl_handle;     /* Handle for curl operations            */
   CURLcode res;              /* Result code from curl                 */
   FILE     *p_GAME_FILE;     /* Local file to write to                */
   int      row = HEADER_ROWS; /* Row for status messages               */
   
   clear_screen();
   mvprintw(row, 0, "Downloading CSV from Google Sheets...");
   refresh();
   
   /* Open local file for writing                                     */
   p_GAME_FILE = fopen(filename, "wb");
   if (p_GAME_FILE == NULL)
   {
      mvprintw(row, 0, "Error: Cannot create file '%s'", filename);
      refresh();
      napms(2000);
      return 0;
   }
   
   /* Initialize curl                                                 */
   curl_global_init(CURL_GLOBAL_ALL);
   curl_handle = curl_easy_init();
   
   if (curl_handle) 
   {
      /* Configure curl to download and write to file                 */
      curl_easy_setopt(curl_handle, CURLOPT_URL, url);
      curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, p_GAME_FILE);
      curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, 
                      "Wheel-Program/1.0");
      curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
      curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
      
      /* Perform the download                                         */
      res = curl_easy_perform(curl_handle);
      
      if (res != CURLE_OK) 
      {
         mvprintw(row, 0, "Error: %s", curl_easy_strerror(res));
         refresh();
         napms(2000);
         fclose(p_GAME_FILE);
         curl_easy_cleanup(curl_handle);
         curl_global_cleanup();
         return 0;
      }
      
      mvprintw(row, 0, "Download complete!");
      refresh();
      curl_easy_cleanup(curl_handle);
   }
   else
   {
      mvprintw(row, 0, "Error: Could not initialize curl");
      refresh();
      napms(2000);
      fclose(p_GAME_FILE);
      curl_global_cleanup();
      return 0;
   }
   
   fclose(p_GAME_FILE);
   curl_global_cleanup();
   
   return 1;  /* Success                                              */
}

/**********************************************************************/
/*              Parse CSV and convert to space-separated format       */
/**********************************************************************/
void file_converter(const char *input_file, const char *output_file)
{
   FILE *p_input;          /* Input CSV file                          */
   FILE *p_output;         /* Output space-separated file             */
   char line[256];         /* Buffer to hold each line                */
   int  player_count,      /* Number of players                       */
        game_count;        /* Number of games                         */
   int  player_counter,    /* Count through players                   */
        game_counter;      /* Count through games                     */
   char player_names[MAX_PLAYERS][MAX_PLAYER_NAME]; /* Store names    */
   int  player_limit;      /* Player limit for each game              */
   char game_name[MAX_GAME_NAME]; /* Game name                        */
   char status_char;       /* Individual status character             */
   int  row = 18;           /* Row for status messages                 */
   
   mvprintw(row, 0, "Parsing CSV file...");
   refresh();
   
   /* Open input CSV file                                             */
   p_input = fopen(input_file, "r");
   if (p_input == NULL)
   {
      mvprintw(row, 0, "Error: Cannot open %s", input_file);
      refresh();
      napms(2000);
      return;
   }
   
   /* Skip first line (headers: "Player Count,Game Count,,,," )       */
   fgets(line, sizeof(line), p_input);
   
   /* Read second line with counts                                    */
   fscanf(p_input, "%d,%d", &player_count, &game_count);
   fgets(line, sizeof(line), p_input); /* Skip rest of line           */
   
   /* Read third line and extract player names                        */
   fscanf(p_input, "%*[^,],%*[^,]"); /* Skip "Player Limit,Game"      */
   for (player_counter = 0; 
        player_counter < player_count; 
        player_counter++)
   {
      fscanf(p_input, ",%[^,\n]", player_names[player_counter]);
   }
   fgets(line, sizeof(line), p_input); /* Skip rest of line           */
   
   /* Open output file for writing                                    */
   p_output = fopen(output_file, "w");
   if (p_output == NULL)
   {
      mvprintw(row, 0, "Error: Cannot create %s", output_file);
      refresh();
      napms(2000);
      fclose(p_input);
      return;
   }
   
   /* Write player count and game count (space-separated)             */
   fprintf(p_output, "%d %d\n", player_count, game_count);
   
   /* Write player names (space-separated)                            */
   for (player_counter = 0; 
        player_counter < player_count; 
        player_counter++)
   {
      fprintf(p_output, "%s", player_names[player_counter]);
      if (player_counter < player_count - 1)
         fprintf(p_output, " ");
   }
   fprintf(p_output, "\n");
   
   /* Read and write game data                                        */
   for (game_counter = 0; game_counter < game_count; game_counter++)
   {
      /* Read player limit and game name                              */
      fscanf(p_input, "%d,%[^,]", &player_limit, game_name);
      
      /* Write player limit and game name (space-separated)           */
      fprintf(p_output, "%d %s ", player_limit, game_name);
      
      /* Read each player's status and write as combined string       */
      for (player_counter = 0; 
           player_counter < player_count; 
           player_counter++)
      {
         fscanf(p_input, ",%c", &status_char);
         fprintf(p_output, "%c", status_char);
      }
      fprintf(p_output, "\n");
      
      fgets(line, sizeof(line), p_input); /* Skip rest of line        */
   }
   
   fclose(p_input);
   fclose(p_output);
   
   mvprintw(row, 0, "Parsing complete!");
   refresh();
   
   return;
}

/**********************************************************************/
/*                      Get a yes or no response                      */
/**********************************************************************/
char get_response(int response_type)
{
   int selected = 0;  /* 0 = Yes, 1 = No                              */
   int key;           /* Key pressed by user                          */
   int row = HEADER_ROWS;           /* Row position for prompt        */

   move(HEADER_ROWS - 2, 0);
   clrtoeol();
   printw("Use the arrow and enter keys to select");

   switch(response_type)  /* Print message based on type */
   {
      case 1:
         mvprintw(row, 0, "Spin for game?");
         break;
      case 2:
         move(HEADER_ROWS, 0);
         clrtoeol();
         clear_screen();
         mvprintw(row, 0, "Include games that need updates or downloads?");
         break;
      case 3:
         clear_csv();
         clear_game_file();
         mvprintw(row, 0, "Remove and reroll?");
         break;
   }
   
   /* Loop until user presses Enter                                   */
   while(1)
   {
      /* Display the options with highlighting                        */
      move(row + 2, 5);
      clrtoeol();
      
      if (selected == 0)
         attron(A_REVERSE);  /* Highlight Yes                         */
      printw("[ Yes ]");
      if (selected == 0)
         attroff(A_REVERSE);
      
      printw("    ");
      
      if (selected == 1)
         attron(A_REVERSE);  /* Highlight No                          */
      printw("[ No ]");
      if (selected == 1)
         attroff(A_REVERSE);
      
      refresh();
      
      /* Get user input using menu navigation function                */
      key = get_menu_input(&selected, 2);
      
      /* Check if Enter was pressed                                   */
      if (key == '\n')
      {
         /* Clear the prompt area                                     */
         move(row, 0);
         clrtoeol();
         move(row + 2, 0);
         clrtoeol();
         refresh();
         
         if (selected == 0)
            return 'y';
         else
            return 'n';
      }
   }
}

/**********************************************************************/
/*                     Print the list of players                      */
/**********************************************************************/
void print_players(PLAYER player_list[MAX_PLAYERS], 
                   int    amount_of_players)
{
   int player_counter; /* Count through each player in it's list       */
   int row = HEADER_ROWS + 3; /* Starting row position                        */

   move(row, 0);
   clrtoeol();

   mvprintw(row++, 0, "Players:");
   
   if (amount_of_players > 0)
   {
      for (player_counter = 0; 
           player_counter < amount_of_players; 
           player_counter++)
      {
         mvprintw(row++, 2, "%d. %s", player_counter + 1, 
                  player_list[player_counter].player_name);
      }
   }
   else
   {
      mvprintw(row, 2, "Empty!");
   }

   refresh();
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
   int row;            /* Row position for output                     */
   
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
      mvprintw(HEADER_ROWS + amount_of_players + 4, 0, "!!PLAYER NOT IN LIST!!");
      refresh();
      napms(1000);  /* Show error for 1 second                       */
      move(row, 0);
      clrtoeol();
   }
   
   refresh();
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
   int   row = HEADER_ROWS + 20; /* Row for displaying filtered games */

   p_new_game = NULL;     /* New game to add to the wheel list        */

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

   /* Display filtered games message                                  */
   //mvprintw(row++, 0, "Filtered games: ");
   refresh();

   /* Insert the filtered games into a wheel list                     */
   for (game_counter = 0; 
        game_counter < amount_of_games; 
        game_counter++)
      if (game_list[game_counter].wheel_approved == 1)
      {
         //printw("%s ", game_list[game_counter].game_name);
         //refresh();
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
      endwin();  /* End ncurses before error message                  */
      printf("\nError #%d occurred in create_game_node.", 
                                                      INSERT_ALLOC_ERR);
      printf("\nCannot allocate memory for a new game.");
      printf("\nThe program is aborting\n\n");
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

   if (p_insert_game == NULL) 
       new_game->p_next_game = new_game;
   else
   {
      new_game->p_next_game      = p_insert_game->p_next_game;
      p_insert_game->p_next_game = new_game;
   }

   return new_game; /* Return the new tail */
}

/**********************************************************************/
/*        Get a count of all the students in the student list         */
/**********************************************************************/
int get_game_count(WHEEL *p_wheel_list)
{
   WHEEL *p_current_game;
   int   game_count = 0;  /* Count the amount of students in the      */
                          /* student id list                          */

   p_current_game = p_wheel_list->p_next_game;

   while(p_current_game != p_wheel_list)
   {
      p_current_game = p_current_game->p_next_game;
      game_count++;
   }

   return game_count;
}

/**********************************************************************/
/*                   Spin the wheel to pick a game                    */
/**********************************************************************/
void wheel(WHEEL **p_current_game, int game_count) 
{
   int spin_counter,   /* Count wheel rotations                       */
       spin_amount;    /* Random spin amount                          */
   int start_row = HEADER_ROWS + 4;  /* Where wheel starts on screen  */
   int start_col = 8;  /* Left margin                                 */
   
   srand(time(NULL));
   spin_amount = game_count + rand() % ((game_count * 2) - game_count + 1);

   if ((*p_current_game) == NULL)
   {
      clear_screen();
      mvprintw(15, 10, "No games in the list...");
      refresh();
      napms(2000);
      exit(NO_LIST_ERR);
   }

   clear_screen(); /* Clear ONCE at the start                         */
   
   /* Draw the wheel FRAME once (doesn't change)                      */
   mvprintw(start_row,      start_col, "               .-------.");
   mvprintw(start_row + 2,  start_col, "   ____________|_______|____________");
   mvprintw(start_row + 3,  start_col, "  |  _      _     _____ _____ _     |");
   mvprintw(start_row + 4,  start_col, "  | / \\  /|/ \\ /|/  __//  __// \\    |");
   mvprintw(start_row + 5,  start_col, "  | | |  ||| |_|||  \\  |  \\  | |    |");
   mvprintw(start_row + 6,  start_col, "  | | |/\\||| | |||  /_ |  /_ | |    |");
   mvprintw(start_row + 7,  start_col, "  | | |/\\||| | |||  /_ |  /_ | |_/\\ |");
   mvprintw(start_row + 8,  start_col, "  | \\_/  \\|\\_/ \\|\\_____\\_____\\____/ |");
   mvprintw(start_row + 9,  start_col, "  |===_______===_______===_______===|");
   mvprintw(start_row + 10, start_col, "  ||*|                           |*||");
   mvprintw(start_row + 11, start_col, "  ||*|                           |*||");  /* Game 1 here */
   mvprintw(start_row + 12, start_col, "  ||*|___________________________|*||");
   mvprintw(start_row + 13, start_col, "  |===_______===_______===_______===|");
   mvprintw(start_row + 14, start_col, "  ||*|\\                          |*||");
   mvprintw(start_row + 15, start_col, "  ||*| >                         |*||");  /* Game 2 (selected) */
   mvprintw(start_row + 16, start_col, "  ||*|/__________________________|*||_");
   mvprintw(start_row + 17, start_col, "  |===_______===_______===_______===|_\\");
   mvprintw(start_row + 18, start_col, "  ||*|                           |*|| \\\\");
   mvprintw(start_row + 19, start_col, "  ||*|                           |*||  ||");  /* Game 3 here */
   mvprintw(start_row + 20, start_col, "  ||*|___________________________|*||  ||");
   mvprintw(start_row + 21, start_col, "  |===___________________________===|  ||");
   mvprintw(start_row + 22, start_col, "  |  /___________________________\\  | (__)");
   mvprintw(start_row + 23, start_col, "  |   |                         |   |");
   mvprintw(start_row + 24, start_col, " _|    \\_______________________/    |_");
   mvprintw(start_row + 25, start_col, "(_____________________________________)");
   
   /* Now animate by ONLY updating the changing parts                 */
   for (spin_counter = 0; spin_counter < spin_amount; spin_counter++)
   { 
      (*p_current_game) = (*p_current_game)->p_next_game;
      
      /* Update ONLY the counter (top of wheel)                       */
      move(start_row + 1, start_col + 15);
      clrtoeol();  /* Clear old number                                */
      printw("|  %3d  |", spin_counter);
      
      /* Update ONLY game 1 name (previous game)                      */
      move(start_row + 11, start_col + 10);
      clrtoeol();
      printw("%20s   |*||", (*p_current_game)->game_name);
      
      /* Update ONLY game 2 name (current/selected game with arrow)   */
      move(start_row + 15, start_col + 10);
      clrtoeol();
      printw("  %20s |*||", (*p_current_game)->p_next_game->game_name);
      
      /* Update ONLY game 3 name (next game)                          */
      move(start_row + 19, start_col + 10);
      clrtoeol();
      printw("%20s   |*||  ||", 
             (*p_current_game)->p_next_game->p_next_game->game_name);
      
      refresh();       /* Show the updates                             */
      napms(50);       /* 50ms delay = smoother (adjust to taste)      */
   }
   
   return;
}

/**********************************************************************/
/*               Print the selected game from the wheel               */
/**********************************************************************/
void selected_game(char previous_game[MAX_GAME_NAME],
                   char selected_game[MAX_GAME_NAME],
                   char next_game[MAX_GAME_NAME],
                   int  game_count)
{
   int start_row = HEADER_ROWS + 4;  /* Where wheel starts on screen  */
   int start_col = 8;  /* Left margin                                 */
   
   clear_screen();  /* Clear screen for final display                 */
   
   /* Draw the complete wheel with final selection                    */
   mvprintw(start_row,      start_col, "               .-------.");
   mvprintw(start_row + 1,  start_col, "               |  %3d  |", game_count);
   mvprintw(start_row + 2,  start_col, "   ____________|_______|____________");
   mvprintw(start_row + 3,  start_col, "  |  _      _     _____ _____ _     |");
   mvprintw(start_row + 4,  start_col, "  | / \\  /|/ \\ /|/  __//  __// \\    |");
   mvprintw(start_row + 5,  start_col, "  | | |  ||| |_|||  \\  |  \\  | |    |");
   mvprintw(start_row + 6,  start_col, "  | | |/\\||| | |||  /_ |  /_ | |    |");
   mvprintw(start_row + 7,  start_col, "  | | |/\\||| | |||  /_ |  /_ | |_/\\ |");
   mvprintw(start_row + 8,  start_col, "  | \\_/  \\|\\_/ \\|\\_____\\_____\\____/ |");
   mvprintw(start_row + 9,  start_col, "  |===_______===_______===_______===|");
   mvprintw(start_row + 10, start_col, "  ||*|                           |*||");
   mvprintw(start_row + 11, start_col, "  ||*|       %20s|*||  __", previous_game);
   mvprintw(start_row + 12, start_col, "  ||*|___________________________|*|| (  )");
   mvprintw(start_row + 13, start_col, "  |===_______===_______===_______===|  ||");
   mvprintw(start_row + 14, start_col, "  ||*|\\                          |*||  ||");
   mvprintw(start_row + 15, start_col, "  ||*| >     %20s|*||  ||", selected_game);
   mvprintw(start_row + 16, start_col, "  ||*|/__________________________|*||_//");
   mvprintw(start_row + 17, start_col, "  |===_______===_______===_______===|_/");
   mvprintw(start_row + 18, start_col, "  ||*|                           |*||");
   mvprintw(start_row + 19, start_col, "  ||*|       %20s|*||", next_game);
   mvprintw(start_row + 20, start_col, "  ||*|___________________________|*||");
   mvprintw(start_row + 21, start_col, "  |===___________________________===|");
   mvprintw(start_row + 22, start_col, "  |  /___________________________\\  |");
   mvprintw(start_row + 23, start_col, "  |   |     %20s|   |", selected_game);
   mvprintw(start_row + 24, start_col, " _|    \\_______________________/    |_");
   mvprintw(start_row + 25, start_col, "(_____________________________________)");
   
   refresh();  /* Display it all                                      */

   return;
}

/**********************************************************************/
/*                     Clear screen below header                      */
/**********************************************************************/
void clear_screen()
{
   int row = HEADER_ROWS;

   while (row < 67)
   {
      move(row, 0);
      clrtoeol();
      row++;
   }

   return;
}

/**********************************************************************/
/*                        Load data from file                         */
/**********************************************************************/
void remove_game(WHEEL *p_game)
{
   WHEEL *p_temp_game; /* Temporary placeholder for freeing           */

   if (p_game != NULL && p_game != p_game->p_next_game) 
   {
      p_temp_game         = p_game->p_next_game;
      p_game->p_next_game = p_game->p_next_game->p_next_game;
      free(p_temp_game);
   }

   return;
}

/**********************************************************************/
/*                        Load data from file                         */
/**********************************************************************/
void reset(PLAYER player_list[MAX_PLAYERS], 
           WHEEL  **p_wheel_list, 
           int    amount_of_games, 
           int    amount_of_players, 
           int    *p_party_count,
           char   *p_remove_game_check)
{
   int player_counter;  /* Count through each player in it's list      */
 WHEEL *p_current_game, /* Point to each game in the list              */
       *p_temp_game;    /* Temporary placeholder for freeing           */

   for (player_counter =  0; 
        player_counter < amount_of_players; 
        player_counter++)
      player_list[player_counter].party_status = 0;

   if (p_wheel_list != NULL && *p_wheel_list != NULL)
   {
      p_current_game = (*p_wheel_list)->p_next_game;

      while (p_current_game != (*p_wheel_list)) 
      {
         p_temp_game    = p_current_game;
         p_current_game = p_current_game->p_next_game;
         free(p_temp_game);
      }
      free((*p_wheel_list));
      (*p_wheel_list) = NULL; 
   }

   *p_party_count       = 0;
   *p_remove_game_check = 'y';

   return;
}

/**********************************************************************/
/*                         Load data manually                         */
/**********************************************************************/
void load_data_manual(GAME   game_list[MAX_GAMES],
                      PLAYER player_list[MAX_PLAYERS],
                      int    *p_amount_of_games, 
                      int    *p_amount_of_players)
{
   int  player_counter, /* Count through each player                  */
        game_counter;   /* Count through each game                    */
   char *token,         /* Pointer for tokenized strings              */
        line1[7],
        line2[MAX_PLAYER_NAME * MAX_PLAYERS],
        line3[(5 + MAX_GAME_NAME + MAX_PLAYERS) * MAX_GAMES];

   /* Initialize hardcoded lines                                      */
   strcpy(line1, "6 47");
   strcpy(line2, "Cavey Deeswa Goater Dudwen Deft Zyn             ");
   strcpy(line3, "0 Abort ynndnn 0 Among_Us nnnynn 4 Apex dnnynn 0 Ark dnnynn 4 Astroneer ynnynn 0 Bluestacks ynnynn 4 Brawlhalla nnnynn 0 Business_Tour ynnynn 0 Crab_Gey ynnynn 0 Cuminme ynnynn 0 Darza ynnynn 0 Destiny_2 nnndnn 0 Diep ynnynn 0 Drunk_Wrest_2 ynnynn 0 E_Od_Oder ynndnn 2 FPS_Chess ynnynn 0 Garrys_Mod ynnynn 4 Godspeed ynnynn 4 Grabity ynnynn 5 Ight dnnynn 0 Itchi ynnynn 5 Leg ynnynn 0 Lethal nnnynn 6 Marvel_Rivals ynnynn 0 Maunt ynnynn 0 Meager ynnynn 0 Mince ynnynn 0 Moomoo ynnnnn 0 Mope ynnnnn 8 Muck ynnynn 0 One_Arm_Robber ynnynn 0 Osu ynnynn 6 Overwatch dnnnnn 0 Party_Games ynnynn 0 Pixel_Gun ynnynn 4 PP ynnynn 5 R6 ynnynn 4 Rain_world ynnynn 5 Ranch ynnynn 0 Roblox ynnynn 0 Rounds ynnynn 0 Shellshock ynnynn 0 Spacewar ynndnn 0 Splitgate ynnynn 0 Terererer ynnynn 4 UCH ynnynn 0 Wargey ynnnnn                                                                                                                     ");

   /* Parse line 1 for number of games and players                    */
   sscanf(line1, "%d %d", p_amount_of_players, p_amount_of_games);

   /* Parse line 2 for player names                                   */
   token = strtok(line2, " ");
   for (player_counter = 0; 
        player_counter < *p_amount_of_players && token != NULL; 
        player_counter++) 
   {
      strcpy(player_list[player_counter].player_name, token);
      token = strtok(NULL, " ");
   }

   /* Parse line 3 for game data                                      */
   token = strtok(line3, " ");
   for (game_counter = 0; 
        game_counter < *p_amount_of_games && token != NULL; 
        game_counter++) 
   {
      /* Get the player_limit for the game                            */
      game_list[game_counter].player_limit = atoi(token);

      /* Get the name of the game                                     */
      token = strtok(NULL, " ");
      strcpy(game_list[game_counter].game_name, token);

      /* Get the players' game status for that game                   */
      token = strtok(NULL, " ");
      strcpy(game_list[game_counter].game_status, token);

      /* Move to the next game                                        */
      token = strtok(NULL, " ");
   }
   
   /* Display completion message                                      */
   mvprintw(3, 0, "Manual data loaded successfully!");
   refresh();

   return;
}

/**********************************************************************/
/*                    Clear the data in CSV file                      */
/**********************************************************************/
void clear_csv()
{
   FILE *p_csv_file;   /* Pointer to CSV file                        */
   
   /* Open file in write mode (truncates/empties the file)            */
   p_csv_file = fopen(CSV_FILE, "w");
   fclose(p_csv_file);
   
   return;
}

/**********************************************************************/
/*                  Clear the data in game file                       */
/**********************************************************************/
void clear_game_file()
{
   FILE *p_game_file;  /* Pointer to game file                       */
   
   /* Open file in write mode (truncates/empties the file)            */
   p_game_file = fopen(GAME_FILE, "w");
   fclose(p_game_file);
   
   return;
}

/**********************************************************************/
/*                    Initializes ncurses settings                    */
/**********************************************************************/
void ncurses_setup()
{
    /* Initializes ncurses mode and settings                          */
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    /* Configures color pairs if terminal supports colors             */
    if (has_colors())
    {
        start_color();

        /* Checks if terminal supports custom color definitions       */
        if (can_change_color())
        {
            /* Defines custom orange color (color index 8)             */
            /* RGB values are 0-1000 in ncurses                        */
            init_color(8, 1000, 647, 0);  
        }

        /* Defines color pairs for UI elements                         */
        init_pair(CP_BG,           COLOR_WHITE,  COLOR_BLACK);
        init_pair(CP_HEADER,       8,            COLOR_BLACK);
        init_pair(CP_NORMAL,       COLOR_WHITE,  COLOR_BLACK);
        init_pair(CP_A,            COLOR_GREEN,  COLOR_BLACK);
        init_pair(CP_B,            COLOR_BLUE,   COLOR_BLACK);
        init_pair(CP_C,            COLOR_WHITE,  COLOR_BLACK);
        init_pair(CP_D,            COLOR_YELLOW, COLOR_BLACK);
        init_pair(CP_F,            COLOR_RED,    COLOR_BLACK);
        init_pair(CP_BORDER,       COLOR_WHITE,  COLOR_BLACK);
        init_pair(CP_FOOTER,       COLOR_WHITE,  COLOR_BLACK);
        init_pair(CP_DANGER,       COLOR_WHITE,  COLOR_RED);
        init_pair(CP_TITLE,        COLOR_WHITE,  COLOR_BLUE);
        init_pair(CP_HIGHLIGHT_BG, COLOR_WHITE,  8);
        init_pair(CP_SCROLL,       8,            COLOR_BLACK);
    }

    /* Sets background color and refreshes screen                     */
    bkgd(COLOR_PAIR(CP_BG));
    refresh();
}

/**********************************************************************/
/*                      Check the terminal size                       */
/**********************************************************************/
void check_term_size()
{
   
   int term_rows,                  /* Terminal rows                   */
       term_cols;                  /* Terminal columns                */
   /* Check minimum terminal size - should pass now!                  */
   getmaxyx(stdscr, term_rows, term_cols);
   
   if (term_cols < 50 || term_rows < 50)
   {
      clear();
      mvprintw(5, 0, "========================================================");
      mvprintw(6, 0, "            ERROR: Terminal Wrong Size");
      mvprintw(7, 0, "========================================================");
      mvprintw(9, 0,  "Current size: %d columns x %d rows", term_cols, term_rows);
      mvprintw(10, 0, "Required:     50 columns x 45 rows (minimum)");
      mvprintw(12, 0, "The window should have auto-resized.");
      mvprintw(13, 0, "If you see this message, please report the issue.");
      mvprintw(15, 0, "Press any key to continue anyway...");
      mvprintw(16, 0, "========================================================");
      refresh();
      getch();
      clear_screen();
   }
}

/**********************************************************************/
/*             Handles keyboard input for menu navigation             */
/**********************************************************************/
int get_menu_input(int *highlighted_choice, int option_count)
{
    int key;    /* Key pressed by user                                */

    /* Gets keyboard input                                             */
    key = getch();

    /* Processes navigation keys                                       */
    switch (key)
    {
        case KEY_LEFT:
            *highlighted_choice = (*highlighted_choice - 1 + option_count) 
                                  % option_count;
            break;
        case KEY_RIGHT:
            *highlighted_choice = (*highlighted_choice + 1) % option_count;
            break;
        case '\n':
            return key;
    }

    return key;
}