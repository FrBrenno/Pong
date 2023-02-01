#include "Components.hpp"

class SoundComponent : public Component
{
private:
    std::map<std::string, Mix_Chunk*> sounds;

public:
    SoundComponent(){}
    ~SoundComponent()
    {
        for (auto& s: sounds)
            Mix_FreeChunk(s.second);
    }

    void addSound(std::string name, std::string path)
    {
        Mix_Chunk *sound;
        if ((sound = Mix_LoadWAV(path.c_str())) == NULL)
            std::cout << "Error: Failed to load sound file: " << Mix_GetError() << std::endl;
        sounds.emplace(name, sound);
    };

    void Play(std::string name)
    {
        Mix_PlayChannel(-1, sounds[name], 0);
    }

};