#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QRandomGenerator>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QDebug>

#include "mole.h"


qreal SCENE_W = 200.0; // ширина
qreal SCENE_H = 150.0; // высота
qreal SCENE_X = -SCENE_W / 2.0;
qreal SCENE_Y = -SCENE_H / 2.0;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_scene(new QGraphicsScene(this))
    , spawnTimer(new QTimer(this))
{
    ui->setupUi(this);


    m_scene->setSceneRect(SCENE_X, SCENE_Y, SCENE_W , SCENE_H);
    // QPixmap backgroundPixmap(":/images/fon.png");
    QColor customColor(0, 100, 0, 255);
    m_scene->setBackgroundBrush(QBrush(customColor));

    ui->graphicsView->setScene(m_scene);
    //ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->fitInView(ui->graphicsView->scene()->sceneRect(),Qt::KeepAspectRatio);

    scoreText = m_scene->addText("Счёт: 0");
    scoreText->setDefaultTextColor(Qt::white);
    scoreText->setScale(12); // увеличить текст
    scoreText->setZValue(3); // поверх всего
    scoreText->setPos(-2000, -2000);

    recordText = m_scene->addText(QString("Рекорд: %1 в минуту").arg(record));
    recordText->setDefaultTextColor(Qt::white);
    recordText->setScale(12); // увеличить текст
    recordText->setZValue(3); // поверх всего
    recordText->setPos(0, -2000);

    timeText = m_scene->addText("Время: 0");
    timeText->setDefaultTextColor(Qt::white);
    timeText->setScale(12);
    timeText->setZValue(3);
    timeText->setPos(-2000, -1800);

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, [this]() {
        second++;
        timeText->setPlainText(QString("Время: %1 сек").arg(second));

        if (second >= 60) {
            on_stop_triggered();  // завершаем игру
            qDebug() << "Игра завершена: время вышло!";
        }
    });
    QVector<QPointF> holePositions = {
        {-1000, -750}, {0, -750}, {1000, -750},
        {-1000, 750},  {0, 750},  {1000, 750}
    };
    for (const QPointF &pos : holePositions) {
        auto *hole = m_scene->addPixmap(QPixmap(":/images/nnhole.png"));
        QRectF rect = hole->boundingRect();
        hole->setScale(2);
        hole->setPos(pos.x() - rect.width()/2, pos.y() - rect.height()/2);
        hole->setZValue(1);
    }

    // connect(spawnTimer, &QTimer::timeout, this, &MainWindow::spawnMole);
    // spawnTimer->start(2000); // Запускаем таймер: новый крот каждые 2 секунды

    // // 5. Сразу создаем первого крота
    // spawnMole();

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
    QPixmap molePixmap(":/images/mole12.png");
    Mole* newMole = new Mole(molePixmap);

    QRectF moleRect = newMole->boundingRect();
    newMole->setPos(spawnPos.x() - moleRect.width()/2,
                    spawnPos.y() - moleRect.height()/2);
    newMole->setOriginalY(spawnPos.y() - moleRect.height()/2);

    m_scene->addItem(newMole);
    newMole->startAnimation();

    connect(newMole, &Mole::moleHit, this, &MainWindow::handleMoleHit);
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

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_newgame_triggered()
{
    second =0;
    score = 0;

    disconnect(spawnTimer, nullptr, this, nullptr);
    connect(spawnTimer, &QTimer::timeout, this, &MainWindow::spawnMole);

    timeText->setPlainText("Время: 0");
    timer.restart();
    updateTimer->start(1000);

    scoreText->setPlainText(QString("Счёт: %1").arg(score));
    recordText->setPlainText(QString("Рекорд: %1 в минуту").arg(record));

    if (currentText && m_scene->items().contains(currentText)) {
        m_scene->removeItem(currentText);
        delete currentText;
        currentText = nullptr;
    }

    spawnTimer->start(2000); // Запускаем таймер: новый крот каждые 2 секунды
    updateTimer->start(1000);


    spawnMole();
}
void MainWindow::on_exit_triggered()
{
    MainWindow::close();
}
void MainWindow::on_stop_triggered()
{
    if (record < score){
        record = score;
    }
    if (updateTimer && updateTimer->isActive()) {
        updateTimer->stop();
    }
    if (spawnTimer && spawnTimer->isActive()) {
        spawnTimer->stop();   // останавливаем таймер появления кротов
    }
    currentText = m_scene->addText(QString("Результат: %1 в минуту").arg(score));
    currentText->setDefaultTextColor(Qt::white);
    currentText->setScale(12); // увеличить текст
    currentText->setZValue(3); // поверх всего
    currentText->setPos(0, -1800);

}
void MainWindow::handleMoleHit() {
    score++;
    scoreText->setPlainText(QString("Счёт: %1").arg(score));
    qDebug() << "Счёт: " << score;
}
