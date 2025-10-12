#ifndef ROW_H
#define ROW_H

#include "Ui.h"
#include "Cell.h"

// Horizontal container.

class Row : public Ui {

  static std::vector<std::shared_ptr<Ui>> toUiList(const std::vector<std::shared_ptr<Cell>> & cells) {
    std::vector<std::shared_ptr<Ui>> uiList;
    for (auto cell : cells) uiList.push_back(cell);
    return uiList;
  }
public:

  explicit Row(std::vector<std::shared_ptr<Cell>>&& children) : Ui(toUiList(children)) {}

  void layout() override;

  static std::shared_ptr<Row> make(std::vector<std::shared_ptr<Cell>>&& children) {
    return std::make_shared<Row>(std::move(children));
  }
};

#endif //ROW_H
