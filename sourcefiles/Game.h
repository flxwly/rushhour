#ifndef RUSHHOUR_GAME_H
#define RUSHHOUR_GAME_H


#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Audio.hpp>
#include "Car.hpp"
#include "LevelHandler.h"
#include "Button.h"
#include "Sounds.hpp"

class Game {
public:
    // Der Konstruktor der Game-Klasse
    Game(sf::Vector2i size, sf::Font &font);

    // Die Methode update() wird in der main()-Methode immer
    // wieder für jeden Frame aufgerufen, bevor die render()-Methode
    // aufgerufen wird.
    // Hier befindet sich die eigentliche Logik des Spiels.
    void update();

    // Die Methode render() schreibt alle Grafiken in den Buffer
    // und anschließend wird dieser mit window.display() angezeigt.
    void render();

    // IsRunning() überprüft, ob das Fenster noch offen ist.
    bool isRunning() const { return window.isOpen(); }

private:

    // Einzelne Methoden um bestimmte Teile des Spiels zu rendern
    void renderButtons();
    void renderCars();
    void renderBackground();

    // Eine Variable vom Typ sf::RenderWindow, die das Fenster repräsentiert.
    // sf::RenderWindow ist eine Klasse von SFML und liefert grundlegende Funktionen
    // zur Darstellung von Grafiken. Siehe auch: http://www.sfml-dev.org/documentation/2.0/classsf_1_1RenderWindow.php
    sf::RenderWindow window;

    // Ein Objekt der Soundklasse, welches dazu dient Sounds und Musik zu spielen.
    Sounds sounds;

    // sf::Clock ist eine Klasse von SFML, die die Zeit in Millisekunden
    // ab einem bestimmten Zeitpunkt zurückgeben kann. Der Zeitpunkt kann immer
    // neu gesetzt werden.
    // Siehe auch: https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Clock.php
    sf::Clock blinker;
    bool blink = false;

    // Bei std::map werden statt normalen Indexen Objekte verwendet, die als Schlüssel
    // verwendet werden. Diese Schlüssel sind hier vom Typ std::string, also würde
    // buttons["knopf"] den Button mit dem Schlüssel "knopf" zurückgeben.
    // Siehe auch: https://en.cppreference.com/w/cpp/container/map
    // Abgesehen davon verhält sich std::map wie std::vector.
    std::map<std::string, Button> buttons;

    // Ein std::vector<Typ T> ist eine dynamische Reihung vom Typ T.
    // Siehe auch: https://en.cppreference.com/w/cpp/container/vector
    // In diesem Fall werden hier alle Autos gespeichert, die in dem aktuellen
    // Level vorhanden sind.
    std::vector<Car> cars;

    // In dieser 2-dimensionalen dynamischen Reihung sind Referenzen zu den einzelnen
    // Autos gespeichert. Wenn ein Feld leer ist, dann ist die Referenz ein nullptr
    // (Für mehr Informationen zu Zeigern siehe auch https://en.cppreference.com/w/cpp/language/pointer).
    std::vector<std::vector<Car *>> board;

    // selectedCar ist eine Referenz auf das aktuell ausgewählte Auto. Wenn kein
    // Auto ausgewählt ist, dann ist selectedCar ein nullptr.
    Car *selectedCar = nullptr;
    int currentLevel = 0;
};


#endif //RUSHHOUR_GAME_H
