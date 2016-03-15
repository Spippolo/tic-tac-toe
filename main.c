#include <stdlib.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct {
    SDL_Surface* cross;
    SDL_Surface* circle;
    SDL_Surface* screen;
} ttt_items;

int clicked_box(int x, int y) {
    int padding = 6;
    int w = 92;
    int h = 92;
    int b_x, b_y;

    for (int index = 0; index < 9; index++) {
        b_x = padding*((index%3)+1) + w*(index%3);
        b_y = padding*((index/3)+1) + h*(index/3);

        if (b_x <= x && x <= b_x + w && b_y <= y && y <= b_y + h) {
            return index;
        }
    }

    return -1;
}

SDL_Rect* clicked_rect(int index) {
    SDL_Rect* ret = NULL;

    int padding = 6;
    int w = 92;
    int h = 92;

    ret = (SDL_Rect*) malloc(sizeof(SDL_Rect));
    ret->x = padding*((index%3)+1) + w*(index%3);
    ret->y = padding*((index/3)+1) + h*(index/3);
    ret->w = w;
    ret->h = h;

    return ret;
}

void manage_click(ttt_items* items, int x, int y, int click_tracker[], int* round) {
    int index = clicked_box(x, y);

    printf("MANAGE CLICK: index = %d\n", index);
    printf("MANAGE CLICK: round = %d\n", *round);

    if (index > -1 && click_tracker[index] == 0) {
        int cross = *round % 2;

        click_tracker[index] = cross ? 1 : 2;
        SDL_Rect* rect = clicked_rect(index);

        SDL_BlitSurface(cross ? items->cross : items->circle, NULL, items->screen, rect);
        (*round)++;
    }
}

int check_win(int click_tracker[], int value) {
  int wins[8] = {
    0b111000000,
    0b000111000,
    0b000000111,
    0b100100100,
    0b010010010,
    0b001001001,
    0b100010001,
    0b001010100
  };

  int counter = 0;
  for (int i=0;i < 9; i++) {
    if (click_tracker[i] == value) {
      counter += (int) pow(2, 8-i);
    }
  }
  printf("Counter: %d\n", counter);
  for (int i=0; i<8; i++) {
    printf("%d == %d ??\n", wins[i] & counter, wins[i]);
    if ((wins[i] & counter) == wins[i]) {
      return 1;
    }
  }

  return 0;
}

int check_game_status(int click_tracker[], int round) {
    int status = 1;

    if (round == 9) {
        printf("CHECK GAME STATUS: game's over!\n");
        status = 0;
    }

    int cross_win = check_win(click_tracker, 1);
    int circle_win = check_win(click_tracker, 2);
    if (cross_win) {
        printf("CHECK GAME STATUS: cross wins!\n");
        status = 0;
    } else if (circle_win) {
        printf("CHECK GAME STATUS: circle wins!\n");
        status = 0;
    }

    return status;
}

int main(int argc, char *argv[]) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        return 1;
    }

    int win_h, win_w;
    win_h = win_w = 298;

    int running = 1;

    SDL_Window* window = SDL_CreateWindow("Tic Tac Toe",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          win_w, win_h, SDL_WINDOW_BORDERLESS);

    if (window == NULL) {
        return 1;
    }

    SDL_Surface* screen = SDL_GetWindowSurface(window);
    SDL_Surface* background = IMG_Load("assets/schema.png");

    SDL_Rect screen_rect;
    screen_rect.x = 0;
    screen_rect.y = 0;
    screen_rect.w = background->w;
    screen_rect.h = background->h;

    SDL_BlitSurface(background, NULL, screen, &screen_rect);
    SDL_UpdateWindowSurface(window);

    int click_tracker[9] = { 0 };
    int round = 0;

    ttt_items items;
    items.cross = IMG_Load("assets/cross.png");
    items.circle = IMG_Load("assets/circle.png");
    items.screen = screen;

    SDL_Event event;

    while (running) {
        SDL_MouseButtonEvent *mouse;
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouse = (SDL_MouseButtonEvent*) &event;
                manage_click(&items, mouse->x, mouse->y, click_tracker, &round);
                running = check_game_status(click_tracker, round);
                break;
            }
        }
        SDL_UpdateWindowSurface(window);
    }

    SDL_Delay(5000);

    SDL_FreeSurface(items.cross);
    SDL_FreeSurface(items.circle);
    SDL_FreeSurface(background);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
