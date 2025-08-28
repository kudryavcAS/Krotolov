#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QDebug>
#include <QMouseEvent>
#include <QUrl>
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
    , wplayer(new QMediaPlayer(this))
    , waudio(new QAudioOutput(this))
    , splayer(new QMediaPlayer(this))
    , saudio(new QAudioOutput(this))
{
    ui->setupUi(this);

    QColor customColor(0, 100, 0, 255);

    m_scene->setSceneRect(SCENE_X, SCENE_Y, SCENE_W , SCENE_H);
    m_scene->setBackgroundBrush(QBrush(customColor));

    myView = new MyView(this);
    myView->setScene(m_scene);
    myView->setCursor(Qt::BlankCursor);
    ui->verticalLayout->addWidget(myView);

    myView->setRenderHint(QPainter::SmoothPixmapTransform);
    myView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    myView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    myView->fitInView(myView->scene()->sceneRect(),Qt::KeepAspectRatio);

    lopata = m_scene->addPixmap(QPixmap(":/images/lopata.png"));
    lopata->setZValue(2);
    lopata->setScale(5);

    connect(myView, &MyView::mouseMoved, this, [this](const QPoint &pos){
        if (lopata) {

            QPointF scenePos = myView->mapToScene(pos);
            QRectF rect = lopata->boundingRect();
            lopata->setPos(scenePos.x() - 175,
                           scenePos.y() - rect.height()*5 + 175);
        }
    });

    myView->setMouseTracking(true);
    this->setMouseTracking(true);

    scoreText = m_scene->addText("Счёт: 0");
    scoreText->setDefaultTextColor(Qt::white);
    scoreText->setScale(12);
    scoreText->setZValue(3);
    scoreText->setPos(-2000, -2000);

    recordText = m_scene->addText(QString("Рекорд: %1 в минуту").arg(record));
    recordText->setDefaultTextColor(Qt::white);
    recordText->setScale(12);
    recordText->setZValue(3);
    recordText->setPos(0, -2000);

    timeText = m_scene->addText("Время: 0");
    timeText->setDefaultTextColor(Qt::white);
    timeText->setScale(12);
    timeText->setZValue(3);
    timeText->setPos(-2000, -1800);

    wplayer->setAudioOutput(waudio);
    wplayer->setSource(QUrl("qrc:/music/nalito.mp3"));
    waudio->setVolume(0.5);

    splayer->setAudioOutput(saudio);
    splayer->setSource(QUrl("qrc:/music/strike.mp3"));
    saudio->setVolume(0.5);

    QGraphicsPixmapItem *house = m_scene->addPixmap(QPixmap(":/images/dom.png"));
    house->setScale(0.3);
    house->setZValue(1);
    house->setPos(-4500, -1800);

    QGraphicsPixmapItem *wc = m_scene->addPixmap(QPixmap(":/images/wc.png"));
    wc->setScale(1.5);
    wc->setZValue(1);
    wc->setPos(3700, 1000);

    QGraphicsPixmapItem *pavil = m_scene->addPixmap(QPixmap(":/images/pavil.png"));
    pavil->setScale(4.0);
    pavil->setZValue(1);
    pavil->setPos(-4500, 900);

    QGraphicsPixmapItem *forest = m_scene->addPixmap(QPixmap(":/images/forest.png"));
    forest->setScale(1.0);
    forest->setZValue(1);
    forest->setPos(2700, -2500);

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, [this]() {
        second++;
        timeText->setPlainText(QString("Время: %1 сек").arg(second));

        if (second == 30) {
            spawnTimer->setInterval(800);
            emit increaseDifficulty();
        }

        if (second >= 60) {
            on_stop_triggered();
        }
    });
    QVector<QPointF> holePositions = {
        {-1800, -850}, {-300, -850}, {1200, -850},
        {-1800, 850},  {-300, 850},  {1200, 850}
    };
    for (const QPointF &pos : holePositions) {
        auto *hole = m_scene->addPixmap(QPixmap(":/images/nnhole.png"));
        QRectF rect = hole->boundingRect();
        hole->setScale(2);
        hole->setPos(pos.x() - rect.width()/2, pos.y() - rect.height()/2);
        hole->setZValue(1);
    }
}

void MainWindow::spawnMole()
{
    int randomIndex = QRandomGenerator::global()->bounded(0, 6);
    QPointF spawnPos;

    switch(randomIndex) {
    case 0:
        spawnPos = QPointF(-1450, -850);
        break;
    case 1:
        spawnPos = QPointF(50, -850);
        break;
    case 2:
        spawnPos = QPointF(1550, -850);
        break;
    case 3:
        spawnPos = QPointF(-1450, 850);
        break;
    case 4:
        spawnPos = QPointF(50, 850);
        break;
    case 5:
        spawnPos = QPointF(1550, 850);
        break;
    default:
        spawnPos = QPointF(0, 0);
        break;
    }

    QPixmap molePixmap(":/images/mole12.png");
    Mole* newMole = new Mole(molePixmap);
    QRectF moleRect = newMole->boundingRect();
    newMole->setPos(spawnPos.x() - moleRect.width()/2,
                    spawnPos.y() - moleRect.height()/2);
    newMole->setOriginalY(spawnPos.y() - moleRect.height()/2);

    m_scene->addItem(newMole);
    newMole->startAnimation();

    connect(newMole, &Mole::moleHit, this, &MainWindow::handleMoleHit);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_newgame_triggered()
{
    second =0;
    score = 0;

    wplayer->stop();

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
    if (winText && m_scene->items().contains(winText)) {
        m_scene->removeItem(winText);
        delete winText;
        winText = nullptr;
    }
    spawnTimer->start(1000);
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

    if (currentText && m_scene->items().contains(currentText)) {
        m_scene->removeItem(currentText);
        delete currentText;
        currentText = nullptr;
    }
    if (winText && m_scene->items().contains(winText)) {
        m_scene->removeItem(winText);
        delete winText;
        winText = nullptr;
    }

    if(score <= 70){
        winText = m_scene->addText("Вы победили!");
        winText->setDefaultTextColor(Qt::yellow);
        winText->setScale(15);
        winText->setZValue(3);
        winText->setPos(-700, -1500);


        // запускаем проигрывание
        wplayer->play();

    }
    if (updateTimer && updateTimer->isActive()) {
        updateTimer->stop();
    }
    if (spawnTimer && spawnTimer->isActive()) {
        spawnTimer->stop();
    }
    currentText = m_scene->addText(QString("Результат: %1 в минуту").arg(score));
    currentText->setDefaultTextColor(Qt::white);
    currentText->setScale(12); // увеличить текст
    currentText->setZValue(3); // поверх всего
    currentText->setPos(0, -1800);

}
void MainWindow::handleMoleHit() {
    splayer->play();
    score++;
    scoreText->setPlainText(QString("Счёт: %1").arg(score));

}


void MainWindow::on_rules_triggered()
{
    QString rulesText =
        "🎮 Игра длится 60 секунд.\n"
        "      Цель - набрать как можно боьше очков,\n      попадая по появляющимя кротам\n"
        "      После 30 секунд скорость появления кротов увеличивается\n"
        "🎯 Чтобы победить - наберите не меньше 70 очков.\n"
        "⚡ Совет: бейте по верхней части лунки - так легче поймать крота\n"    ;

    QMessageBox::information(this, "Правила игры", rulesText);
}
