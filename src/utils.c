/*
* Grupo:
* - Matheus de Andrade Lourenço (RA 10419691)
* - Murillo Cardoso Ferreira (RA 10418082)
* - Pietro Zanaga Neto (RA 10418574)
* - Lucas Tuon de Matos (RA 10417987)
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

    // Inicia a SDL_ttf
    if (!TTF_Init()) {
        SDL_Log("\t-Erro ao iniciar a SDL_ttf: %s", SDL_GetError());
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

// Classificacao da media da intensidade entre clara, media ou escura
const char* calculate_brightness_level(float mean) {
    if (mean < 85.0f) return "Escura";
    if (mean < 170.0f) return "Media";
    return "Clara";
}

// Classificacao do desvio padrao (contraste) entre alto, medio e baixo
const char* calculate_contrast_level(float std_dev) {
    if (std_dev < 40.0f) return "Baixo";
    if (std_dev < 80.0f) return "Medio";
    return "Alto";
}

void calculate_image_stats(Image *image, ImageStats *stats) {

    if (!image || !image->surface || !stats) return;

    // zerar histograma e variaveis
    for (int i = 0; i <256; i++) stats->histogram[i] = 0;
    double sum_intensities = 0;
    int total_pixels = image->surface->w * image->surface->h;

    // bloqueia a superficie pra nao ter alteracao nos pixels
    SDL_LockSurface(image->surface);
    Uint32 *pixels = (Uint32 *)image->surface->pixels;
    const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(image->surface->format);

    // contafem dos pixels
    for (int i = 0; i < total_pixels; i++) {
        Uint8 r, g, b, a;
        SDL_GetRGBA(pixels[i], format, NULL, &r, &g, &b, &a);
        
        stats->histogram[r]++;
        sum_intensities += r;

    }
    SDL_UnlockSurface(image->surface);

    // calculo da media
    stats->mean = (float)(sum_intensities / total_pixels);

    // calculo do desvio padrao
    double sum_sq_diff = 0;
    for (int i = 0; i < 256; i++) {
        double diff = i - stats->mean;
        sum_sq_diff += stats->histogram[i] * (diff * diff);
    }
    stats->std_dev = sqrtf((float)(sum_sq_diff / total_pixels));

    // atribuicao da intensidade e do contraste
    stats->brightness_level = calculate_brightness_level(stats->mean);
    stats->contrast_level = calculate_contrast_level(stats->std_dev);

    SDL_Log("> Analise concluida: Media %.2f (%s), Desvio %.2f (%s)", 
            stats->mean, stats->brightness_level, 
            stats->std_dev, stats->contrast_level);
}

// Funcao auxiliar pra desenhar o texto
void draw_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y) {
    if (!font || !text) return;

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, 0, white);
    if (!surface) return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FRect dstRect = { (float)x, (float)y, (float)surface->w, (float)surface->h };

    SDL_RenderTexture(renderer, texture, NULL, &dstRect);

    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
}

void render_histogram_graph(Window *win_interface, ImageStats *stats, TTF_Font *font) {
    if (!win_interface || !win_interface->renderer || !stats) return;

    // mudando o fundo da janela da interface
    // essa funcao apaga o fundo por completo para evitar um histograma em cima do outro
    // logo, seria bom usar camadas na hora de implementar o botão
    SDL_SetRenderDrawColor(win_interface->renderer, 30, 30, 30, 255);
    SDL_RenderClear(win_interface->renderer);

    // Configurações de layout
    int off_x = 80;
    int off_y = 420;
    int weight = 512;
    int height = 250;

    // Desenhar os eixos
    SDL_SetRenderDrawColor(win_interface->renderer, 150, 150, 150, 255);
    SDL_RenderLine(win_interface->renderer, (float)off_x, (float)off_y, (float)off_x, (float)(off_y - height)); // Eixo Y
    SDL_RenderLine(win_interface->renderer, (float)off_x, (float)off_y, (float)(off_x + weight), (float)off_y); // Eixo X

    // Procurar o valor maximo no histograma para normalizar a altura
    int max_freq = 0;
    for (int i = 0; i < 256; i++) {
        if (stats->histogram[i] > max_freq) max_freq = stats->histogram[i];
    }

    // Desenhando o grafico
    SDL_SetRenderDrawColor(win_interface->renderer, 200, 200, 200, 255); // Barras cinza claro
    for (int i = 0; i < 256; i++) {
        if (stats->histogram[i] > 0) {
            float bar_h = ((float)stats->histogram[i] / max_freq) * height;
            float x_pos = (float)(off_x + i * 2);
            SDL_RenderLine(win_interface->renderer, x_pos, (float)off_y, x_pos, (float)(off_y - bar_h));
        }
    }

    // Adicionar as classificaos e as labels
    if (font) {
        
        draw_text(win_interface->renderer, font, "0", off_x - 5, off_y + 5);
        draw_text(win_interface->renderer, font, "128", off_x + 256 - 15, off_y + 5);
        draw_text(win_interface->renderer, font, "255", off_x + 512 - 15, off_y + 5);
        draw_text(win_interface->renderer, font, "Intensidade", off_x + 200, off_y + 30);

        // Informações da Análise
        char buffer[128];
        sprintf(buffer, "Media: %.2f (%s)", stats->mean, stats->brightness_level);
        draw_text(win_interface->renderer, font, buffer, 50, 40);

        sprintf(buffer, "Desvio Padrao: %.2f (%s)", stats->std_dev, stats->contrast_level);
        draw_text(win_interface->renderer, font, buffer, 50, 70);
    }


    SDL_RenderPresent(win_interface->renderer);
}


void shutdown(void)
{
    SDL_Log("> Encerrando o SDL");
    TTF_Quit();
    SDL_Quit();
}

