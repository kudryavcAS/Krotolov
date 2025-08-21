#include "mole.h"
#include <QGraphicsScene>

Mole::Mole(const QPixmap &pixmap, QObject *parent)
    : QObject(parent), QGraphicsPixmapItem(pixmap)
{
    m_originalY = 0;
    m_yPos = m_originalY;

    // Устанавливаем начальное значение масштаба
    m_scaleFactor = 1.0;
    setTransformOriginPoint(boundingRect().center());

    setZValue(0);

    m_scaleAnimation = new QPropertyAnimation(this, "scaleFactor", this);
    m_scaleAnimation->setDuration(1500);
    m_scaleAnimation->setStartValue(0.7);
    m_scaleAnimation->setEndValue(1.3);
    m_scaleAnimation->setEasingCurve(QEasingCurve::InOutSine);

    m_yAnimation = new QPropertyAnimation(this, "yPos", this);
    m_yAnimation->setDuration(1500);
    m_yAnimation->setEasingCurve(QEasingCurve::InOutSine);

    m_animationGroup = new QParallelAnimationGroup(this);
    m_animationGroup->addAnimation(m_scaleAnimation);
    m_animationGroup->addAnimation(m_yAnimation);


    connect(m_animationGroup, &QParallelAnimationGroup::finished,
            this, &Mole::animationFinished);
}

qreal Mole::scaleFactor() const
{
    return m_scaleFactor;
}
void Mole::setScaleFactor(qreal newScaleFactor)
{
    if (qFuzzyCompare(m_scaleFactor, newScaleFactor))
        return;
    m_scaleFactor = newScaleFactor;
    this->setScale(m_scaleFactor);
    emit scaleFactorChanged();
}
qreal Mole::yPos() const
{
    return m_yPos;
}
void Mole::setYPos(qreal newYPos)
{
    if (qFuzzyCompare(m_yPos, newYPos))
        return;
    m_yPos = newYPos;
    // Устанавливаем новую позицию, X остается неизменным
    this->setPos(this->x(), m_yPos);
    emit yPosChanged();
}
void Mole::startAnimation()
{
    // Устанавливаем параметры анимации движения
    // Двигаемся вверх на 20 пикселей от исходной позиции
    m_yAnimation->setStartValue(m_originalY);      // Начало: исходная позиция
    m_yAnimation->setEndValue(m_originalY - 200);   // Конец: на 20 пикселей выше

    m_animationGroup->start();
}
void Mole::stopAnimation()
{
    m_animationGroup->stop();
    // Возвращаем в исходное состояние
    setScaleFactor(1.0);
    setYPos(m_originalY);
}
void Mole::setOriginalY(qreal y)
{
    m_originalY = y;
    m_yPos = y;
}
void Mole::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    emit moleHit();   // отправляем сигнал
    if (scene()) {
        scene()->removeItem(this);
    }
    delete this;      // уничтожаем крота
}
