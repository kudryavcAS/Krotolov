#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include "mole.h"
#include <QPixmap>
#include <QRandomGenerator>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //resizeEvent(nullptr);

    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(-100, -75, 200, 150);
//m_scene->setSceneRect(-1200, -900, 2400, 1800);

    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


updateViewScale();
    spawnTimer = new QTimer(this);
    connect(spawnTimer, &QTimer::timeout, this, &MainWindow::spawnMole);
    spawnTimer->start(2000); // Запускаем таймер: новый крот каждые 2 секунды

    // 5. Сразу создаем первого крота
    spawnMole();
}

void MainWindow::spawnMole()
{
    int randomIndex = QRandomGenerator::global()->bounded(0, 6);
    QPointF spawnPos;

    switch(randomIndex) {
    case 0:
        spawnPos = QPointF(-1000, -750); // Левый верхний
        break;
    case 1:
        spawnPos = QPointF(0, -750);    // Центр верхний
        break;
    case 2:
        spawnPos = QPointF(1000, -750);  // Правый верхний
        break;
    case 3:
        spawnPos = QPointF(-1000, 750);  // Левый нижний
        break;
    case 4:
        spawnPos = QPointF(0, 750);     // Центр нижний
        break;
    case 5:
        spawnPos = QPointF(1000, 750);   // Правый нижний
        break;
    default:
        spawnPos = QPointF(0, 0);      // На всякий случай
        break;
    }

    qDebug() << "Spawning mole at:" << spawnPos << "Scene rect:" << m_scene->sceneRect();
    // Остальная логика создания крота остается такой же...
    QPixmap molePixmap(":/images/mole.png");
    Mole* newMole = new Mole(molePixmap);

    QRectF moleRect = newMole->boundingRect();
    newMole->setPos(spawnPos.x() - moleRect.width()/2,
                    spawnPos.y() - moleRect.height()/2);
    newMole->setOriginalY(spawnPos.y() - moleRect.height()/2);

    m_scene->addItem(newMole);
    newMole->startAnimation();

    // Автоудаление через 3 секунды
    connect(newMole, &Mole::animationFinished, this, [newMole, this]() {
        QTimer::singleShot(100, this, [newMole, this]() { // небольшая задержка
            if (newMole && m_scene->items().contains(newMole)) {
                m_scene->removeItem(newMole);
                delete newMole;
            }
        });
    });
}

void MainWindow::updateViewScale()
{
    QRect viewGeometry = ui->graphicsView->geometry();
    QRectF sceneRect = m_scene->sceneRect();

    qreal scaleX = viewGeometry.width() / sceneRect.width();
    qreal scaleY = viewGeometry.height() / sceneRect.height();
    qreal scale = qMin(scaleX, scaleY);

    QTransform transform;
    transform.scale(scale, scale);
    ui->graphicsView->setTransform(transform);
    ui->graphicsView->centerOn(0, 0);
}
void MainWindow::resizeEvent(QResizeEvent *event)
{
   QMainWindow::resizeEvent(event);
  updateViewScale(); // Масштабируем БЕЗ сброса позиций
}
MainWindow::~MainWindow()
{
    delete ui;
}
