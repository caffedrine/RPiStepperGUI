#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QCoreApplication>
#include <QQmlContext>



class MainWindow : public QObject
{
    Q_OBJECT
public:
    QQmlApplicationEngine *engine;

    explicit MainWindow(QWidget *parent = nullptr);
    void SetupUI();

signals:

public slots:    
    void SetProperty(const char* property, const QVariant value);
    //void SetStatus(QString text, UiStatusType status);

private:

    QObject *ui;
};

#endif // MAINWINDOW_H
