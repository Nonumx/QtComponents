#include "tool_bar.h"
#include <QDebug>

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  
  ToolBar tb;
  tb.show();
  
  auto tb_ui = tb.GetUI();
  tb.connect(tb_ui->actionNew, &QAction::triggered, []{ qInfo() << "actionNew triggered"; });
  tb.connect(tb_ui->actionOpen, &QAction::triggered, []{ qInfo() << "actionOpen triggered"; });
  
  return QApplication::exec();
}
