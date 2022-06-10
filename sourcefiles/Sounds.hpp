#ifndef RUSHHOUR_SOUNDS_HPP
#define RUSHHOUR_SOUNDS_HPP


#include <SFML/Audio.hpp>
#include <map>


enum class SoundEffect {
    ButtonClick,
    CarMove,
    CarCrash,
    GameWon
};

enum class MusicTrack {
    Game
};

class Sounds {

public:
    Sounds();
    void playSound(SoundEffect sound);
    void playMusic(MusicTrack track);
    void stopMusic();


private:
    std::map<SoundEffect, sf::Sound> sound;
    std::map<SoundEffect, sf::SoundBuffer> soundEffects = std::map<SoundEffect, sf::SoundBuffer>();
    sf::Music backgroundMusic;
};


#endif //RUSHHOUR_SOUNDS_HPP
