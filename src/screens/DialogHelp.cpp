#include "DialogHelp.h"

#include "Application.h"

// @formatter:off
static std::shared_ptr<Ui> _lineKeyBind(const std::string &key, const std::string &action) {
  return Row::make({
    Cell::abs(Label::make(key, 0xffffffb0, LabelAlign::RIGHT), 150),
    Cell::abs(24),
    Cell::rel(Label::make(action, LabelAlign::LEFT)),
  });
}
// @formatter:on

// @formatter:off
DialogHelp::DialogHelp(const std::shared_ptr<Ui> &prev) : prev(prev) {
  std::vector<std::shared_ptr<ItemY>> helpItems;

  helpItems.push_back(ItemY::make(20));

  helpItems.push_back(ItemY::make(Label::make("General Shortcuts", 0xffffff60, LabelAlign::LEFT), 24));
  helpItems.push_back(ItemY::make(_lineKeyBind("F11", "Toggle fullscreen"), 24));
  helpItems.push_back(ItemY::make(20));

  helpItems.push_back(ItemY::make(Label::make("Help Screen Shortcuts", 0xffffff60, LabelAlign::LEFT), 24));
  helpItems.push_back(ItemY::make(_lineKeyBind("ESC", "Exit"), 24));
  helpItems.push_back(ItemY::make(20));

  helpItems.push_back(ItemY::make(Label::make("Main Screen Shortcuts", 0xffffff60, LabelAlign::LEFT), 24));
  helpItems.push_back(ItemY::make(_lineKeyBind("F1", "Help..."), 24));
  helpItems.push_back(ItemY::make(_lineKeyBind("Space", "Play selected drill"), 24));
  helpItems.push_back(ItemY::make(_lineKeyBind("Tab", "Select drill"), 24));
  helpItems.push_back(ItemY::make(20));

  helpItems.push_back(ItemY::make(Label::make("Main Screen Debug Shortcuts", 0xffffff60, LabelAlign::LEFT), 24));
  helpItems.push_back(ItemY::make(_lineKeyBind("T", "Open test screen"), 24));
  helpItems.push_back(ItemY::make(_lineKeyBind("1", "Open test scene 1"), 24));
  helpItems.push_back(ItemY::make(20));

  helpItems.push_back(ItemY::make(Label::make("In Game Shortcuts", 0xffffff60, LabelAlign::LEFT), 24));
  helpItems.push_back(ItemY::make(_lineKeyBind("ESC", "Pause"), 24));
  helpItems.push_back(ItemY::make(20));

  helpItems.push_back(ItemY::make(Label::make("Pause Screen Shortcuts", 0xffffff60, LabelAlign::LEFT), 24));
  helpItems.push_back(ItemY::make(_lineKeyBind("ESC", "Resume"), 24));
  helpItems.push_back(ItemY::make(_lineKeyBind("R", "Restart"), 24));
  helpItems.push_back(ItemY::make(_lineKeyBind("Q", "Quit"), 24));
  helpItems.push_back(ItemY::make(_lineKeyBind("S", "Settings"), 24));
  helpItems.push_back(ItemY::make(20));

  helpItems.push_back(ItemY::make(Label::make("Settings Screen Shortcuts", 0xffffff60, LabelAlign::LEFT), 24));
  helpItems.push_back(ItemY::make(_lineKeyBind("ESC", "Go to previous screen"), 24));
  helpItems.push_back(ItemY::make(_lineKeyBind("Shift", "Slider: Fine control"), 24));
  helpItems.push_back(ItemY::make(_lineKeyBind("Shift + Arrow", "Field: Select field. Also works with Home/End"), 24));
  helpItems.push_back(ItemY::make(_lineKeyBind("Ctrl + C/V/X", "Field: Copy paste in text field"), 24));
  helpItems.push_back(ItemY::make(_lineKeyBind("Ctrl + Arrow", "Field: Move cursor by word. Also works with Home/End and Shift"), 24));

  helpItems.push_back(ItemY::make(20));

  add(Row::make({
    Cell::abs(100),
    Cell::rel(Column::make({
      Cell::abs(20),
      Cell::rel(PanelY::make(helpItems)),
      Cell::abs(20),
    })),
    Cell::abs(100)
  }));
}
// @formatter:on

void DialogHelp::handle(UiEvent &event) {
  Ui::handle(event);

  if (event.type == UiEventType::KEY && event.down) {
    if (event.button == GLFW_KEY_ESCAPE) {
      auto prev = this->prev;
      Application::app.later([prev] { Application::app.setScreen(prev); });
    }
  }
}
