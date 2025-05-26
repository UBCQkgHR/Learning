#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <queue>
#include <unordered_set>
#include <vector>

class Level {
public:
  sf::RectangleShape shape;
  Level() {
    shape.setSize(sf::Vector2f(39.f, 29.f));
  } // virtual void update() {};
  virtual ~Level();
  virtual void draw(sf::RenderWindow &window);
};

Level::~Level() {};
void Level::draw(sf::RenderWindow &window) { window.draw(shape); };

class Goal : public Level {
public:
  Goal(int i, int u) {
    shape.setSize(sf::Vector2f(39.f, 29.f));
    shape.setPosition(i * 40, u * 30);
    shape.setFillColor(sf::Color::Yellow);
  }
  ~Goal() {};
};

class Block : public Level {
public:
  Block(int i, int u) {
    shape.setSize(sf::Vector2f(39.f, 29.f));
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(sf::Vector2f(i * 40.f, u * 30.f));
  };
};

class Player : public Level {
public:
  Player(int i, int u) {
    shape.setSize(sf::Vector2f(39.f, 29.f));
    shape.setFillColor(sf::Color::Blue);
    shape.setPosition(i * 39.f, u * 29.f);
  };
};

struct Vec2 {
  int x, y;
  bool operator==(const Vec2 &o) const { return x == o.x && y == o.y; }
};
namespace std {
template <> struct hash<Vec2> {
  size_t operator()(const Vec2 &v) const {
    size_t h1 = std::hash<int>()(v.x);
    size_t h2 = std::hash<int>()(v.y);
    return h1 ^ (h2 << 1);
  }
};
} // namespace std

