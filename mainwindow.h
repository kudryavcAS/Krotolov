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

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::MainWindow *ui;
 void updateViewScale();
    QGraphicsScene* m_scene;
    QTimer* spawnTimer;
};
#endif // MAINWINDOW_H
