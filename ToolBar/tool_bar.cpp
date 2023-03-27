#include "tool_bar.h"

ToolBar::ToolBar(QWidget* parent) : QToolBar(parent) { ui_.setupUi(this); }

ToolBar::~ToolBar() = default;
