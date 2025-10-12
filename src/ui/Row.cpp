#include "Row.h"

void Row::layout() {
  const int total = width;

  // find total absolute and total relative space, also find last relative cell
  int totalAbsolute = 0;
  int totalRelative = 0;
  const Cell *lastRelative = nullptr;
  for (auto &child: children) {
    const auto pCell = dynamic_cast<Cell *>(child.get());
    if (pCell->type == CellType::ABSOLUTE) {
      totalAbsolute += pCell->size;
    } else {
      totalRelative += pCell->size;
      lastRelative = pCell;
    }
  }

  // get available spacing for relative cells to divide
  int available = total - totalAbsolute;
  if (available < 0) {
    available = 0;
  }

  // calculate bounds for each child
  int off = 0;
  int taken = 0;
  double remainder = 0;
  for (auto &child: children) {
    const auto pCell = dynamic_cast<Cell *>(child.get());

    // calculate size as a decimal
    double dSize = remainder;
    if (pCell->type == CellType::ABSOLUTE) {
      dSize += pCell->size;
    } else {
      dSize += static_cast<double>(pCell->size) / static_cast<double>(totalRelative) * available;
    }

    // calculate integer size
    int size = static_cast<int>(dSize);
    if (pCell == lastRelative) size = available - taken; // last relative block gets pixels left

    // update relative size which was taken
    if (pCell->type == CellType::RELATIVE) taken += size;

    // layout cell
    pCell->setBounds(x + off, y, size, height);
    pCell->layout();

    off += size;
    remainder = dSize - size;
  }
}
