#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QScrollBar>
#include <QTimer>
#include <QStyle>
#include <QPainter>
#include <QPaintEvent>

class CustomGraphicsView : public QGraphicsView {
    Q_OBJECT
public:
    explicit CustomGraphicsView(QWidget *parent = nullptr)
        : QGraphicsView(parent), isDragging(false), autoFitEnabled(false)
    {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        setStyleSheet("background-color: rgb(0, 0, 0);");

        setTransformationAnchor(AnchorUnderMouse);

        setRenderHint(QPainter::Antialiasing);

        setRenderHint(QPainter::SmoothPixmapTransform);

        setViewportUpdateMode(FullViewportUpdate);

        setCursor(Qt::ArrowCursor);

        currentScale = 1.0;
    }

    void fitToView() {
        if (!scene() || scene()->items().isEmpty()) return;

        QRectF sceneRect = scene()->sceneRect();
        if (sceneRect.isEmpty()) sceneRect = scene()->itemsBoundingRect();
        if (sceneRect.isEmpty()) return;

        QPointF viewCenter = mapToScene(viewport()->rect().center());

        resetTransform();

        fitInView(sceneRect, Qt::KeepAspectRatio);

        QTransform currentTransform = transform();
        currentScale = currentTransform.m11();

        centerOn(viewCenter);
    }

protected:
    void wheelEvent(QWheelEvent *event) override {

        double scaleFactor = (event->angleDelta().y() > 0) ? 1.2 : 1/1.2;

        scale(scaleFactor, scaleFactor);

        QTransform currentTransform = transform();
        currentScale = currentTransform.m11();

        event->accept();
    }

    void mouseDoubleClickEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {

            QPointF scenePos = mapToScene(event->pos());

            fitToView();

            centerOn(scenePos);

            event->accept();
            return;
        }
        QGraphicsView::mouseDoubleClickEvent(event);
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {

            lastDragPos = event->pos();

            isDragging = true;

            setCursor(Qt::ClosedHandCursor);

            event->accept();
            return;
        }
        QGraphicsView::mousePressEvent(event);
    }

    void mouseMoveEvent(QMouseEvent *event) override {

        if (isDragging && (event->buttons() & Qt::LeftButton)) {

            QPoint delta = event->pos() - lastDragPos;
            lastDragPos = event->pos();

            horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
            verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());

            event->accept();
            return;
        }
        QGraphicsView::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton && isDragging) {

            isDragging = false;

            setCursor(Qt::ArrowCursor);

            event->accept();
            return;
        }
        QGraphicsView::mouseReleaseEvent(event);
    }

    void resizeEvent(QResizeEvent *event) override {
        QGraphicsView::resizeEvent(event);
        if (autoFitEnabled) {
            QTimer::singleShot(50, this, &CustomGraphicsView::fitToView);
        }
    }

    void drawBackground(QPainter *painter, const QRectF &rect) override {
        painter->fillRect(rect, Qt::black);
    }

    void paintEvent(QPaintEvent *event) override {
        QPainter painter(viewport());
        painter.fillRect(viewport()->rect(), Qt::black);
        painter.end();

        QGraphicsView::paintEvent(event);
    }

    void showEvent(QShowEvent *event) override {
        QGraphicsView::showEvent(event);
        viewport()->update();
    }

public slots:
    void setAutoFitEnabled(bool enabled) {
        autoFitEnabled = enabled;
        if (enabled) fitToView();
    }

private:
    double currentScale;   
    QPoint lastDragPos; 
    bool isDragging;  
    bool autoFitEnabled;  
};

#endif // CUSTOMGRAPHICSVIEW_H
