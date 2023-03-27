## Toolbar

工具栏，用于Windows的应用程序

### 关联信号

通过将UI Class导出进行信号关联

```c++
ToolBar tb;
auto ui = tb.GetUI();
connect(ui.actionNew, QAction::triggered, ...);
```

### 添加图标

通过 `.ui` 文件来添加 `QAction` 和图标








