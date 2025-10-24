#include "UiDrill.h"

#include "screens/ScreenMain.h"
#include "Application.h"

// @formatter:off
UiDrill::UiDrill() {
  add(Column::make({
    Cell::abs(Row::make({
      Cell::rel(),
      Cell::abs(lblMainStat = Label::make(""), 200),
      Cell::rel(),
    }), 30),
    Cell::rel(),
  }));
}
// @formatter:on

void UiDrill::handle(UiEvent &event) {
  Ui::handle(event);
}
