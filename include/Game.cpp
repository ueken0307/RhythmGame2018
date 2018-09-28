#include "Game.hpp"

void Game::init() {
  printf("selected -> %ls\n",m_data->folderName.c_str());
}

void Game::update() {
  if (Input::KeyRight.clicked) {
    printf("Next scene is Result\n");
    changeScene(L"Result");
  }
}

void Game::draw() const {

}

void Game::drawFadeIn(double t) const {
  draw();
  Transformer2D tr(Graphics2D::GetTransform().inverse());
  Window::ClientRect().draw(ColorF(0.0).setAlpha(1.0 - t));
}

void Game::drawFadeOut(double t) const {
  draw();
  Transformer2D tr(Graphics2D::GetTransform().inverse());
  Window::ClientRect().draw(ColorF(0.0).setAlpha(t));
}