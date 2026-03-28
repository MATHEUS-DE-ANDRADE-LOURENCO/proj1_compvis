/*
* Grupo:
* - Matheus de Andrade Lourenço (RA 10419691)
* - Murillo Cardoso Ferreira (RA 10418082)
* - Pietro Zanaga Neto (RA 10418574)
*/

#ifndef PROJ8_UTILS_H
#define PROJ8_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

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

SDL_AppResult initialize_SDL_Windows_Renderers(Window *window1, Window *window2, Image *image);
void render_image(Window *window, Image *image);
void load_image(const char *filename, Window *window, Image *image);
bool convert_image_to_grayscale(Window *window, Image *image);
void destroy_image(Image *image);
void destroy_window(Window *window);
void shutdown(void);

#endif // PROJ8_UTILS_H