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

  Cell(CellType type, int size)
    : type(type),
      size(size) {
  }

  ~Cell() override = default;

  static Cell abs(std::shared_ptr<Ui> child, int size) {
    return Cell(CellType::ABSOLUTE, size);
  }

  static Cell rel(std::shared_ptr<Ui> child, int size = 1) {
    return Cell(CellType::RELATIVE, size);
  }

};



#endif //CELL_H
