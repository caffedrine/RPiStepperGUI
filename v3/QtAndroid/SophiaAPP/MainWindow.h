#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QCoreApplication>



class MainWindow : public QObject
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void SetupUI();
    void SetProperty(const char* property, const QVariant value);
    //void SetStatus(QString text, UiStatusType status);

signals:

public slots:    

private:
    QQmlApplicationEngine *engine;
    QObject *ui;
};

#endif // MAINWINDOW_H
