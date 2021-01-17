#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>
#include <sstream>

using namespace sf;
using namespace std;

class Player {
  public:
    float x, y, w, h, speed, friction;
    sf::Vector2f velocity;
    String File;
    Image image;
    Texture texture;
    Sprite sprite;

    RectangleShape bullet;
    bool is_shot;
    float speed_shot;
    int score;

    Player(String F, float X, float Y, float W, float H)
        : bullet(Vector2f(5, 20)) {
        File = F;
        w = W;
        h = H;
        speed = 0;
        image.loadFromFile(File);
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        sprite.setPosition(x = X, y = Y);
        sprite.setOrigin(w / 2, h / 2);

        friction = 0.96;
        velocity.x = 0;
        velocity.y = 0;

        is_shot = false;
        speed_shot = 500;
        bullet.setFillColor(Color::Green);

        score = 0;
    }
};

int main() {
    srand(std::time(0));
    int speed = 300;
    RenderWindow window(sf::VideoMode(600, 680), "Space Adventures", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);

    RectangleShape karta(Vector2f(680, 840));
    RectangleShape bort_1(Vector2f(30, 840));
    RectangleShape bort_2(Vector2f(30, 840));
    RectangleShape bort_3(Vector2f(80, 80));

    karta.setFillColor(Color::Black);
    karta.setPosition(0, 0);

    bort_1.setFillColor(Color::Red);
    bort_1.setPosition(0, 0);
    bort_2.setFillColor(Color::Red);
    bort_2.setPosition(570, 0);
    bort_3.setFillColor(Color::Red);
    bort_3.setPosition(300, 0);

    sf::Font font;
    font.loadFromFile("./fonts/DejaVuSansMono.ttf");

    sf::Text score_text;
    score_text.setFont(font);
    score_text.setString("0");
    score_text.setCharacterSize(30);
    score_text.setStyle(sf::Text::Bold);
    score_text.setColor(sf::Color::Blue);
    score_text.setPosition(35, 10);

    sf::Text game_over_text;
    game_over_text.setFont(font);
    game_over_text.setString("GAME OVER");
    game_over_text.setCharacterSize(50);
    game_over_text.setStyle(sf::Text::Bold);
    game_over_text.setColor(sf::Color::White);
    game_over_text.setPosition(600 / 4.5, 680 / 2);

    bool is_game_over = false;

    Player p("Airplane.png", 300, 540, 130, 90);

    sf::Clock clock;
    


    while (window.isOpen()) {
        sf::Time dt = clock.restart();

        Event event;
        while (window.pollEvent(event)) {

            if (event.type == Event::Closed)
                window.close();
        }

        if (!is_game_over) {

            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                p.velocity.x = -speed * dt.asSeconds();
            }

            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                p.velocity.x = speed * dt.asSeconds();
            }

            p.sprite.move(p.velocity);
            p.velocity *= p.friction;

            if (p.sprite.getPosition().x < 95)
                p.sprite.setPosition(95, p.sprite.getPosition().y);
            else if (p.sprite.getPosition().x > 505)
                p.sprite.setPosition(505, p.sprite.getPosition().y);

            bort_3.move(0, 600 * dt.asSeconds());

            if (bort_3.getPosition().y > 680) {
                sf::Vector2f pos(std::rand() % 600, std::rand() % 100 - 150);
                bort_3.setPosition(pos);
            }
            if (bort_3.getPosition().x < 95)
                bort_3.setPosition(std::rand() % 600, std::rand() % 100 - 150);
            else if (bort_3.getPosition().x > 505)
                bort_3.setPosition(std::rand() % 600, std::rand() % 100 - 150);

            if (p.sprite.getGlobalBounds().intersects(
                    bort_3.getGlobalBounds())) {
                is_game_over = true;
            }

            if (p.is_shot) {
                p.bullet.move(0, -p.speed_shot * dt.asSeconds());

                if (p.bullet.getGlobalBounds().intersects(
                        bort_3.getGlobalBounds())) {
                    bort_3.setPosition(std::rand() % 600,
                                       std::rand() % 100 - 150);
                    p.is_shot = false;
                    p.score++;
                    std::ostringstream oss;
                    oss << p.score;
                    score_text.setString(oss.str());
                }

                if (p.bullet.getPosition().y < -20)
                    p.is_shot = false;
            }

            if (Keyboard::isKeyPressed(Keyboard::Space) && !p.is_shot) {
                p.is_shot = true;
                p.bullet.setPosition(p.sprite.getGlobalBounds().left +
                                         p.sprite.getGlobalBounds().width / 2.0,
                                     p.sprite.getGlobalBounds().top - 2.0);
            }

        } // if (!game_over)
        else {
            if (Keyboard::isKeyPressed(sf::Keyboard::Return)) {
                is_game_over = false;
                p.is_shot = false;
                bort_3.setPosition(std::rand() % 600, std::rand() % 100 - 150);
                p.score = 0;

                std::ostringstream oss;
                oss << p.score;
                score_text.setString(oss.str());
            }
        }

        window.clear();

        if (!is_game_over) {
            window.draw(karta);
            window.draw(bort_1);
            window.draw(bort_2);
            window.draw(bort_3);
            if (p.is_shot)
                window.draw(p.bullet);
            window.draw(p.sprite);
        } else {
            window.draw(game_over_text);
        }

        window.draw(score_text);

        window.display();
    }
}
