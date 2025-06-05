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

        setStyleSheet("background-color: rgb(0, 0, 0);");

        // 设置缩放以鼠标为中心
        setTransformationAnchor(AnchorUnderMouse);

        // 启用平滑渲染
        setRenderHint(QPainter::Antialiasing);
        setRenderHint(QPainter::SmoothPixmapTransform);

        // 设置视口更新模式
        setViewportUpdateMode(FullViewportUpdate);

        // 设置默认光标
        setCursor(Qt::ArrowCursor);

        // 初始化当前缩放因子
        currentScale = 1.0;
    }

    // 自适应窗口显示
    void fitToView() {
        if (!scene() || scene()->items().isEmpty()) return;

        // 获取场景的边界矩形
        QRectF sceneRect = scene()->sceneRect();
        if (sceneRect.isEmpty()) sceneRect = scene()->itemsBoundingRect();
        if (sceneRect.isEmpty()) return;

        // 保存当前视图中心点
        QPointF viewCenter = mapToScene(viewport()->rect().center());

        // 重置变换
        resetTransform();

        // 计算自适应缩放比例
        fitInView(sceneRect, Qt::KeepAspectRatio);

        // 更新当前缩放因子
        QTransform currentTransform = transform();
        currentScale = currentTransform.m11();

        // 恢复或调整视图中心
        centerOn(viewCenter);
    }

protected:
    // 鼠标滚轮事件 - 缩放
    void wheelEvent(QWheelEvent *event) override {
        // 计算缩放因子（上滚放大，下滚缩小）
        double scaleFactor = (event->angleDelta().y() > 0) ? 1.2 : 1/1.2;

        // 应用缩放
        scale(scaleFactor, scaleFactor);

        // 更新当前缩放因子
        QTransform currentTransform = transform();
        currentScale = currentTransform.m11();

        event->accept();
    }

    // 鼠标双击事件 - 自适应窗口显示
    void mouseDoubleClickEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            // 获取当前点击位置
            QPointF scenePos = mapToScene(event->pos());

            // 执行自适应
            fitToView();

            // 将点击位置置于中心
            centerOn(scenePos);

            event->accept();
            return;
        }
        QGraphicsView::mouseDoubleClickEvent(event);
    }

    // 鼠标按下事件 - 开始拖拽
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            // 保存点击位置以便拖拽计算
            lastDragPos = event->pos();

            // 设置拖拽状态
            isDragging = true;

            // 改变光标为手形
            setCursor(Qt::ClosedHandCursor);

            event->accept();
            return;
        }
        QGraphicsView::mousePressEvent(event);
    }

    // 鼠标移动事件 - 处理拖拽
    void mouseMoveEvent(QMouseEvent *event) override {
        // 处理左键拖动
        if (isDragging && (event->buttons() & Qt::LeftButton)) {
            // 计算移动距离
            QPoint delta = event->pos() - lastDragPos;
            lastDragPos = event->pos();

            // 调整视图滚动条位置
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
            verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());

            event->accept();
            return;
        }
        QGraphicsView::mouseMoveEvent(event);
    }

    // 鼠标释放事件 - 结束拖拽
    void mouseReleaseEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton && isDragging) {
            // 重置拖拽状态
            isDragging = false;

            // 恢复光标为箭头
            setCursor(Qt::ArrowCursor);

            event->accept();
            return;
        }
        QGraphicsView::mouseReleaseEvent(event);
    }

    // 窗口大小变化时自动调整视图
    void resizeEvent(QResizeEvent *event) override {
        QGraphicsView::resizeEvent(event);
        if (autoFitEnabled) {
            // 延迟自适应调整
            QTimer::singleShot(50, this, &CustomGraphicsView::fitToView);
        }
    }

    // 自定义绘图 - 确保背景为黑色
    void drawBackground(QPainter *painter, const QRectF &rect) override {
        painter->fillRect(rect, Qt::black);
    }

    // 重写绘制事件
    void paintEvent(QPaintEvent *event) override {
        // 方法5: 在绘制前填充背景
        QPainter painter(viewport());
        painter.fillRect(viewport()->rect(), Qt::black);
        painter.end();

        QGraphicsView::paintEvent(event);
    }

    // 显示事件
    void showEvent(QShowEvent *event) override {
        QGraphicsView::showEvent(event);
        // 确保视图刷新背景
        viewport()->update();
    }

public slots:
    // 设置是否启用窗口大小变化时自动调整
    void setAutoFitEnabled(bool enabled) {
        autoFitEnabled = enabled;
        if (enabled) fitToView();
    }

private:
    double currentScale;      // 当前缩放因子
    QPoint lastDragPos;       // 最后拖拽位置
    bool isDragging;          // 是否正在拖拽中
    bool autoFitEnabled;      // 是否启用窗口大小变化时自动调整
};

#endif // CUSTOMGRAPHICSVIEW_H
