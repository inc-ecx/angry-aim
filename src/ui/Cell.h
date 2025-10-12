#ifndef CELL_H
#define CELL_H

#include "Ui.h"

enum class CellType {
  ABSOLUTE, RELATIVE
};

// A cell is the child of a column or a row.

class Cell : public Ui {

public:
  CellType type;
  int size;

  Cell(CellType type, int size, std::shared_ptr<Ui> child)
    : Ui({child}),
      type(type),
      size(size)
  {
  }

  ~Cell() override = default;

  static std::shared_ptr<Cell> abs(std::shared_ptr<Ui> child, int size) {
    return std::make_shared<Cell>(CellType::ABSOLUTE, size, child);
  }

  static std::shared_ptr<Cell> rel(std::shared_ptr<Ui> child, int size = 1) {
    return std::make_shared<Cell>(CellType::RELATIVE, size, child);
  }

};



#endif //CELL_H
