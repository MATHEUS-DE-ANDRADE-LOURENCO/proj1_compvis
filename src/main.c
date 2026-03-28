/*
* Grupo:
* - Matheus de Andrade Lourenço (RA 10419691)
* - Murillo Cardoso Ferreira (RA 10418082)
* - Pietro Zanaga Neto (RA 10418574)
*/

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

// Funções auxiliares
# include "utils.h"
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

    // Carregando a imagem no arquivo
        char *file = argv[1];
        load_image(file, window_1, image);

        // Converter a imagem para a escala de cinza.
        if (!convert_image_to_grayscale(window_1, image)) {
            SDL_Log("> Falha ao converter a imagem para escala de cinza.");
        }

        // Ajustar tamanho da janela à imagem
        int image_w = (int)image->rect.w;
        int image_h = (int)image->rect.h;

        int top, left = 0;
        SDL_Log("> w: %d, h: %d", image_w, image_h);
        if(image_w > WINDOW_WIDTH || image_h > WINDOW_HEIGHT) {
            // Obtém o tamanho das bordas da janela 1
            SDL_GetWindowBordersSize(window_1->window, &top, &left, NULL, NULL);
            SDL_Log("> Ajustando tamanho da imagem de (%d, %d) para (%d, %d), e alterando para a posição (%d, %d)",
            WINDOW_WIDTH, WINDOW_HEIGHT, image_w, image_h, left, top);

            SDL_SetWindowSize(window_1->window, image_w, image_h);
            SDL_SetWindowPosition(window_1->window, left, top);

            SDL_SyncWindow(window_1->window);

        }

        // Converter a image para a escala de cinzas
    

   // Realizar a conversão para a escala de cinza
   
   // Mostrar as duas interfaces (?)

    // Destrói os recursos alocados (janelas, renderizadores, superfície, textura, etc.)
    bool mustRefresh = false;

    render_image(window_1, image);

    bool isRunning = true;
    SDL_Event event;
    while (isRunning) {
         
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
            }
        }

        if(mustRefresh) render_image(window_1, image);
    }

 

    destroy_image(image);
    destroy_window(window_2);
    destroy_window(window_1);

    free(image);
    free(window_2);
    free(window_1);
    
    return 0;
}
