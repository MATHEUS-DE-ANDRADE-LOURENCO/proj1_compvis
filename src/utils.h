/*
* Grupo:
* - Matheus de Andrade Lourenço (RA 10419691)
* - Murillo Cardoso Ferreira (RA 10418082)
* - Pietro Zanaga Neto (RA 10418574)
* - Lucas Tuon de Matos (RA 10417987)
*/

#ifndef PROJ8_UTILS_H
#define PROJ8_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

enum constants
{
  WINDOW_WIDTH = 640,
  WINDOW_HEIGHT = 480,
  WINDOW_TITLE_MAX_LENGTH = 64,
};

typedef struct Window
{
  SDL_Window *window;
  SDL_Renderer *renderer;
} Window;

typedef struct Image
{
  SDL_Surface *surface;
  SDL_Texture *texture;
  SDL_FRect rect;
} Image;

typedef struct ImageStats {
  int histogram[256];
  float mean; // media
  float std_dev; // desvio padrao
  char *brightness_level; // clara, media ou escura
  char *contrast_level; // alto, medio ou baixo
} ImageStats;

typedef enum {
    BUTTON_STATE_NORMAL,    // azul normal
    BUTTON_STATE_HOVER,     // azul claro (mouse em cima)
    BUTTON_STATE_PRESSED    // azul escuro (clicado)
} ButtonState;

typedef struct {
    SDL_FRect rect;  // posição e tamanho do botão
    ButtonState state;
    bool isEqualized;  // true = imagem está equalizada
    const char* text; // texto atual do botão
} EqualizeButton;

SDL_AppResult initialize_SDL_Windows_Renderers(Window *window1, Window *window2, Image *image);
void render_image(Window *window, Image *image);
void load_image(const char *filename, Window *window, Image *image);
bool convert_image_to_grayscale(Window *window, Image *image);
void destroy_image(Image *image);
void destroy_window(Window *window);
const char* calculate_brightness_level(float mean);
const char* calculate_contrast_level(float std_dev);
void calculate_image_stats(Image *image, ImageStats *stats);
void draw_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y);
void render_histogram_graph(Window *win_interface, ImageStats *stats, TTF_Font *font);
bool equalize_histogram(Window *window, Image *image);
void shutdown(void);
void render_button(Window *win, EqualizeButton *btn, TTF_Font *font);
bool handle_button_event(EqualizeButton *btn, SDL_Event *event);
bool save_image(const char *filename, Image *image);

#endif // PROJ8_UTILS_H
