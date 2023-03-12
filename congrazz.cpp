#include <SDL_image.h>
#include <SDL.h>
#include <string>

SDL_Surface* LoadImage(std::string file_path, Uint32 format)
{
    SDL_Surface* load_image = NULL;
    SDL_Surface* optimize_image = NULL;
    load_image = IMG_Load(file_path.c_str());
    if (load_image != NULL)
    {
        optimize_image = SDL_ConvertSurfaceFormat(load_image, format, 0);
        SDL_FreeSurface(load_image);
    }
    return optimize_image;
}

int main(int argc, char* argv[])
{
    SDL_Window* window = NULL;
    SDL_Surface* image = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        return 1;
    }

    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 467, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
                break;
            }
        }

        if (image == NULL)
        {
            image = LoadImage("bggame.webp", SDL_GetWindowPixelFormat(window));
            if (image == NULL)
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "LoadImage failed: %s", SDL_GetError());
                running = false;
                break;
            }

            texture = SDL_CreateTextureFromSurface(renderer, image);
            SDL_FreeSurface(image);
            if (texture == NULL)
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
                running = false;
                break;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_Delay(10);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
