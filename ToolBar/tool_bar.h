#pragma once

#include "ui_tool_bar.h"

class ToolBar : public QToolBar {
  Q_OBJECT

 public:
  ToolBar(QWidget* parent = nullptr);
  ~ToolBar() override;

  auto GetUI() { return ui_; }

 private:
  Ui::ToolBarClass ui_;
};
