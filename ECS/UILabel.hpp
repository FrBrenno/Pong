#include "Components.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class UILabel : public Component
{
private:
    SDL_Rect position;
    std::string labelText;
    std::string path;
    TTF_Font *font;
    int fontSize;
    SDL_Color textColor;
    SDL_Texture *labelTexture;

public:
    UILabel(int posX, int posY, std::string text, int size, SDL_Color &color)
    {
        position.x = posX;
        position.y = posY;
        labelText = text;
        fontSize = size;
        textColor = color;
        path = "assets/Lilita.ttf";

        LoadFont();
        setLabelText();
    }
    ~UILabel()
    {
        if (labelTexture)
            SDL_DestroyTexture(labelTexture);

    }

    void LoadFont()
    {
        font = TTF_OpenFont(path.c_str(), fontSize);
        if (!font)
            std::cout << "Error: Failed to open TTF Font: " << TTF_GetError() << std::endl;
    }

    void setLabelText()
    {
        SDL_Surface *surf = TTF_RenderText_Blended(font, labelText.c_str(), textColor);
        labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
        SDL_FreeSurface(surf);

        SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h); // Set texture width and height
    }

    void draw() override
    {
        SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
    }
};