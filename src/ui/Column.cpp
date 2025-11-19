#include "Column.h"

#include "Log.h"

static int queryWrapHeight(Cell *cell) {
  int subWidth = 0, subHeight = 0;
  bool subHasWidth = false, subHasHeight = false;
  cell->queryWrap(subWidth, subHeight, subHasWidth, subHasHeight);
  if (!subHasHeight) {
    Log::warn("COL: A wrapped cell does not provide its size");
  }
  return subHeight;
}

void Column::queryWrap(int &width, int &height, bool &hasWidth, bool &hasHeight) {
  hasHeight = true;
  int totalAbsolute = 0;
  for (auto &child: children) {
    const auto pCell = dynamic_cast<Cell *>(child.get());
    if (pCell->type == CellType::ABSOLUTE) {
      totalAbsolute += pCell->size;
    } else if (pCell->type == CellType::WRAP) {
      totalAbsolute += queryWrapHeight(pCell);
    }
  }
  height = totalAbsolute;

  // transfer opposite dimension
  int subWidth = 0, subHeight = 0;
  bool subHasWidth = false, subHasHeight = false;
  queryWrapChildren(subWidth, subHeight, subHasWidth, subHasHeight);
  if (subHasWidth) {
    hasWidth = true;
    width = subWidth;
  }
}

void Column::layout() {
  const int total = height;

  // find total absolute and total relative space, also find last relative cell
  int totalAbsolute = 0;
  int totalRelative = 0;
  const Cell *lastRelative = nullptr;
  for (auto &child: children) {
    const auto pCell = dynamic_cast<Cell *>(child.get());
    if (pCell->type == CellType::ABSOLUTE) {
      totalAbsolute += pCell->size;
    } else if (pCell->type == CellType::WRAP) {
      totalAbsolute += queryWrapHeight(pCell);
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
    } else if (pCell->type == CellType::WRAP) {
      dSize += queryWrapHeight(pCell);
    } else {
      dSize += static_cast<double>(pCell->size) / static_cast<double>(totalRelative) * available;
    }

    // calculate integer size
    int size = static_cast<int>(dSize);
    if (pCell == lastRelative) size = available - taken; // last relative block gets pixels left

    // update relative size which was taken
    if (pCell->type == CellType::RELATIVE) taken += size;

    // layout cell
    pCell->setBounds(x, y + off, width, size);
    pCell->setViewport(vpx1, vpy1, vpx2, vpy2);
    pCell->layout();

    off += size;
    remainder = dSize - size;
  }
}
