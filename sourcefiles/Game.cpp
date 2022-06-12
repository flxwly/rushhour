#include "Game.h"

Game::Game(sf::Vector2i size, sf::Font &font) : window(sf::VideoMode(size.x, size.y), "Rush Hour") {

    // Es werden die Knöpfe an den jeweiligen Positionen erstellt und
    // in einer std::map gespeichert. Der Schlüssel ist immer der erste
    // String bei den Parametern von buttons.emplace(Schlüssel, Wert).
    buttons.emplace("prev",
                    Button(sf::Vector2f(20, size.y - 40), sf::Vector2f(40, 20), sf::Color::Green, sf::Color::Red,
                           sf::Color::Blue, sf::Color::White, "<", font));
    buttons.emplace("next", Button(sf::Vector2f(size.x - 60, size.y - 40), sf::Vector2f(40, 20), sf::Color::Green,
                                   sf::Color::Red, sf::Color::Blue, sf::Color::White, ">", font));
    buttons.emplace("restart",
                    Button(sf::Vector2f(size.x / 2 - 60, size.y - 40), sf::Vector2f(120, 20), sf::Color::Green,
                           sf::Color::Red, sf::Color::Blue, sf::Color::White, "nochmal", font));

    // Zu Beginn ist das Spielfeld leer und das Board besitzt keine Größe.
    this->cars = std::vector<Car>();
    this->board = std::vector<std::vector<Car *>>();

    // Als nächstes wird ein Level geladen
    LevelHandler::loadLevel(&this->cars, &this->board, "original" + std::to_string(currentLevel));

    sounds.playMusic(MusicTrack::MyCastleTown);
}

