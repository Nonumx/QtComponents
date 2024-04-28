#include "i_window_finder.h"

IWindowFinder::IWindowFinder(QObject* parent)
    : QObject(parent), m_process(new QProcess(this)) {}

void IWindowFinder::SetProgram(const QString& program) const
{
    m_process->setProgram(program);
}

void IWindowFinder::SetArguments(const QStringList& arguments) const
{
    m_process->setArguments(arguments);
}

void IWindowFinder::Start()
{
    m_process->start();
}