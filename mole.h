#ifndef MOLE_H
#define MOLE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>

class Mole: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    Q_PROPERTY(qreal scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleFactorChanged)
    Q_PROPERTY(qreal yPos READ yPos WRITE setYPos NOTIFY yPosChanged)
public:
   explicit Mole(const QPixmap &pixmap, QObject *parent = nullptr);

    qreal scaleFactor() const;
    void setScaleFactor(qreal newScaleFactor);

    qreal yPos() const;
    void setYPos(qreal newYPos);
    void setOriginalY(qreal);

    void startAnimation();
    void stopAnimation();

signals:
    void scaleFactorChanged();
    void yPosChanged();
    void animationFinished();
    void moleHit();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
 QPainterPath shape() const override;
private:
    QPropertyAnimation *m_scaleAnimation;
    QPropertyAnimation *m_yAnimation;
    QParallelAnimationGroup *m_animationGroup;

    qreal m_scaleFactor;
    qreal m_yPos;
    qreal m_originalY;

};

#endif // MOLE_H