void Game::update() {

    // sf::Event ist eine Klasse, die Informationen über ein Event enthält.
    // window.pollEvent() liest das nächste Event aus und speichert es
    // in der übergebenen Variable. Solange ein Event vorhanden ist, liefert
    // window.pollEvent() wahr zurück. Es wird für jeden Aufruf von
    // window.pollEvent() immer nur ein Event in der Variable gespeichert.
    // Wenn es mehrere Events gibt, muss window.pollEvent() mehrmals aufgerufen
    // werden. Im Prinzip so lange, bis die Methode falsch zurückliefert.
    sf::Event event;
    while (window.pollEvent(event)) {

        // Wenn das Event vom Typ sf::Event::Closed ist, dann hat
        // der Benutzer auf das rote X geklickt und das Fenster soll geschlossen werden
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Für jeden Button soll geprüft werden, ob dieser gedrückt wurde
        // und dementsprechend gehandelt werden.
        for (const auto &btn: buttons) {
            if (btn.second.isPressed(window, event)) {
                sounds.playSound(SoundEffect::ButtonClick);
                selectedCar = nullptr;

                if (btn.first == "prev") {
                    currentLevel = std::max(0, currentLevel - 1);
                } else if (btn.first == "next") {
                    currentLevel++;
                }
                if (!LevelHandler::loadLevel(&cars, &board, "original" + std::to_string(currentLevel))) {
                    currentLevel = 0;
                    LevelHandler::loadLevel(&cars, &board, "original" + std::to_string(currentLevel));
                }
                break;
            }
        }

        // Wenn das Event vom Typ sf::Event::MouseButtonPressed ist,
        // dann hat der Benutzer eine Maustaste gedrückt.
        // Über event.mouseButton.button == sf::Mouse::Left wird getestet,
        // ob es die linke Maustaste ist
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {


            // Wenn das Spielfeld leer ist, kann keins der einzelnen Felder angeklickt werden.
            // Das Event wird ab hier übersprungen.
            if (board.empty()) {
                continue;
            }

            // event.mouseButton enthält die Position des Mauszeigers, wenn der
            // Eventtyp sf::Event::MouseButtonPressed ist.
            sf::Vector2i cell = sf::Vector2i(
                    (event.mouseButton.x - MARGIN) * (static_cast<int>(board.size())) / GAME_WIDTH,
                    (event.mouseButton.y - MARGIN) * (static_cast<int>(board[0].size())) / GAME_HEIGHT);

            // Wenn die Maus auf ein Feld in der Spielfeld-Matrix geklickt wurde,
            // dann wird geguckt, ob auf ein Feld mit einem Auto geklickt wurde.
            if (cell.x >= 0 && cell.x < board.size() &&
                cell.y >= 0 && cell.y < board[0].size()) {

                // Ein Feld mit einem Auto enthält einen Zeiger auf das jeweilige Auto.
                // Ist der Zeiger ein nullptr, dann ist das Feld leer.
                // (bool) ptr liefert immer dann wahr zurück, wenn ptr kein nullptr ist.
                if (board[cell.x][cell.y]) {
                    // Es wurde ein Auto angeklickt und es soll nun ausgewählt sein.
                    selectedCar = board[cell.x][cell.y];
                } else {
                    // Es wurde ein leeres Feld angeklickt und falls ein Auto ausgewählt ist,
                    // soll dieses abgewählt werden. Andernfalls ist selectedCar schon ein nullptr.
                    selectedCar = nullptr;
                }
            }
        }

        // Wenn das Event vom Typ sf::Event::KeyPressed ist, dann wurde
        // eine Taste auf der Tastatur gedrückt. Wenn es nun noch ein
        // ausgewähltes Auto gibt, so soll dieses bewegt werden.
        // event.key.code enthält die Taste, die gedrückt wurde.
        // sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Up und sf::Keyboard::Down
        // sind die Pfeiltasten.
        if (event.type == sf::Event::KeyPressed && selectedCar) {
            bool shouldMove = true, moved;
            switch (event.key.code) {
                case sf::Keyboard::Left:
                    moved = selectedCar->move({-1, 0}, board);
                    break;
                case sf::Keyboard::Right:
                    moved = selectedCar->move({1, 0}, board);
                    break;
                case sf::Keyboard::Up:
                    moved = selectedCar->move({0, -1}, board);
                    break;
                case sf::Keyboard::Down:
                    moved = selectedCar->move({0, 1}, board);
                    break;
                default:
                    shouldMove = false;
                    break;
            }
            if (shouldMove && moved) {
                sounds.playSound(SoundEffect::CarMove);
            } else if (shouldMove) {
                sounds.playSound(SoundEffect::CarCrash);
            }
        }

        // Das Level ist beendet, wenn keins der Autos, die rauswollen mehr auf dem Brett ist.
        // Dafür wird für jedes Auto geprüft, ob die belegten Felder leer sind.
        bool gameWon = true;
        for (const auto &car: cars) {
            if (car.getOutDir() != sf::Vector2i(0, 0) && !car.getOccupiedPositions().empty()) {
                gameWon = false;
            }
        }

        // Das Level ist abgeschlossen und das nächste soll geladen werden.
        if (gameWon) {
            sounds.playSound(SoundEffect::GameWon);
            selectedCar = nullptr;
            currentLevel++;
            if (!LevelHandler::loadLevel(&cars, &board, "original" + std::to_string(currentLevel))) {
                currentLevel = 0;
            }
        }
    }
}

void Game::render() {

    renderBackground();
    renderCars();
    renderButtons();


    // Blinkanimation für Autos
    if (selectedCar) {
        sf::Vector2f cellSize = sf::Vector2f(GAME_WIDTH / static_cast<float>(board.size()),
                                             GAME_HEIGHT / static_cast<float>(board[0].size()));
        sf::RectangleShape cell(cellSize);
        for (auto segment: selectedCar->getOccupiedPositions()) {
            if (blink) {
                cell.setFillColor(sf::Color(255, 255, 255, 100));
            } else {
                cell.setFillColor(selectedCar->getColor());
            }

            cell.setPosition(MARGIN + static_cast<float>(segment.x) * cellSize.x,
                             MARGIN + static_cast<float>(segment.y) * cellSize.y);
            window.draw(cell);
        }
    }

    if (blinker.getElapsedTime().asSeconds() > 0.5) {
        blink = !blink;
        blinker.restart();
    }

    window.display();
}

