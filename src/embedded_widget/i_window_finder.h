#pragma once

#include <QObject>
#include <QProcess>

class IWindowFinder : public QObject
{

public:
    explicit IWindowFinder(QObject* parent = nullptr);

    void SetProgram(const QString& program) const;
    void SetArguments(const QStringList& arguments) const;

public slots:
    void Start();

protected:
    QProcess* m_process;
};