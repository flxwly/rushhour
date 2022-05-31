#include "Game.h"

Game::Game(sf::Vector2i size, sf::Font &font) : window(sf::VideoMode(size.x, size.y), "Rush Hour"), gui(sf::Vector2f (size), font) {
    // Font
    this->font = font;

    // Game content
    this->cars = std::vector<Car>();
    this->board = std::vector<std::vector<Car *>>();

    std::cout << "Game created" << std::endl;
}

void Game::update() {
    if (!isRunning()) {
        return;
    }

    gui.update();

    if (gui.close.isPressed()) {
        window.close();
    } else if (gui.play.isPressed()) {
        if (gui.curLevel >= 0 && gui.curLevel < LEVEL_COUNT) {
            LevelHandler::loadLevel(&cars, &board, "original" + std::to_string(gui.curLevel));
        }
    } else if (gui.load.isPressed()) {

    } else if (gui.save.isPressed()) {

    } else if (gui.levelSelect.isPressed()) {
        gui.setShowState(1);
    }

    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (gui.getShowState() == 3) {
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i cell = sf::Vector2i((event.mouseButton.x - MARGIN_LEFT) * GRID_WIDTH / GAME_WIDTH,
                                                     (event.mouseButton.y - MARGIN_TOP) * GRID_HEIGHT / GAME_HEIGHT);

                    if (cell.x >= 0 && cell.x < GRID_WIDTH && cell.y >= 0 && cell.y < GRID_HEIGHT) {
                        if (board[cell.x][cell.y]) {
                            selectedCar = board[cell.x][cell.y];
                        } else {
                            selectedCar = nullptr;
                        }
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    if (selectedCar != nullptr) {
                        selectedCar->move({-1, 0}, board);
                    }
                }
                if (event.key.code == sf::Keyboard::Right) {
                    if (selectedCar != nullptr) {
                        selectedCar->move({1, 0}, board);
                    }
                }
                if (event.key.code == sf::Keyboard::Up) {
                    if (selectedCar != nullptr) {
                        selectedCar->move({0, -1}, board);
                    }
                }
                if (event.key.code == sf::Keyboard::Down) {
                    if (selectedCar != nullptr) {
                        selectedCar->move({0, 1}, board);
                    }
                }
            }

            bool gameOver = true;
            for (const auto &car: cars) {
                if (car.getOutDir() != sf::Vector2i(0, 0) && !car.getOccupiedPositions().empty()) {
                    gameOver = false;
                }
            }

            if (gameOver) {
                gui.setShowState(2);
            }
        }
    }
}

void Game::render() {
    if (!isRunning()) {
        return;
    }



    window.clear(sf::Color::White);
    window.draw(gui);

    sf::RectangleShape cell(sf::Vector2f(CELL_WIDTH, CELL_HEIGHT));
    for (int x = 0; x < board.size(); x += 1) {
        for (int y = 0; y < board.at(x).size(); y += 1) {
            cell.setPosition(MARGIN_LEFT + x * CELL_WIDTH, MARGIN_TOP + y * CELL_HEIGHT);
            cell.setFillColor(board[x][y] == nullptr ? sf::Color::White : board[x][y]->getColor());

            if (board[x][y] && board[x][y]->getOutDir() != sf::Vector2i(0, 0)) {
                cell.setFillColor(sf::Color(255, 0, 0));
            }

            window.draw(cell);
        }
    }

    if (selectedCar) {
        if (blink) {
            for (auto segment: selectedCar->getOccupiedPositions()) {
                cell.setPosition(MARGIN_LEFT + segment.x * CELL_WIDTH, MARGIN_TOP + segment.y * CELL_HEIGHT);
                cell.setFillColor(sf::Color(255, 255, 255, 100));
                window.draw(cell);
            }
        }

        if (selectedCar->getOutDir() != sf::Vector2i(0, 0)) {

            if (blink) {
                cell.setFillColor(sf::Color(255, 0, 0));
            } else {
                cell.setFillColor(sf::Color(255, 100, 100, 0));
            }

            if (selectedCar->getOutDir().x > 0) {
                cell.setPosition(MARGIN_LEFT + GAME_WIDTH, MARGIN_TOP);
                cell.setSize(sf::Vector2f(MARGIN_RIGHT, GAME_HEIGHT));
                window.draw(cell);
            } else if (selectedCar->getOutDir().x < 0) {
                cell.setPosition(0, MARGIN_TOP);
                cell.setSize(sf::Vector2f(MARGIN_LEFT, GAME_HEIGHT));
                window.draw(cell);
            }

            if (selectedCar->getOutDir().y > 0) {
                cell.setPosition(MARGIN_LEFT, MARGIN_TOP + GAME_HEIGHT);
                cell.setSize(sf::Vector2f(GAME_WIDTH, MARGIN_BOTTOM));
                window.draw(cell);
            } else if (selectedCar->getOutDir().y < 0) {
                cell.setPosition(MARGIN_LEFT, 0);
                cell.setSize(sf::Vector2f(GAME_WIDTH, MARGIN_TOP));
                window.draw(cell);
            }
        }
    }

    if (gui.curLevel == -1) {
        selectedCar = nullptr;
        sf::Text text("Du hast gewonnen!!!!", font, 50);
        text.setPosition(WINDOW_WIDTH / 2 - text.getLocalBounds().width / 2,
                         WINDOW_HEIGHT / 2 - text.getLocalBounds().height / 2);
        text.setOutlineThickness(4);

        if (blink) {
            text.setFillColor(sf::Color(255, 200, 50));
            text.setOutlineColor(sf::Color::White);
        } else {
            text.setFillColor(sf::Color::Red);
            text.setOutlineColor(sf::Color::Black);
        }

        window.draw(text);
    }

    if (blinker.getElapsedTime().asSeconds() > 0.5) {
        blink = !blink;
        blinker.restart();
    }

    window.display();
}
