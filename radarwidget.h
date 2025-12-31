#ifndef RADARWIDGET_H
#define RADARWIDGET_H
#define MAX_DISTANCE 30   // cm

#include <QWidget>
#include <QVector>
#include <QPointF>
#include <QFile>
#include <QTextStream>
struct RadarPoint {
    QPointF pos;
    int life;   // fade için
};

class RadarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RadarWidget(QWidget *parent = nullptr);
     ~RadarWidget();
    void updateData(int angle, int distance);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_angle;
    QVector<RadarPoint> points;

    int currentAngle = 0;
    int currentDistance = 0;
    bool targetVisible = false;
    QFile logFile;
    QTextStream logStream;
};

#endif
