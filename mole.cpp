#include "mole.h"
#include <QGraphicsScene>

Mole::Mole(const QPixmap &pixmap, QObject *parent)
    : QObject(parent), QGraphicsPixmapItem(pixmap)
{
    m_originalY = 0;
    m_yPos = m_originalY;

    m_scaleFactor = 1.0;
    setTransformOriginPoint(boundingRect().center());

    setZValue(0);

    m_scaleAnimation = new QPropertyAnimation(this, "scaleFactor", this);
    m_scaleAnimation->setDuration(750);
    m_scaleAnimation->setStartValue(0.7);
    m_scaleAnimation->setEndValue(1.3);
    m_scaleAnimation->setEasingCurve(QEasingCurve::InOutSine);

    m_yAnimation = new QPropertyAnimation(this, "yPos", this);
    m_yAnimation->setDuration(750);
    m_yAnimation->setEasingCurve(QEasingCurve::InOutSine);

    m_animationGroup = new QParallelAnimationGroup(this);
    m_animationGroup->addAnimation(m_scaleAnimation);
    m_animationGroup->addAnimation(m_yAnimation);


    connect(m_animationGroup, &QParallelAnimationGroup::finished, this, [this]() {
        if (scene()) {
            scene()->removeItem(this);
        }
        delete this;
    });
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
    this->setPos(this->x(), m_yPos);
    emit yPosChanged();
}
void Mole::startAnimation()
{
    m_yAnimation->setStartValue(m_originalY);
    m_yAnimation->setEndValue(m_originalY - 200);

    m_animationGroup->start();
}
void Mole::stopAnimation()
{
    m_animationGroup->stop();

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
    emit moleHit();
    if (scene()) {
        scene()->removeItem(this);
    }
    delete this;
}

