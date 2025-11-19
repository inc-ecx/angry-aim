#ifndef CELL_H
#define CELL_H

#include "Ui.h"

enum class CellType {
  ABSOLUTE, RELATIVE, WRAP
};

// A cell is the child of a column or a row.

class Cell : public Ui {
public:
  CellType type;
  int size;

  Cell(CellType type, int size, std::shared_ptr<Ui> child) :
    Ui({child}),
    type(type),
    size(size) {}

  Cell(CellType type, int size) :
    type(type),
    size(size) {}

  ~Cell() override = default;

  static std::shared_ptr<Cell> abs(std::shared_ptr<Ui> child, int size) {
    return std::make_shared<Cell>(CellType::ABSOLUTE, size, child);
  }

  static std::shared_ptr<Cell> rel(std::shared_ptr<Ui> child, int size = 1) {
    return std::make_shared<Cell>(CellType::RELATIVE, size, child);
  }

  static std::shared_ptr<Cell> abs(int size) {
    return std::make_shared<Cell>(CellType::ABSOLUTE, size);
  }

  static std::shared_ptr<Cell> rel(int size = 1) {
    return std::make_shared<Cell>(CellType::RELATIVE, size);
  }

  // this uses the biggest provided absolute size by any child in the hierarchy. (provided using Ui::queryWrap)
  // this allows for nested rows or cols to effectively have their own absolute size, without hardcoding it.
  // example: Row::make(
  //   ... some cells
  //   Cell::wrap(
  //     ... some hierarchy
  //       Row::make({
  //         Cell::abs(Button::make("Hi1"), 100),
  //         Cell::abs(Button::make("Hi2"), 100)
  //       }),
  //     ... end of some hierarchy
  //   ), // equivalent of Cell::abs(..., 200)
  //   ...
  // )
  static std::shared_ptr<Cell> wrap(std::shared_ptr<Ui> child) {
    return std::make_shared<Cell>(CellType::WRAP, 0, child);
  }
};


#endif //CELL_H
