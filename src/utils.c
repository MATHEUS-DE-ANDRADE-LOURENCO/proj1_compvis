/*
* Grupo:
* - Matheus de Andrade Lourenço (RA 10419691)
* - Murillo Cardoso Ferreira (RA 10418082)
* - Pietro Zanaga Neto (RA 10418574)
*/

#include "utils.h"

SDL_AppResult initialize_SDL_Windows_Renderers(Window *window1, Window *window2, Image *image)
{
    (void)image;

    SDL_Log("> Inicializando SDL...");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("\t-Erro ao iniciar a SDL: %s", SDL_GetError());
        SDL_Log("< Encerrando inicializacao");
        return SDL_APP_FAILURE;
    }

    SDL_Log("> Criando janelas e renderizadores...");
    if (!SDL_CreateWindowAndRenderer("Imagem", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window1->window, &window1->renderer))
    {
        SDL_Log("\t-Erro ao criar a janela e/ou renderizador (window1): %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Interface", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window2->window, &window2->renderer))
    {
        SDL_Log("\t-Erro ao criar a janela e/ou renderizador (window2): %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

void load_image(const char *filename, Window *window, Image *image)
{
    SDL_Log("> Carregando imagem...");

    if (!filename || !window || !window->renderer || !image)
    {
        SDL_Log("\t-Parametros invalidos para load_image().");
        return;
    }

    destroy_image(image);

    SDL_Surface *loaded_surface = IMG_Load(filename);
    if (!loaded_surface)
    {
        SDL_Log("\t-Erro ao carregar a imagem: %s", SDL_GetError());
        return;
    }

    image->surface = SDL_ConvertSurface(loaded_surface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(loaded_surface);
    if (!image->surface)
    {
        SDL_Log("\t-Erro ao converter a imagem para RGBA32: %s", SDL_GetError());
        return;
    }

    image->texture = SDL_CreateTextureFromSurface(window->renderer, image->surface);
    if (!image->texture)
    {
        SDL_Log("\t-Erro ao criar textura: %s", SDL_GetError());
        return;
    }

    SDL_GetTextureSize(image->texture, &image->rect.w, &image->rect.h);
}

bool convert_image_to_grayscale(Window *window, Image *image)
{
    if (!window || !window->renderer || !image || !image->surface)
    {
        SDL_Log("\t-Parametros invalidos para convert_image_to_grayscale().");
        return false;
    }

    if (!SDL_LockSurface(image->surface))
    {
        SDL_Log("\t-Erro ao travar superficie para conversao em escala de cinza: %s", SDL_GetError());
        return false;
    }

    const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(image->surface->format);
    const size_t pixel_count = (size_t)image->surface->w * (size_t)image->surface->h;
    Uint32 *pixels = (Uint32 *)image->surface->pixels;

    for (size_t i = 0; i < pixel_count; ++i)
    {
        Uint8 r = 0;
        Uint8 g = 0;
        Uint8 b = 0;
        Uint8 a = 0;

        SDL_GetRGBA(pixels[i], format, NULL, &r, &g, &b, &a);

        // I = 0.2125 * R + 0.7154 * G + 0.0721 * B
        int gray = (int)(0.2125f * r + 0.7154f * g + 0.0721f * b + 0.5f);
        if (gray < 0)
        {
            gray = 0;
        }
        else if (gray > 255)
        {
            gray = 255;
        }

        pixels[i] = SDL_MapRGBA(format, NULL, (Uint8)gray, (Uint8)gray, (Uint8)gray, a);
    }

    SDL_UnlockSurface(image->surface);

    if (image->texture)
    {
        SDL_DestroyTexture(image->texture);
        image->texture = NULL;
    }

    image->texture = SDL_CreateTextureFromSurface(window->renderer, image->surface);
    if (!image->texture)
    {
        SDL_Log("\t-Erro ao atualizar textura apos conversao em escala de cinza: %s", SDL_GetError());
        return false;
    }

    return true;
}

void destroy_image(Image *image)
{
    if (!image)
    {
        return;
    }

    if (image->texture)
    {
        SDL_DestroyTexture(image->texture);
        image->texture = NULL;
    }

    if (image->surface)
    {
        SDL_DestroySurface(image->surface);
        image->surface = NULL;
    }
}

void render_image(Window *window, Image *image)
{
  SDL_SetRenderDrawColor(window->renderer, 128, 128, 128, 255);
  SDL_RenderClear(window->renderer);

  SDL_RenderTexture(window->renderer, image->texture, &image->rect, &image->rect);

  SDL_RenderPresent(window->renderer);
}

void destroy_window(Window *window)
{
    if (!window)
    {
        return;
    }

    if (window->renderer)
    {
        SDL_DestroyRenderer(window->renderer);
        window->renderer = NULL;
    }

    if (window->window)
    {
        SDL_DestroyWindow(window->window);
        window->window = NULL;
    }
}



void shutdown(void)
{
    SDL_Log("> Encerrando o SDL");
    SDL_Quit();
}

