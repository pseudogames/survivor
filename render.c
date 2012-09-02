#include <SDL_image.h>
#include "render.h"
#include "font.h"
#include "aStarLibrary.h"

SDL_Color red = {0xAA, 0X55, 0x00};
SDL_Color white = {0xFF, 0XFF, 0xFF};
SDL_Color green = {0x00, 0XFF, 0x00};

void renderPlayer(SDL_Surface *screen, Player *player){
	if(player->state != PLAYER_READY) return;
	printf("blitting on X: %i/Y: %i\n", player->body.pos.x, player->body.pos.y);
	int a = player->body.angle;
	SDL_Surface *image;
	if(a >= 315 || a < 45) image = player->right;
	else if(a >= 45 && a < 135) image = player->up;
	else if(a >= 135 && a < 225) image = player->left;
	else if(a >= 225 && a < 315) image = player->down;
	else return;
	SDL_Rect rect = {
		player->body.pos.x - image->w/2,
		player->body.pos.y - image->h/2,
		player->body.pos.w,
		player->body.pos.h
	};
	SDL_BlitSurface(image, NULL, screen, &rect);
}

void renderEnemy(SDL_Surface *screen, Enemy *enemy){
	SDL_Surface *image = enemy->image;
	SDL_Rect rect = {
		enemy->body.pos.x - image->w/2,
		enemy->body.pos.y - image->h/2,
		enemy->body.pos.w,
		enemy->body.pos.h
	};
	SDL_BlitSurface(image, NULL, screen, &rect);
}

void renderEnemies(App *app)
{
  int i = 0;
  for(; i < ENEMY_COUNT; i++)
  {
    if(app->game.enemies[i].state == ENEMY_LIVE)
    {
      renderEnemy(app->screen, &app->game.enemies[i]);
    }
  }
}

void render(App *app){
	int x,y;

	Game game = app->game;

	Uint32 color = SDL_MapRGB(app->screen->format, 33, 33,33 );
	SDL_FillRect(app->screen, NULL , color);

	SDL_BlitSurface(app->game.board.image, NULL, app->screen, NULL);


	//for (x=0; x < mapWidth;x++) {
	//	for (y=0; y < mapHeight;y++) {
	//		if(walkability[x][y]) {
	//			SDL_Rect rect = { x*tileSize, y*tileSize, tileSize, tileSize };
	//			SDL_FillRect(app->screen, &rect , 0xffffff);
	//		}
	//	}
	//}

	renderPlayer(app->screen, &game.player1);
	renderPlayer(app->screen, &game.player2);
  renderEnemies(app);
	//SDL_UpdateRect(app->screen, 0, 0, 0, 0);
	SDL_Flip(app->screen);
}

void renderMapInit(App *app, int map_index) {
	char image_path[256];
	char hit_path[256];
	sprintf(image_path, "data/map%d.bmp", map_index);
	sprintf(hit_path, "data/map%d_hit.bmp", map_index);
	app->game.board.image = IMG_Load(image_path);
	app->game.board.hit = IMG_Load(hit_path);

}
void renderInit(App *app){
	renderMapInit(app, 0);

	app->game.player1.up = IMG_Load("data/soldado1_costas.png");
	app->game.player1.down = IMG_Load("data/soldado1.png");
	app->game.player1.left = IMG_Load("data/soldado1.png");
	app->game.player1.right = IMG_Load("data/soldado1_costas.png");
	app->game.player2.up = IMG_Load("data/engenheiro1.png");
	app->game.player2.down = IMG_Load("data/engenheiro1.png");
	app->game.player2.left = IMG_Load("data/engenheiro1.png");
	app->game.player2.right = IMG_Load("data/engenheiro1.png");
  app->game.enemy_class_medic.image = IMG_Load("data/zombie2.png");
  app->game.enemy_class_soldier.image = IMG_Load("data/zombie2.png");

	app->screen = SDL_SetVideoMode(1024, 768, 32, SDL_HWSURFACE);
}

/**
 *
 * TODO: refactor this to be part of render func
 */
void renderMenu(App *app){
	Uint32 color = SDL_MapRGB(app->screen->format, 33, 33,33 );
	SDL_FillRect(app->screen, NULL , color);
	Menu *menu = &app->menu;
	SDL_Surface *screen = app->screen;

	int resumePadding = 0;
	if(app->state  == STATE_PAUSED){
	  resumePadding = 100;
	  text_write(screen, 100, 250, "resume game", menu->selected == MENU_RESUME);
	}
    text_write_raw(screen, 300, 100, "Survivor", red, 96);

	text_write(screen, 100, 250 + resumePadding, "new game", menu->selected == MENU_NEW_GAME);
	text_write(screen, 100, 450 + resumePadding, "credits", menu->selected == MENU_CREDITS);
	text_write(screen, 100, 550 + resumePadding, "exit", menu->selected == MENU_QUIT);

  SDL_UpdateRect(app->screen, 0, 0, 0, 0);
}

void renderCredits(App *app)
{
	Uint32 color = SDL_MapRGB(app->screen->format, 33, 33,33 );
	SDL_Surface *screen = app->screen;
	SDL_FillRect(screen, NULL , color);

    text_write_raw(screen, 300, 30, "Credits", red, 96);
    text_write_raw(screen, 100, 150, "team", green, 36);
    text_write_raw(screen, 100, 200, "Carlo \"zED\" Caputo", white, 26);
    text_write_raw(screen, 100, 250, "Pedro Mariano", white, 26);
    text_write_raw(screen, 100, 300, "Caires Vinicius", white, 26);

    text_write_raw(screen, 100, 350, "chars", green, 36);
    text_write_raw(screen, 100, 400, "based on http://pixelblock.tumblr.com", white, 26);
    text_write_raw(screen, 100, 450, "modified by Pedro Jatoba", white, 26);

    text_write_raw(screen, 100, 500, "tileset", green, 36);
    text_write_raw(screen, 100, 550, "lost garden", white, 26);
    text_write_raw(screen, 400, 500, "font", green, 36);
    text_write_raw(screen, 400, 550, "Chalkduster, 2008 Apple Computer, Inc", white, 26);


  SDL_UpdateRect(screen, 0, 0, 0, 0);
}

