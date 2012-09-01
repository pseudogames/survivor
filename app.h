#include <SDL.h>
typedef struct {

} Menu;

typedef enum {
  PLAYER_READY,
  PLAYER_IDLE,
} PlayerStatus;

typedef struct{
  PlayerStatus state;
  SDL_Rect *position;
} Player;

typedef struct {
  Player player1;
  Player player2;
} Game;

typedef enum {
  STATE_EXIT,
  STATE_MENU,
} AppState;
typedef struct {
  SDL_Surface *screen;
  Game game;
  AppState state;
  Menu menu;
} App;
