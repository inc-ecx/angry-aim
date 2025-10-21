#include "ScreenTest.h"

#include "../Application.h"
#include "ui/Field.h"
#include "ui/PanelY.h"
#include "ui/Range.h"
#include "ui/Slider.h"

// @formatter:off
std::shared_ptr<Ui> constructTestList() {
  std::vector<std::shared_ptr<ItemY> > items;

  items.push_back(ItemY::make(Row::make({
    Cell::abs(5),
    Cell::rel(Button::make("Test")),
    Cell::abs(5)
  }), 25));

  items.push_back(ItemY::make(Row::make({
    Cell::abs(5),
    Cell::rel(Field::make("Test", nullptr, nullptr)),
    Cell::abs(5)
  }), 25));

  items.push_back(ItemY::make(Row::make({
    Cell::abs(5),
    Cell::rel(Slider::make([]{})),
    Cell::abs(5)
  }), 25));

  items.push_back(ItemY::make(Row::make({
    Cell::abs(5),
    Cell::rel(Range::make(0.0, 30.0, 160.0, 0.01, [](double value) {})),
    Cell::abs(5)
  }), 25));

  items.push_back(ItemY::make(10));

  // items.push_back(ItemY::make(Label::make("Scene selector panel"), 25));
  // items.push_back(ItemY::make(10));
  return PanelY::make(items);
}
// @formatter:on

// @formatter:off
ScreenTest::ScreenTest(const std::shared_ptr<Ui> &prev) : prev(prev)  {
  add(Column::make({
    Cell::abs(Row::make({
      Cell::rel(Row::make({
        Cell::abs(Button::make("Back", std::bind(&ScreenTest::actionBack, this)), 120),
        Cell::rel(1),
      }), 1),
      Cell::abs(Label::make("AngryAim"), 150),
      Cell::rel(1),
    }), 30),
    Cell::abs(40),
    Cell::rel(Row::make({
      Cell::rel(),
      Cell::abs(constructTestList(), 400),
      Cell::rel()
    })),
    Cell::abs(40),
  }));
}

void ScreenTest::handle(UiEvent &event) {
  if (event.type == UiEventType::KEY && event.down && event.button == GLFW_KEY_ESCAPE) {
    std::shared_ptr<Ui> prev = this->prev;
    Application::app.later([prev] {
      Application::app.setScreen(prev);
    });
  }

  Ui::handle(event);
}

// @formatter:on

void ScreenTest::actionBack() {
  Application::app.setScreen(prev);
}
