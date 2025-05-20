#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <iostream>
#include <memory>
#include <vector>

class Level {

public:
  sf::RectangleShape shape;
  //  Level();
  // virtual void update() {};
  virtual ~Level();
  virtual void draw(sf::RenderWindow &window);
};
Level::~Level() {};
void Level::draw(sf::RenderWindow &window) { window.draw(shape); };

class Block : public Level {

public:
  Block(int i) {
    shape.setSize(sf::Vector2f(49.f, 49.f));
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(sf::Vector2f(i * 50.f, 50.f));
  };
};

//////////////////////////////////////////////////////////////////////////
int main() {
  std::vector<std::vector<int>> grid = {{0, 0, 0, 0, 0},
                                        {1, 1, 0, 1, 0},
                                        {0, 0, 0, 1, 0},
                                        {0, 1, 1, 1, 0},
                                        {0, 0, 0, 0, 0}};

  Level level;
  std::vector<std::unique_ptr<Level>> levels;
  for (int i = 0; i < 5; ++i) {
    levels.push_back(std::make_unique<Block>(i));
  }

  sf::RenderWindow window(sf::VideoMode(800, 600), "Astar");

  sf::RectangleShape rect;
  rect.setSize(sf::Vector2f(50, 50));
  rect.setFillColor(sf::Color::Red);
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    for (auto &e : levels) {
      e->draw(window);
    }
    // window.draw(rect);

    window.display();
  };

  return 0;
}
