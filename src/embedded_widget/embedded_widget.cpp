#include "embedded_widget.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QWindow>


#ifdef Q_OS_WIN
#include "window_finder_win.h"
#endif

EmbeddedWidget::EmbeddedWidget(QWidget* parent)
    : QWidget(parent), m_finder(new WindowFinder(this))
{
    auto* main_layout = new QHBoxLayout(this);
    main_layout->setMargin(0);
    connect(m_finder, &WindowFinder::FoundWindow, this,
            &EmbeddedWidget::EmbedWindow);
}

void EmbeddedWidget::SetProgram(const QString& program) const
{
    m_finder->SetProgram(program);
}

void EmbeddedWidget::SetArguments(const QStringList& arguments) const
{
    m_finder->SetArguments(arguments);
}

void EmbeddedWidget::Start() const
{
    m_finder->Start();
}

void EmbeddedWidget::EmbedWindow(WId wid)
{
    while (layout()->count() > 0)
    {
        auto* item = layout()->takeAt(0);
        item->widget()->deleteLater();
    }
    auto* window = QWindow::fromWinId(wid);
    auto* container = createWindowContainer(window, this);
    layout()->addWidget(container);
}
