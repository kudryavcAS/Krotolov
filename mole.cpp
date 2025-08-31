#include "mole.h"
#include <QGraphicsScene>

Mole::Mole(const QPixmap &pixmap, QObject *parent)
    : QObject(parent)
    , QGraphicsPixmapItem(pixmap)

{
    originalY = 0;
    yPos = originalY;

    scaleFactor = 1.0;
    setTransformOriginPoint(boundingRect().center());
    setFlag(QGraphicsItem::ItemIsSelectable);
    setZValue(0);

    scaleAnimation = new QPropertyAnimation(this, "scaleFactor", this);
    scaleAnimation->setDuration(750);
    scaleAnimation->setStartValue(0.7);
    scaleAnimation->setEndValue(1.3);
    scaleAnimation->setEasingCurve(QEasingCurve::InOutSine);

    yAnimation = new QPropertyAnimation(this, "yPos", this);
    yAnimation->setDuration(750);
    yAnimation->setEasingCurve(QEasingCurve::InOutSine);

    animationGroup = new QParallelAnimationGroup(this);
    animationGroup->addAnimation(scaleAnimation);
    animationGroup->addAnimation(yAnimation);

    connect(animationGroup, &QParallelAnimationGroup::finished, this, [this]() {
        if (scene()) {
            scene()->removeItem(this);
        }
        delete this;
    });
}

double Mole::getScaleFactor() const
{
    return scaleFactor;
}
void Mole::setScaleFactor(double newScaleFactor)
{
    if (qFuzzyCompare(scaleFactor, newScaleFactor))
        return;
    scaleFactor = newScaleFactor;
    this->setScale(scaleFactor);
    emit scaleFactorChanged();
}
double Mole::getYPos() const
{
    return yPos;
}
void Mole::setYPos(double newYPos)
{
    if (qFuzzyCompare(yPos, newYPos))
        return;
    yPos = newYPos;
    this->setPos(this->x(), yPos);
    emit yPosChanged();
}
void Mole::startAnimation()
{
    yAnimation->setStartValue(originalY);
    yAnimation->setEndValue(originalY - 200);

    animationGroup->start();
}
void Mole::stopAnimation()
{
    animationGroup->stop();

    setScaleFactor(1.0);
    setYPos(originalY);
}
void Mole::setOriginalY(double y)
{
    originalY = y;
    yPos = y;
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

QPainterPath Mole::shape() const
{
    QPainterPath path;
    QRectF rect = boundingRect();

    QRectF biggerRect = rect.adjusted(
        -rect.width() * 0.25,
        -rect.height() * 0.25,
        rect.width() * 0.25,
        rect.height() * 0.25
        );

    path.addRect(biggerRect);
    return path;
}
