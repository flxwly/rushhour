#include <iostream>
#include <SFML/Audio/Sound.hpp>
#include "Sounds.hpp"

Sounds::Sounds() {
    soundEffects[SoundEffect::ButtonClick] = sf::SoundBuffer();
    soundEffects[SoundEffect::CarMove] = sf::SoundBuffer();
    soundEffects[SoundEffect::CarCrash] = sf::SoundBuffer();
    soundEffects[SoundEffect::GameWon] = sf::SoundBuffer();

    if (!soundEffects[SoundEffect::ButtonClick].loadFromFile("../sounds/buttonClick.wav")) {
        std::cout << "Could not load buttonClick.wav" << std::endl;
    } else {
        sound[SoundEffect::ButtonClick].setBuffer(soundEffects[SoundEffect::ButtonClick]);
    }
    if (!soundEffects[SoundEffect::CarMove].loadFromFile("../sounds/carMove.wav")) {
        std::cout << "Could not load carMove.wav" << std::endl;
    } else {
        sound[SoundEffect::CarMove].setBuffer(soundEffects[SoundEffect::CarMove]);
    }
    if (!soundEffects[SoundEffect::CarCrash].loadFromFile("../sounds/carCrash.wav")) {
        std::cout << "Could not load carCrash.wav" << std::endl;
    } else {
        sound[SoundEffect::CarCrash].setBuffer(soundEffects[SoundEffect::CarCrash]);
    }
    if (!soundEffects[SoundEffect::GameWon].loadFromFile("../sounds/gameWon.wav")) {
        std::cout << "Could not load gameWon.wav" << std::endl;
    } else {
        sound[SoundEffect::GameWon].setBuffer(soundEffects[SoundEffect::GameWon]);
    }
}

void Sounds::playSound(SoundEffect sound) {
    this->sound[sound].play();
}

void Sounds::playMusic(MusicTrack track) {
    if (track == MusicTrack::MyCastleTown) {
        backgroundMusic.openFromFile("../music/My Castle Town.ogg");
        backgroundMusic.setVolume(20);
    }
    backgroundMusic.setLoop(true);
    backgroundMusic.play();
}

void Sounds::stopMusic() {
    backgroundMusic.stop();
}
