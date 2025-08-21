#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
class QGraphicsScene;
class Mole;
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
    int score = 0;
   // void updateViewFit();
};
#endif // MAINWINDOW_H
