#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>
#include "MyView.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
class QGraphicsScene;
class Mole;
class QGraphicsTextItem;
class QElapsedTimer;
class QGraphicsPixmapItem;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void spawnMole();
    void handleMoleHit();

    void on_newgame_triggered();
    void on_exit_triggered();
    void on_stop_triggered();

    void on_rules_triggered();

signals:
    void increaseDifficulty();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* m_scene;
    QTimer* spawnTimer;
    QTimer *updateTimer;

    int score = 0;
    int record = 0;
    int second = 0;

    QGraphicsPixmapItem *lopata = nullptr;
    MyView *myView;

    QGraphicsTextItem *scoreText;
    QGraphicsTextItem *recordText;
    QGraphicsTextItem *timeText;
    QGraphicsTextItem *currentText;
    QGraphicsTextItem *winText;

    QElapsedTimer timer;

    QMediaPlayer *wplayer;
    QAudioOutput *waudio;
    QMediaPlayer *splayer;
    QAudioOutput *saudio;



};
#endif // MAINWINDOW_H
