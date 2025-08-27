#ifndef MYVIEW_H
#define MYVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

class MyView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyView(QWidget *parent = nullptr) : QGraphicsView(parent) {
        setMouseTracking(true);
    }

signals:
    void mouseMoved(const QPoint &pos);

protected:
    void mouseMoveEvent(QMouseEvent *event) override {
        emit mouseMoved(event->pos());
        QGraphicsView::mouseMoveEvent(event);
    }
};

#endif // MYVIEW_H
