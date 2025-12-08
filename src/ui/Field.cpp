#include "Field.h"

#include "Application.h"

// ui events

void Field::onSubmit() {
  if (listenerSubmit) listenerSubmit();
}

void Field::onTextChanged() {
  if (listenerChanged) listenerChanged();
}

void Field::handleClick(UiEvent &event) {
  select = -1;
  if (!event.down) return;
  if (!hovered()) {
    cursor = -1;
    return;
  }
  if (event.button != GLFW_MOUSE_BUTTON_LEFT) return;
  cursor = static_cast<int>(textData.size());
  msInteracted = msCurrent();
}

void Field::handleKeyDown(UiEvent &event) {
  if (!focused()) return;

  event.claimed = true;

  auto &app = Application::app;

  if (glfwGetKey(app.window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
    switch (event.button) {
      case GLFW_KEY_A: {
        select = 0;
        cursor = static_cast<int>(textData.size());
        return;
      }
      case GLFW_KEY_C: {
        if (!selected()) return;
        int min2 = std::min(cursor, select);
        int max2 = std::max(cursor, select);
        std::string selected(textData.begin() + min2, textData.begin() + max2);
        app.setClipboardText(selected);
        return;
      }
      case GLFW_KEY_X: {
        if (!selected()) return;
        int min2 = std::min(cursor, select);
        int max2 = std::max(cursor, select);
        std::string selected2(textData.begin() + min2, textData.begin() + max2);
        app.setClipboardText(selected2);
        deleteSelected();
        return;
      }
      case GLFW_KEY_V: {
        deleteSelected();
        std::string s;
        if (app.getClipboardText(s)) {
          for (char c: s) {
            if (!allowChar(c)) continue;
            if (textData.size() >= this->max) break;
            textData.insert(textData.begin() + cursor, c);
            cursor++;
          }
          onTextChanged();
          return;
        }
      }
      default:
        break;
    }
  }

  bool shiftDown = glfwGetKey(app.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;

  bool interact = true;
  switch (event.button) {
    case GLFW_KEY_HOME: {
      if (shiftDown && !selected()) select = cursor;
      else if (!shiftDown && exitSelection()) break;
      cursor = 0;
      break;
    }
    case GLFW_KEY_END: {
      if (shiftDown && !selected()) select = cursor;
      else if (!shiftDown && exitSelection()) break;
      cursor = static_cast<int>(textData.size());
      break;
    }
    case GLFW_KEY_LEFT: {
      if (shiftDown && !selected()) select = cursor;
      else if (!shiftDown && exitSelection()) break;
      if (cursor > 0) cursor = getNextCursor(-1);
      break;
    }
    case GLFW_KEY_RIGHT: {
      if (shiftDown && !selected()) select = cursor;
      else if (!shiftDown && exitSelection()) break;
      if (cursor < textData.size()) cursor = getNextCursor(1);
      break;
    }
    case GLFW_KEY_BACKSPACE: {
      if (deleteSelected()) break;
      if (cursor <= 0) break;
      int next1 = getNextCursor(-1);
      textData.erase(textData.begin() + next1, textData.begin() + cursor);
      cursor = next1;
      onTextChanged();
      break;
    }
    case GLFW_KEY_DELETE: {
      if (deleteSelected()) break;
      if (cursor == textData.size()) break;
      int next2 = getNextCursor(1);
      textData.erase(textData.begin() + cursor, textData.begin() + next2);
      onTextChanged();
      break;
    }
    case GLFW_KEY_ENTER: {
      distract();
      onSubmit();
      break;
    }
    default: {
      interact = false;
      break;
    }
  }

  if (select == cursor) select = -1;

  if (interact) msInteracted = msCurrent();
}

void Field::handleChar(UiEvent &event) {
  if (!focused()) return;

  char c = static_cast<char>(event.codepoint);
  if (c < 32) return;
  if (!allowChar(c)) return;
  deleteSelected();
  if (textData.size() >= max) return;
  textData.insert(textData.begin() + cursor, c);
  onTextChanged();
  cursor++;
  msInteracted = msCurrent();
}

void Field::handle(UiEvent &event) {
  if (event.type == UiEventType::CHAR) {
    handleChar(event);
  } else if (event.type == UiEventType::MOUSE_BUTTON) {
    handleClick(event);
  } else if (event.type == UiEventType::KEY) {
    if (event.down) handleKeyDown(event);
  }
}

void Field::render(double dt, const UiRenderParams &params) {
  auto &app = Application::app;
  auto &render = app.renderUi;
  auto &renderFont = app.renderFont;

  std::string text = this->text();

  int rightSpace = static_cast<int>(round(width * 0.15f));

  int xPadding = 3;

  render.start();
  render.color(0x101010ff);
  if (params.toBuffer)
    glBlendFunc(GL_ONE, GL_ZERO);
  render.rect(x, y, width, height, 3.0f, 1.0f);
  if (params.toBuffer)
    glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
  render.stop();

  int textHeight = renderFont.height();

  int cursorX = renderFont.width(text.substr(0, focused() ? cursor : 0));

  int offset = -cursorX + (width - xPadding * 2) - rightSpace;

  if (cursorX < (width - xPadding * 2) - rightSpace) {
    offset = 0;
  }

  app.pushScissors(x + xPadding, y, width - xPadding * 2, height);

  if (selected()) {
    int selectionHeight = textHeight + 2;
    int selectionY = static_cast<int>(y + round((height - textHeight) / 2.0f));

    int min2 = std::min(cursor, select);
    int max2 = std::max(cursor, select);

    int minX = renderFont.width(text.substr(0, min2));
    int width = renderFont.width(text.substr(min2, max2 - min2));

    render.start();
    render.color(0x006080ff);
    render.rect(x + xPadding + minX + offset, selectionY, width, selectionHeight);
    render.stop();
  }

  renderFont.start();
  if (params.toBuffer)
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
  renderFont.renderText(
    text,
    static_cast<float>(x + xPadding + offset),
    y + round((height - textHeight) / 2.0f),
    0xffffffff
  );
  if (params.toBuffer)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  renderFont.stop();

  uint64_t msNow = msCurrent();
  if ((msNow % 700 > 350 || msNow - msInteracted < 400) && focused()) {
    int cursorHeight = textHeight + 4;
    int cursorY = y + 2;
    render.start();
    render.color(0xffffffff);
    render.rect(x + xPadding + cursorX + offset, cursorY, 2, cursorHeight);
    render.stop();
  }

  app.popScissors();
}

//
// text util
//

bool Field::deleteSelected() {
  if (select == -1) return false;
  int min2 = std::min(cursor, select);
  int max2 = std::max(cursor, select);
  if (min2 == max2) return false;
  cursor = min2;
  select = -1;
  textData.erase(textData.begin() + min2, textData.begin() + max2);
  onTextChanged();
  return true;
}

bool Field::exitSelection() {
  bool selected = this->selected();
  this->select = -1;
  return selected;
}

bool isSign(char c) {
  return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}

int Field::getNextCursor(int direction) {
  auto &app = Application::app;
  bool ctrlDown = glfwGetKey(app.window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;

  int result = this->cursor;
  result += direction;
  if (!ctrlDown) return result;

  if (result == textData.size()) return result;
  bool lastSign = isSign(textData[result]);
  result += direction;
  while (result > 0 && result < textData.size()) {
    bool sign = isSign(textData[result]);
    if (sign != lastSign) return result - (direction == -1 ? -1 : 0);
    result += direction;
  }
  if (result < 0) result = 0;
  return result;
}

bool Field::allowChar(char c) {
  if (c < 32) return false;

  if (allowCharFunc && !allowCharFunc(c)) return false;

  return true;
}

void Field::distract() {
  cursor = -1;
}

void Field::set(const std::string &value) {
  setSilently(value);
  onTextChanged();
}

void Field::setSilently(const std::string &value) {
  textData.clear();
  for (char c: value) {
    if (!allowChar(c)) continue;
    if (textData.size() >= this->max) break;
    textData.push_back(c);
  }
  cursor = -1;
}

void Field::focus() {
  cursor = static_cast<int>(textData.size());
}
