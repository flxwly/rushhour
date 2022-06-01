#include "Game.h"

Game::Game(sf::Vector2i size, sf::Font &font) : window(sf::VideoMode(size.x, size.y), "Rush Hour") {
    // Buttons
    this->font = font;
    buttons.emplace("prev",
                    Button(sf::Vector2f(20, size.y - 40), sf::Vector2f(40, 20), sf::Color::Green, sf::Color::Red,
                           sf::Color::Blue, sf::Color::White, "<", font));
    buttons.emplace("next", Button(sf::Vector2f(size.x - 60, size.y - 40), sf::Vector2f(40, 20), sf::Color::Green,
                                   sf::Color::Red, sf::Color::Blue, sf::Color::White, ">", font));
    buttons.emplace("restart",
                    Button(sf::Vector2f(size.x / 2 - 60, size.y - 40), sf::Vector2f(120, 20), sf::Color::Green,
                           sf::Color::Red, sf::Color::Blue, sf::Color::White, "nochmal", font));

    // Game content
    this->cars = std::vector<Car>();
    this->board = std::vector<std::vector<Car *>>();


    std::cout << "Game created" << std::endl;
}

void Game::update() {

    // Für jeden Button soll geprüft werden,
    // ob sich die Maus über diesem befindet.
    for (auto btn: buttons) {
        btn.second.isHover(window);
    }

    // sf::Event ist eine Klasse, die Informationen über ein Event enthält.
    // window.pollEvent() liest das nächste Event aus und speichert es
    // in der übergenen Variable. Solange ein Event vorhanden ist, liefert
    // window.pollEvent() wahr zurück. Es wird für jeden Aufruf von
    // window.pollEvent() immer nur ein Event in der Variable gespeichert.
    // Wenn es mehrere Events gibt, muss window.pollEvent() mehrmals aufgerufen
    // werden. Im Prinzip solange, bis die Methode falsch zurück liefert.
    sf::Event event;
    while (window.pollEvent(event)) {

        // Wenn das Event vom Typ sf::Event::Closed ist, dann hat
        // der Benutzer auf das rote X geklickt und das Fenster soll geschlossen werden
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Für jeden Button soll geprüft werden, ob dieser gedrückt wurde
        // und dementsprechend gehandelt werden.
        for (auto btn: buttons) {
            if (btn.second.isPressed(window, event)) {
                selectedCar = nullptr;

                if (btn.first == "prev") {
                    currentLevel = std::max(0, currentLevel - 1);
                } else if (btn.first == "next") {
                    currentLevel++;
                }
                if (!LevelHandler::loadLevel(&cars, &board, "original" + std::to_string(currentLevel))) {
                    currentLevel = 0;
                };
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
            sf::Vector2i cell = sf::Vector2i((event.mouseButton.x - MARGIN) * board.size() / GAME_WIDTH,
                                             (event.mouseButton.y - MARGIN) * board[0].size() / GAME_HEIGHT);

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

        if (event.type == sf::Event::KeyPressed && selectedCar) {
            switch (event.key.code) {
                case sf::Keyboard::Left:
                    selectedCar->move({-1, 0}, board);
                    break;
                case sf::Keyboard::Right:
                    selectedCar->move({1, 0}, board);
                    break;
                case sf::Keyboard::Up:
                    selectedCar->move({0, -1}, board);
                    break;
                case sf::Keyboard::Down:
                    selectedCar->move({0, 1}, board);
                    break;
            }
        }

        bool gameOver = true;
        for (const auto &car: cars) {
            if (car.getOutDir() != sf::Vector2i(0, 0) && !car.getOccupiedPositions().empty()) {
                gameOver = false;
            }
        }

        if (gameOver) {
            selectedCar = nullptr;
            currentLevel++;
            if (!LevelHandler::loadLevel(&cars, &board, "original" + std::to_string(currentLevel))) {
                currentLevel = 0;
            };
        }

    }
}

void Game::render() {

    window.clear(sf::Color::White);

    renderBackground();
    renderCars();
    renderButtons();

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
    sf::Vector2f cellSize = sf::Vector2f(GAME_WIDTH / board.size(), GAME_HEIGHT / board[0].size());


    sf::RectangleShape cell(cellSize);
    sf::RectangleShape importantCell(cellSize / 2.0f);
    importantCell.setOutlineThickness(2);
    importantCell.setOutlineColor(sf::Color::Black);
    for (int x = 0; x < board.size(); x += 1) {
        for (int y = 0; y < board.at(x).size(); y += 1) {
            cell.setPosition(MARGIN + x * cellSize.x, MARGIN + y * cellSize.y);
            cell.setFillColor(board[x][y] == nullptr ? sf::Color::Transparent : board[x][y]->getColor());

            if (board[x][y] && board[x][y]->getOutDir() != sf::Vector2i(0, 0)) {
                importantCell.setPosition(MARGIN + x * cellSize.x + cellSize.x / 4,
                                          MARGIN + y * cellSize.y + cellSize.y / 4);
                importantCell.setFillColor(board[x][y]->getColor());
                window.draw(importantCell);
            }

            window.draw(cell);
        }
    }

    if (selectedCar) {
        for (auto segment: selectedCar->getOccupiedPositions()) {
            if (blink) {
                cell.setFillColor(sf::Color(255, 255, 255, 100));
            } else {
                cell.setFillColor(selectedCar->getColor());
            }

            cell.setPosition(MARGIN + segment.x * cellSize.x, MARGIN + segment.y * cellSize.y);
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
    sf::Vector2f cellSize(GAME_WIDTH / board.size(), GAME_HEIGHT / board[0].size());

    // Vertikale Linien
    for (int i = 0; i < board.size() + 1; ++i) {
        sf::Vertex line[] =
                {
                        sf::Vertex(sf::Vector2f(i * cellSize.x + MARGIN, MARGIN), sf::Color::Black),
                        sf::Vertex(sf::Vector2f(i * cellSize.x + MARGIN, GAME_HEIGHT + MARGIN), sf::Color::Black)
                };
        window.draw(line, 2, sf::Lines);
    }

    // Horizontale Linien
    for (int i = 0; i < board[0].size() + 1; ++i) {
        sf::Vertex line[] =
                {
                        sf::Vertex(sf::Vector2f(MARGIN, i * cellSize.y + MARGIN), sf::Color::Black),
                        sf::Vertex(sf::Vector2f(GAME_WIDTH + MARGIN, i * cellSize.y + MARGIN), sf::Color::Black)
                };
        window.draw(line, 2, sf::Lines);
    }

    // Ausgang einzeichnen
    sf::RectangleShape out(cellSize);
    out.setFillColor(sf::Color::White);
    for (auto car: cars) {
        for (auto segment: car.getOutPositions(board)) {
            out.setPosition(MARGIN + segment.x * cellSize.x, MARGIN + segment.y * cellSize.y);
            window.draw(out);
        }
    }
}