void Game::renderCars() {
    if (board.empty()) {
        return;
    }
    sf::Vector2f cellSize = sf::Vector2f(GAME_WIDTH / static_cast<float>(board.size()),
                                         GAME_HEIGHT / static_cast<float>(board[0].size()));


    sf::RectangleShape cell(cellSize);
    sf::RectangleShape importantCell(cellSize / 2.0f);
    importantCell.setOutlineThickness(2);
    importantCell.setOutlineColor(sf::Color::Black);
    for (int x = 0; x < board.size(); x += 1) {
        for (int y = 0; y < board.at(x).size(); y += 1) {
            cell.setPosition(MARGIN + static_cast<float>(x) * cellSize.x,
                             MARGIN + static_cast<float>(y) * cellSize.y);
            cell.setFillColor(board[x][y] == nullptr ? sf::Color::Transparent : board[x][y]->getColor());

            if (board[x][y] && board[x][y]->getOutDir() != sf::Vector2i(0, 0)) {
                importantCell.setPosition(MARGIN + static_cast<float>(x) * cellSize.x + cellSize.x / 4,
                                          MARGIN + static_cast<float>(y) * cellSize.y + cellSize.y / 4);
                importantCell.setFillColor(board[x][y]->getColor());
                window.draw(importantCell);
            }

            window.draw(cell);
        }
    }
}

void Game::renderButtons() {
    for (const auto &btn: buttons) {
        if (btn.second.isPressed(window)) {
            window.draw(btn.second.pressedShape);
        } else if (btn.second.isHover(window)) {
            window.draw(btn.second.hoverShape);
        } else {
            window.draw(btn.second.standardShape);
        }
        window.draw(btn.second.text);
    }
}

void Game::renderBackground() {

    if (board.empty()) {
        return;
    }

    // Hintergrundbild
    sf::RectangleShape background(sf::Vector2f(GAME_WIDTH, GAME_HEIGHT));
    background.setFillColor(sf::Color(255, 255, 255, 100));
    background.setOutlineColor(sf::Color::Black);
    background.setOutlineThickness(MARGIN);
    background.setPosition(MARGIN, MARGIN);
    window.draw(background);

    // Spielfeld
    sf::Vector2f cellSize(GAME_WIDTH / static_cast<float>(board.size()),
                          GAME_HEIGHT / static_cast<float>(board[0].size()));

    // Vertikale Linien
    for (int i = 0; i < board.size() + 1; i++) {
        sf::Vertex line[] =
                {
                        sf::Vertex(sf::Vector2f(static_cast<float>(i) * cellSize.x + MARGIN,
                                                MARGIN), sf::Color::Black),
                        sf::Vertex(sf::Vector2f(static_cast<float>(i) * cellSize.x + MARGIN,
                                                GAME_HEIGHT + MARGIN), sf::Color::Black)
                };
        window.draw(line, 2, sf::Lines);
    }

    // Horizontale Linien
    for (int i = 0; i < board[0].size() + 1; ++i) {
        sf::Vertex line[] =
                {
                        sf::Vertex(sf::Vector2f(MARGIN,
                                                static_cast<float>(i) * cellSize.y + MARGIN), sf::Color::Black),
                        sf::Vertex(sf::Vector2f(GAME_WIDTH + MARGIN,
                                                static_cast<float>(i) * cellSize.y + MARGIN), sf::Color::Black)
                };
        window.draw(line, 2, sf::Lines);
    }

    // Ausgang einzeichnen
    sf::RectangleShape out(cellSize);
    out.setFillColor(sf::Color::White);
    for (auto car: cars) {
        for (auto segment: car.getOutPositions(board)) {
            out.setPosition(MARGIN + static_cast<float>(segment.x) * cellSize.x,
                            MARGIN + static_cast<float>(segment.y) * cellSize.y);
            window.draw(out);
        }
    }
}
