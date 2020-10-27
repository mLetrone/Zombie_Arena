
// Standard libraries
#include <iostream>
// SFML libraries
#include <SFML/Graphics.hpp>
using namespace sf;
//Local import
#include "Player.hpp"
#include "Bullet.hpp"
#include "Pickup.hpp"
#include "Zombie_Arena.hpp"

// Game in four states 
enum class GameState
{
    PAUSED, LEVEL_UP, GAME_OVER, PLAYING, INIT
};

int main()
{

    TextureHolder holder;
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
    Texture backgroundTexture = TextureHolder::getTexture("graphics/background_sheet.png");
    
    // Prepare for a horde of zombies
    int numZombies;
    int numZombiesAlive;
    std::vector<std::shared_ptr<Zombie>> zombies;


    // Bullet variables
    std::vector<Bullet> bullets;
    bullets.resize(100);
    int currentBullet = 0,
        bulletSpare = 24,
        bulletInClip = 6,
        clipSize = 6;
    float fireRate = 10;
    Time lastPressed;

    // Transform the mouse as a crosshair
    window.setMouseCursorVisible(false);
    Sprite crosshairSprite;
    Texture crosshairTexture = TextureHolder::getTexture("graphics/crosshair.png");
    crosshairSprite.setTexture(crosshairTexture);
    crosshairSprite.setOrigin(25, 25);
    unique_ptr<Pickup> healthPickup(new Pickup(1));
    unique_ptr<Pickup> ammoPickup(new Pickup(2));
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
                    // pause a game
                    if (gameState == GameState::PLAYING)
                        gameState = GameState::PAUSED;
                    // restart a paused game
                    else if (gameState == GameState::PAUSED) {
                        gameState = GameState::PLAYING;
                        clock.restart();
                    }
                    else if (gameState == GameState::GAME_OVER)
                        gameState = GameState::INIT;


                }

            }
            if (gameState == GameState::PLAYING) {
                if (event.key.code == Keyboard::R)
                {
                    if (bulletSpare >= clipSize) {
                        // Fully reload the clip
                        bulletInClip = clipSize;
                        bulletSpare -= clipSize;
                    }
                    else if (bulletSpare > 0) {
                        // few bullets left
                        bulletInClip = bulletSpare;
                        bulletSpare = 0;
                    }
                    else {}
                }
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            gameState = GameState::GAME_OVER;
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

            // fire bullet
            if (Mouse::isButtonPressed(Mouse::Left)) {
                if (bulletInClip > 0 &&
                    (gameTotalTime.asMilliseconds() - lastPressed.asMilliseconds() >= 1000 / fireRate)) {
                    bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y,
                        mouseWorldPosition.x, mouseWorldPosition.y);
                    currentBullet = (currentBullet <99)? currentBullet+1: 0;

                    lastPressed = gameTotalTime;

                    bulletInClip--;

                }
            }

        }

        if (gameState == GameState::INIT) {
                gameState = GameState::PLAYING;

            if (gameState == GameState::PLAYING) {
                arena.width = 1000;
                arena.height = 1000;
                arena.left = 0;
                arena.top = 0;

                numZombies = 10;
                try
                {
                    zombies.clear();
                    zombies.resize(numZombies);
                }
                catch (const std::exception&)
                {
                    cout << "OUCH " << endl;
                }
               
                zombies = createHorde(numZombies, arena);
                numZombiesAlive = numZombies;
                int tileSize = CreateBackground(background, arena);
                player.spawn(arena, resolution, tileSize);
                healthPickup->setArena(arena);
                ammoPickup->setArena(arena);

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

            // set crosshair position to mouse world location
            crosshairSprite.setPosition(mouseWorldPosition);

            player.update(dt.asSeconds(), mouseScreenPosition);

            Vector2f playerPosition(player.getCenter());

            mainView.setCenter(playerPosition);

            for (int i = 0; i < numZombies; i++) {
                if (zombies[i]->isAlive())
                    zombies[i]->update(dt.asSeconds(), playerPosition);
            }

            // update bullets that are shooted
            for (int i = 0; i < bullets.size(); i++) {
                if (bullets[i].isShooted())
                    bullets[i].update(dt.asSeconds());
            }

            // update pickups
            healthPickup->update(dt.asSeconds());
            ammoPickup->update(dt.asSeconds());
            for (int i=0; i < bullets.size(); i++)
            {
                    for (int j = 0; j < zombies.size(); j++) {
     
                        if (bullets[i].isShooted() && zombies[j]->isAlive()) {
                           
                            if (bullets[i].getPOsition().intersects(zombies[j]->getPosition()))
                            {
                                bullets[i].stop();
                          
                                if (zombies[j]->hit()) {
                                    // score+=10;
                                    numZombiesAlive--;
                                }
                                if (numZombiesAlive == 0) {
                                    gameState = GameState::INIT;
                                }

                            }


                        }
                    }
               
                
            }// end bullet/zombie collision

            // have zombie touched player
            for (int i = 0; i < numZombies; i++) {
                
                if (player.getPosition().intersects(zombies[i]->getPosition()) 
                    && (zombies[i]->isAlive())) {
                    if (player.hit(gameTotalTime, zombies[i])) {

                    }

                    else if (player.getHealth() <= 0) {
                        gameState = GameState::GAME_OVER;
                        cout << "player is dead" << endl;
                    }
                }
            }

            if (player.getPosition().intersects(healthPickup->getPosition()) &&
                healthPickup->isSpawned())
                player.increaseHealthLevel(healthPickup->gotIt());

            if (player.getPosition().intersects(ammoPickup->getPosition()) &&
                ammoPickup->isSpawned())
                bulletSpare += ammoPickup->gotIt();
             
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
            //draw zombies
            for (int i = 0; i < numZombies; i++)
                window.draw(zombies[i]->getSprite());

            // draw bullets
            for (int i = 0; i < 100; i++) {
                if (bullets[i].isShooted())
                    window.draw(bullets[i].getShape());
            }

            // draw pickups
            if (healthPickup->isSpawned())
                window.draw(healthPickup->getSprite());

            if (ammoPickup->isSpawned())
                window.draw(ammoPickup->getSprite());

            window.draw(crosshairSprite);
            window.draw(player.getSprite());

            window.display();
        }

    }//While game is running

    return 0;
}
