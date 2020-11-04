
// Standard libraries
#include <iostream>
#include <sstream>
#include <string>
// SFML libraries
#include <SFML/Graphics.hpp>
using namespace sf;
//Local import
#include "player/Player.hpp"
#include "player/Bullet.hpp"
#include "game elements/Pickup.hpp"
#include "utils/Zombie_Arena.hpp"
#include "utils/TextureHolder.hpp"
#include "utils/Random.hpp"
#include "zombie/Zombie.hpp"

// Game in four states 
enum class GameState
{
    PAUSED, LEVEL_UP, GAME_OVER, PLAYING, INIT
};

void setTextConfig(sf::Text& text,const Font& font,
    const int& size, const Color& color, const Vector2i& position, const std::string& mess );

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
    Texture backgroundTexture = TextureHolder::getTexture("assets/graphics/background_sheet.png");
    
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
    Texture crosshairTexture = TextureHolder::getTexture("assets/graphics/crosshair.png");
    crosshairSprite.setTexture(crosshairTexture);
    crosshairSprite.setOrigin(25, 25);

    //Pickups 
    unique_ptr<Pickup> healthPickup(new Pickup(1));
    unique_ptr<Pickup> ammoPickup(new Pickup(2));

    // UX
    View hudView(FloatRect(0, 0, resolution.x, resolution.y));
    int score(0),
        hiscore(0),
        wave(0);

    Sprite GameOverSprite, ammoSprite;
    GameOverSprite.setTexture(TextureHolder::getTexture("assets/graphics/background.png"));
    GameOverSprite.setPosition(0, 0);

    ammoSprite.setTexture(TextureHolder::getTexture("assets/graphics/ammo_icon.png"));
    ammoSprite.setPosition(20, 980);

    Font font;
    font.loadFromFile("assets/fonts/zombiecontrol.ttf");

    Text pausedText,
        gameOverText,
        ammoText,
        scoreText,
        hiscoreText,
        zombieRemainingText,
        waveText;

    stringstream ss;
    ss <<"High Score :" << hiscore;

    setTextConfig(pausedText, font, 155, Color::White,
        Vector2i(400, 400), " Press Enter \nto continue");
    setTextConfig(gameOverText, font, 125, Color::Red,
        Vector2i(250,850), "Press Enter to play");
    setTextConfig(ammoText, font, 55, Color::White,
        Vector2i(200, 980), "");
    setTextConfig(scoreText, font, 55, Color::White,
        Vector2i(20, 0), "");
    setTextConfig(hiscoreText, font, 55, Color::White,
        Vector2i(1400, 0), ss.str());
    ss.clear(); ss.str(string());
    setTextConfig(zombieRemainingText, font, 55, Color::White,
        Vector2i(1500, 980), "zombies: 100");

    // Health bar
    RectangleShape healthBar;
    healthBar.setFillColor(Color::Red);
    healthBar.setPosition(450, 980);


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
                    else if (gameState == GameState::GAME_OVER) {
                        gameState = GameState::INIT;
                        wave = 0;
                        score = 0;
                        currentBullet = 0;
                        bulletInClip = 6;
                        bulletSpare = 24;
                    }
                       


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

        if (gameState == GameState::INIT || gameState == GameState::LEVEL_UP) {
                gameState = GameState::PLAYING;

            if (gameState == GameState::PLAYING) {
                wave++;
                arena.width = 1000*wave;
                arena.height = 1000*wave;
                arena.left = 0;
                arena.top = 0;
                bulletSpare += clipSize * wave;
                numZombies = 10 * wave;
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
                                    score+=10;
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
             
            healthBar.setSize(Vector2f(player.getHealth() * 3, 50));

            
                stringstream ammoSS;
                ammoSS << bulletInClip << " / " << bulletSpare;
                ammoText.setString(ammoSS.str());
                stringstream scoreSS;
                scoreSS << "Score: " << score;
                scoreText.setString(scoreSS.str());
                stringstream zombieSS;
                 zombieSS<< "zombies: " << numZombiesAlive;
                zombieRemainingText.setString(zombieSS.str());
                
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

            window.setView(hudView);
            window.draw(ammoSprite);
            window.draw(ammoText);
            window.draw(scoreText);
            window.draw(healthBar);
            window.draw(zombieRemainingText);
           
        }
        else if (gameState == GameState::GAME_OVER) {
            window.draw(GameOverSprite);
            window.draw(gameOverText);
        }
        window.display();
    }//While game is running

    return 0;
}


void setTextConfig(sf::Text& text, const Font& font, const int& size, const Color& color, const Vector2i& position, const string& mess)
{
    text.setFont(font);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition(position.x, position.y);
    text.setString(mess);
}
