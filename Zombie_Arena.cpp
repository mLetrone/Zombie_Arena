
// Standard libraries
#include <iostream>
// SFML libraries
#include <SFML/Graphics.hpp>
using namespace sf;
//Local import
#include "Player.hpp"
#include "Zombie_Arena.hpp"

// Game in four states 
enum class GameState
{
    PAUSED, LEVEL_UP, GAME_OVER, PLAYING
};

int main()
{
    // At the beginning, start with GAME_OVER state
    GameState gameState = GameState::GAME_OVER;

    // Get computer resolution
    // and create a dynamic SFML window
    Vector2f resolution;
    resolution.x = VideoMode::getDesktopMode().width;
    resolution.y = VideoMode::getDesktopMode().height;

    RenderWindow window(VideoMode(resolution.x, resolution.y),
        "Zombie Arena", Style::Fullscreen);

    // Create a an SFML View for the main action
    View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));
    Player player;

    // Track elapsing time
    Clock clock;
    // how long the player have played so far (PLAYING state)
    Time gameTotalTime;

    // track mouse coordinates
    Vector2i mouseScreenPosition;
    Vector2f mouseWorldPosition;

    IntRect arena;

    //create the background
    VertexArray background;
    Texture backgroundTexture;
    backgroundTexture.loadFromFile("graphics/background_sheet.png");

    // init random seed 
    InitRandom();

    while (window.isOpen())
    {

        /*
        *********************
            Handle input
        *********************
        */

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Return) {

                    if (gameState == GameState::PLAYING)
                        gameState = GameState::PAUSED;

                    else if (gameState == GameState::PAUSED) {
                        gameState = GameState::PLAYING;
                        clock.restart();
                    }

                    else if (gameState == GameState::GAME_OVER)
                        gameState = GameState::LEVEL_UP;


                }

            }
            if (gameState == GameState::PLAYING) {

            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }

        // Handle WASD  while playing for player's movements 
      if (gameState == GameState::PLAYING) {
            if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::Q))
                player.moveLeft();
            else
                player.stopLeft();

            if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
                player.moveRight();
            else
                player.stopRight();

            if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Z))
                player.moveUp();
            else
                player.stopUp();

            if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))
                player.moveDown();
            else
                player.stopDown();

        }

        if (gameState == GameState::LEVEL_UP) {
            // Handle all keyboard::NumX, 1 to 9 using their value
            if (event.key.code >= 27 && event.key.code <= 35)
                gameState = GameState::PLAYING;

            if (gameState == GameState::PLAYING) {
                arena.width = 500;
                arena.height = 500;
                arena.left = 0;
                arena.top = 0;

                int tileSize = CreateBackground(background, arena);
                player.spawn(arena, resolution, tileSize);

                clock.restart();
            }

        }

        /*
        ***********************
            Update the frame
        ***********************
        */
        if (gameState == GameState::PLAYING) {

            Time dt = clock.restart();
            gameTotalTime += dt;

            mouseScreenPosition = Mouse::getPosition();
            // Convert mouse position to world coordinates of mainView
            mouseWorldPosition = window.mapPixelToCoords(mouseScreenPosition, mainView);

            player.update(dt.asSeconds(), mouseScreenPosition);

            Vector2f playerPosition(player.getCenter());

            mainView.setCenter(playerPosition);
        }

        /*
        **********************
            Draw the scene
        **********************
        */

        if (gameState == GameState::PLAYING) {
            window.clear();

            //set the mainView to be displayed in the window
            // so draw everything related to it
            window.setView(mainView);
            window.draw(background, &backgroundTexture);
            window.draw(player.getSprite());

            window.display();
        }

    }//While game is running

    return 0;
}
