#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QCoreApplication>



class MainWindow : public QObject
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void SetupUI();

signals:
    void onConnectButtonPressed();
    void onConnectButtonReleased();

public slots:    
    void SetProperty(const char* property, const QVariant value);
    //void SetStatus(QString text, UiStatusType status);

    Q_INVOKABLE void connectButtonPressed();
    Q_INVOKABLE void connectButtonReleased();

private:
    QQmlApplicationEngine *engine;
    QObject *ui;
};

#endif // MAINWINDOW_H
