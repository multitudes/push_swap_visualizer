#include "gui.h"
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <random>

#include <imgui-SFML.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

std::list<int> Gui::_generateValues(const int size) {
  if (size <= 0) {
    return {};
  }

  std::vector<int> values;
  for (unsigned int v = 1; v <= size; ++v) {
    values.push_back(v);
  }
  std::shuffle(values.begin(), values.end(),
               std::mt19937{std::random_device{}()});
  std::list<int> shuffledList;
  std::move(values.begin(), values.end(), back_inserter(shuffledList));
  return shuffledList;
}

std::vector<sf::RectangleShape> Gui::_updateBars() {
  sf::RectangleShape shape(sf::Vector2f(100, 50));
  shape.setFillColor(sf::Color::Green);
  return std::vector<sf::RectangleShape>{shape};
}

void Gui::_updateControls() {
  ImGui::Begin("Controls");
  int speed{1};
  ImGui::SliderInt("Speed", &speed, 1, 60, "%i/s");
  ImGui::Button("Run");
  ImGui::SameLine();
  ImGui::Button("Pause");
  ImGui::SameLine();
  ImGui::Button("Reset");
  ImGui::End();

  ImGui::Begin("Values");
  ImGui::Text("Values to generate");
  ImGui::InputInt("Count", &this->generateNumberSize);

  if (ImGui::Button("Shuffle")) {
    std::list<int> valueInts = this->_generateValues(this->generateNumberSize);
    std::string values;
    for (const int value : valueInts) {
      values += std::to_string(value) + ' ';
    }
    this->numbers = values;
  }

  ImGui::Text("Space separated values");
  ImGui::InputTextMultiline("Values", &this->numbers);

  ImGui::Text("push_swap file path");
  ImGui::InputText("", &this->pushswap.path);
  if (ImGui::Button("Compute")) {
      this->pushswap.run(this->numbers);
  }

  ImGui::End();

  ImGui::Begin("Commands");
  for (const auto &cmd : this->pushswap.commands) {
    ImGui::Text(cmd.c_str());
  }
  ImGui::End();
}

Gui::Gui()
    : _window{sf::VideoMode::getDesktopMode(), "Push Swap Visualizer"},
      generateNumberSize{0} {
  _window.setFramerateLimit(60);
  ImGui::SFML::Init(_window);
}

void Gui::loop() {
  sf::Clock deltaClock;
  while (_window.isOpen()) {
    sf::Event event;
    while (_window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);

      if (event.type == sf::Event::Closed) {
        _window.close();
      }
    }

    const std::vector<sf::RectangleShape> shapes = this->_updateBars();
    ImGui::SFML::Update(_window, deltaClock.restart());

    this->_updateControls();

    _window.clear();

    for (const auto &shape : shapes) {
      _window.draw(shape);
    }

    ImGui::SFML::Render(_window);
    _window.display();
  }
}

Gui::~Gui() { ImGui::SFML::Shutdown(); }
