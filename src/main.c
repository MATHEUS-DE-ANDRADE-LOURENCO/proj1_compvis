/*
* Grupo:
* - Matheus de Andrade Lourenço (RA 10419691)
* - Murillo Cardoso Ferreira (RA 10418082)
* - Pietro Zanaga Neto (RA 10418574)
* - Lucas Tuon de Matos (RA 10417987)
*/

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

// Funções auxiliares
#include "utils.h"
///------------------------------------------------------------------------------

int main(int argc, char *argv[]) {
    (void)argv;

    if (argc != 2) {
        SDL_Log("> Uso: ./<nome_do_executavel> <arquivo de imagem>");
        return SDL_APP_FAILURE;
    }
    atexit(shutdown);

   // Inicialização da SDL

   // Inicializa as janelas e renderizadores
   Window *window_1 = malloc(sizeof(Window));
   *window_1 = (Window){.window = NULL, .renderer = NULL};

   Window *window_2 = malloc(sizeof(Window));
   *window_2 = (Window){.window = NULL, .renderer = NULL};

    // Inicializa a estrutura da imagem
    Image *image = malloc(sizeof(Image));
    *image = (Image){.surface = NULL, 
                     .texture = NULL, 
                     .rect = {.x = 0.0f, .y = 0.0f, .w = 0.0f, .h = 0.0f}};
    
    if (initialize_SDL_Windows_Renderers(window_1, window_2, image) == SDL_APP_FAILURE) {
        free(image);
        free(window_2);
        free(window_1);
        return SDL_APP_FAILURE;
    }

    // Abrindo a fonte arial para o SDL_ttf
    TTF_Font *font = TTF_OpenFont("arial.ttf", 18); 
    if (!font) {
        SDL_Log("> Erro ao carregar fonte: %s", SDL_GetError());
    }

    // Carregando a imagem no arquivo
        char *file = argv[1];
        load_image(file, window_1, image);

        // Converter a imagem para a escala de cinza.
        if (!convert_image_to_grayscale(window_1, image)) {
            SDL_Log("> Falha ao converter a imagem para escala de cinza.");
        }


    Image original_image = *image;
    if (image->surface) {
        original_image.surface = SDL_DuplicateSurface(image->surface);
        if (original_image.surface) {
            original_image.texture = SDL_CreateTextureFromSurface(window_1->renderer, original_image.surface);
            original_image.rect   = image->rect;
        }
    }


    EqualizeButton button = {
        .rect         = {210.0f, 100.0f, 220.0f, 55.0f},  // acima do histograma, centralizado
        .state        = BUTTON_STATE_NORMAL,
        .isEqualized  = false,
        .text         = "Equalizar"
    };


    ImageStats stats;
    calculate_image_stats(image, &stats);


    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        bool button_clicked = false;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
            }

            // Processa eventos do botão
            if (handle_button_event(&button, &event)) {
                button_clicked = true;
            }


            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_S) {
                save_image("output_image.png", image);
            }
        }

        if (button_clicked) {
            if (button.isEqualized) {
                equalize_histogram(window_1, image);
            } else {
                // Reverter para original
                destroy_image(image);

                image->surface = SDL_DuplicateSurface(original_image.surface);
                if (image->surface) {
                    image->texture = SDL_CreateTextureFromSurface(window_1->renderer, image->surface);
                    image->rect    = original_image.rect;
                } else {
                    SDL_Log("> ERRO: Falha ao duplicar surface original!");
                }
            }

            calculate_image_stats(image, &stats);
        }

        render_image(window_1, image);

        render_histogram_graph(window_2, &stats, font);
        render_button(window_2, &button, font);
        SDL_RenderPresent(window_2->renderer);

        SDL_Delay(16);
    }

    destroy_image(&original_image);   

    if (font) TTF_CloseFont(font);

    destroy_image(image);
    destroy_window(window_2);
    destroy_window(window_1);

    free(image);
    free(window_2);
    free(window_1);
    
    return 0;
}