float heuristic(const Vec2 &a, const Vec2 &b) {
  return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

struct Node {
  Vec2 pos;
  float g, h;
  Node *parent;
  float f() const { return g + h; }
};

struct Compare {
  bool operator()(Node *a, Node *b) { return a->f() > b->f(); }
};
std::vector<Vec2> AStar(const std::vector<std::vector<int>> &grid, Vec2 start,
                        Vec2 goal) {
  std::priority_queue<Node *, std::vector<Node *>, Compare> open;
  std::unordered_set<Vec2> closed;
  Node *startNode = new Node{start, 0, heuristic(start, goal), nullptr};
  open.push(startNode);
  const Vec2 dirs[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

  while (!open.empty()) {
    Node *current = open.top();
    open.pop();
    if (current->pos == goal) {
      std::vector<Vec2> path;
      while (current) {
        path.push_back(current->pos);
        current = current->parent;
      }
      std::reverse(path.begin(), path.end());
      return path;
    }
    closed.insert(current->pos);

    for (const Vec2 &d : dirs) {
      Vec2 neighbor = {current->pos.x + d.x, current->pos.y + d.y};

      if (neighbor.x < 0 || neighbor.y < 0 || neighbor.x >= grid[0].size() ||
          neighbor.y >= grid.size()) {
        continue;
      }
      if (grid[neighbor.y][neighbor.x] == 1 || closed.count(neighbor)) {
        continue;
      }

      float tentative_g = current->g + 1;

      Node *neighborNode =
          new Node{neighbor, tentative_g, heuristic(neighbor, goal), current};
      open.push(neighborNode);
    }
  }
  return {};
}

//////////////////////////////////////////////////////////////////////////
int main() {
  std::vector<std::vector<int>> grid = {
      {0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
      {0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1},
      {1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1},
      {0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0},
      {0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0},
      {0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
      {0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0},
      {0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0},
      {0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1},
      {0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1},
      {0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1},
      {1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1},
      {0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1},
      {1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0},
      {1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
      {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0},
      {0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0},
      {0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0},
      {1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0}};

  Vec2 start{0, 0}, goal{19, 19};
  auto path = AStar(grid, start, goal);
  std::cout << "path size: " << path.size() << "\n";
  Level level;
  Goal goalplayer(19, 19);
  Player player(0, 0);
  std::vector<std::unique_ptr<Level>> levels;
  for (int i = 0; i < 20; ++i) {
    for (int u = 0; u < 20; ++u) {
      if (grid[u][i] == 1) {
        levels.push_back(std::make_unique<Block>(i, u));
      } else {
        continue;
      }
    }
  }

  sf::RenderWindow window(sf::VideoMode(800, 600), "Astar");
  window.setFramerateLimit(10);
  sf::RectangleShape rect;

  sf::Vector2i mousePos;
  sf::Vector2f worldPos;
  rect.setSize(sf::Vector2f(40, 30));
  rect.setFillColor(sf::Color::Red);
  std::cout << "("
               ","
               ")";
  for (auto &p : path) {
    std::cout << "(" << p.x << "," << p.y << ")" << std::endl;
    ;
  };
  sf::Clock *clock = new sf::Clock;
  float dt;
  const sf::Time moveDelay = sf::microseconds(900);
  int x, y;
  int mouseX, mouseY;
  while (window.isOpen()) {
    sf::Event event;
    dt = clock->restart().asMilliseconds();
    static int step = 0;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {

        window.close();
      }
      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Middle) {
          mouseX = event.mouseButton.x / 40;
          mouseY = event.mouseButton.y / 30;

          if (grid[mouseY][mouseX] != 1) {

            goal = {mouseX, mouseY};
            goalplayer.shape.setPosition(mouseX * 40, mouseY * 30);
            start = {(static_cast<int>(player.shape.getPosition().x)) / 40,
                     (static_cast<int>(player.shape.getPosition().y)) / 30};

            path = AStar(grid, start, goal);
            step = 0;
          };
        }
        if (event.mouseButton.button == sf::Mouse::Left) {
          mouseX = event.mouseButton.x / 40;
          mouseY = event.mouseButton.y / 30;

          if (grid[mouseY][mouseX] == 0) {

            grid[mouseY][mouseX] = 1;

            levels.push_back(std::make_unique<Block>(mouseX, mouseY));

            start = {(static_cast<int>(player.shape.getPosition().x)) / 40,
                     (static_cast<int>(player.shape.getPosition().y)) / 30};

            path = AStar(grid, start, goal);
            step = 0;
          }
          std::cout << " Left press" << event.mouseButton.x << ", "
                    << event.mouseButton.y << std::endl;
        }
        if (event.mouseButton.button == sf::Mouse::Right) {
          mouseX = event.mouseButton.x / 40;
          mouseY = event.mouseButton.y / 30;

          mousePos = sf::Mouse::getPosition(window);
          worldPos = window.mapPixelToCoords(mousePos);
          if (grid[mouseY][mouseX] == 1) {
            grid[mouseY][mouseX] = 0;
            for (auto it = levels.begin(); it != levels.end(); ++it) {
              if ((*it)->shape.getGlobalBounds().contains(worldPos)) {
                levels.erase(it);
                break;
              }
            }

            start = {(static_cast<int>(player.shape.getPosition().x)) / 40,
                     (static_cast<int>(player.shape.getPosition().y)) / 30};

            path = AStar(grid, start, goal);
            step = 0;
            std::cout << "MouseX= " << mouseX << "MouseY= " << mouseY
                      << std::endl;
          }
        }
      }
    }

    window.clear();
    for (auto &e : levels) {
      e->draw(window);
    }
    //  if (clock->getElapsedTime() >= moveDelay) {
    std::cout << "dt = " << dt << "--" << "step = " << step << "--"
              << std::endl;
    if (!path.empty()) {
      x = path[step].x;
      y = path[step].y;
    }

    sf::Vector2f posit = player.shape.getPosition();
    std::cout << "x=" << posit.x << "y=" << posit.y << std::endl;
    if (step < path.size()) {
      ++step;
    }

    if (step >= path.size()) {
      step = 0;
      start = {0, 0};
      path = AStar(grid, start, goal);
    }
    player.shape.setPosition(x * 40.f, y * 30.f);
    player.draw(window);
    window.display();
    goalplayer.draw(window);
    clock->restart();
    //}
  }
  return 0;
}
