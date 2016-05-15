#include "menuselectoptionelement.h"

void MenuSelectOptionElement::init(std::string identifier, int row, int col, std::string label) {
  this->identifier = identifier;
  this->row = row;
  this->col = col;
  this->label = label;
  active = false;
  shown = true;
  selectable = true;
}

MenuSelectOptionElement::~MenuSelectOptionElement() {

}

void MenuSelectOptionElement::setPosition(int row, int col) {
  this->row = row;
  this->col = col;
}

std::string MenuSelectOptionElement::getLabelText() const {
  return label;
}

std::string MenuSelectOptionElement::getIdentifier() const {
  return identifier;
}

unsigned int MenuSelectOptionElement::getId() const {
  return id;
}

bool MenuSelectOptionElement::activate() {
  active = true;
  return true;
}

void MenuSelectOptionElement::deactivate() {
  active = false;
}

bool MenuSelectOptionElement::isActive() const {
  return active;
}

std::string MenuSelectOptionElement::getLegendText() const {
  return "";
}

unsigned int MenuSelectOptionElement::getCol() const {
  return col;
}

unsigned int MenuSelectOptionElement::getRow() const {
  return row;
}

int MenuSelectOptionElement::cursorPosition() const {
  return -1;
}

void MenuSelectOptionElement::inputChar(int ch) {

}

void MenuSelectOptionElement::hide() {
  shown = false;
}

void MenuSelectOptionElement::show() {
  shown = true;
}

bool MenuSelectOptionElement::visible() const {
  return shown;
}

bool MenuSelectOptionElement::isSelectable() const {
  return selectable;
}

void MenuSelectOptionElement::setSelectable(bool selectable) {
  this->selectable = selectable;
}

void MenuSelectOptionElement::setId(unsigned int id) {
  this->id = id;
}