#ifndef COLUMN_H
#define COLUMN_H

#include "Ui.h"
#include "Cell.h"

// Vertical container.

class Column : public Ui {

  static std::vector<std::shared_ptr<Ui>> toUiList(const std::vector<std::shared_ptr<Cell>> & cells) {
    std::vector<std::shared_ptr<Ui>> uiList;
    for (auto cell : cells) uiList.push_back(cell);
    return uiList;
  }
public:

  explicit Column(std::vector<std::shared_ptr<Cell>>&& children) : Ui(toUiList(children)) {}

  void layout() override;

  static std::shared_ptr<Column> make(std::vector<std::shared_ptr<Cell>>&& children) {
    return std::make_shared<Column>(std::move(children));
  }
};

#endif //COLUMN_H
