#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
class QGraphicsScene;
class Mole;
class QGraphicsTextItem;
class QElapsedTimer;
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

protected:
   // void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow *ui;
    QGraphicsScene* m_scene;
    QTimer* spawnTimer;
    QTimer *updateTimer;
    int score = 0;
    int record = 0;
    int second = 0;
    QGraphicsTextItem *scoreText;
    QGraphicsTextItem *recordText;
    QGraphicsTextItem *timeText;
    QGraphicsTextItem *currentText;
    QElapsedTimer timer;
   // void updateViewFit();
};
#endif // MAINWINDOW_H
