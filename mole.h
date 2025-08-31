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

    Q_PROPERTY(double scaleFactor READ getScaleFactor WRITE setScaleFactor NOTIFY scaleFactorChanged)
    Q_PROPERTY(double yPos READ getYPos WRITE setYPos NOTIFY yPosChanged)
public:
   explicit Mole(const QPixmap &pixmap, QObject *parent = nullptr);

    double getScaleFactor() const;
    void setScaleFactor(double newScaleFactor);

    double getYPos() const;
    void setYPos(double newYPos);
    void setOriginalY(double);

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
    QPropertyAnimation *scaleAnimation;
    QPropertyAnimation *yAnimation;
    QParallelAnimationGroup *animationGroup;

    double scaleFactor;
    double yPos;
    double originalY;

};

#endif // MOLE_H
