#include "radarwidget.h"
#include <QPainter>
#include <QtMath>
#include <QTimer>
#include <QDateTime>
RadarWidget::RadarWidget(QWidget *parent)
    : QWidget(parent), m_angle(0)
{
    setMinimumSize(500, 300);
    // 📄 LOG DOSYASI
    logFile.setFileName("radar_log.txt");

    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        logStream.setDevice(&logFile);
        logStream << "\n--- RADAR LOG STARTED ---\n";
    }
    // ⏳ LIFE TIMER (NOKTA SİLME)
    QTimer *timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [this]() {

        // Tüm noktaların ömrünü azalt
        for (int i = 0; i < points.size(); i++)
            points[i].life -= 4;

        // Ömrü bitenleri sil
        points.erase(
            std::remove_if(points.begin(), points.end(),
                           [](const RadarPoint &rp){
                               return rp.life <= 0;
                           }),
            points.end());

        update();   // repaint
    });

    timer->start(50); // 50 ms → hız ayarı
}

void RadarWidget::updateData(int angle, int distance)
{
    m_angle = angle;

    // 🚫 30 cm üstünü ve hatalı ölçümleri yok say
    if (distance <= 0 || distance > MAX_DISTANCE) {
        update();    // sweep yine çizilsin
        return;
    }
    // 📄 LOG YAZ
    if (logFile.isOpen()) {
        logStream
            << QDateTime::currentDateTime().toString("hh:mm:ss.zzz")
            << " | Angle: " << angle
            << " deg | Distance: " << distance
            << " cm\n";
        logStream.flush(); // anında diske yaz
    }


    double rad = qDegreesToRadians((double)angle);

    double r = qMin(width(), height()) * 0.9
               * ((double)distance / MAX_DISTANCE);

    QPointF p;
    p.setX(width()/2 + r * cos(rad));
    p.setY(height() - r * sin(rad));

    // 🔴 SADECE GEÇERLİ MESAFE EKLENİR
    points.append({p, 255});

    update();
}
RadarWidget::~RadarWidget()
{
    if (logFile.isOpen())
        logFile.close();
}
void RadarWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), QColor(0, 10, 0));

    int cx = width() / 2;
    int cy = height();
    int radius = qMin(width(), height()) * 0.9;

    // Mesafe çemberleri
    p.setPen(QPen(QColor(0,255,0,120), 1));
    for (int r = radius/4; r <= radius; r += radius/4)
        p.drawEllipse(QPoint(cx, cy), r, r);

    // Açı çizgileri
    for (int a = 0; a <= 180; a += 30) {
        double rad = qDegreesToRadians((double)a);
        p.drawLine(cx, cy,
                   cx + radius * cos(rad),
                   cy - radius * sin(rad));
    }

    // Sweep ışını
    double sweepRad = qDegreesToRadians((double)m_angle);
    QPen sweepPen(QColor(0,255,0,180), 3);
    p.setPen(sweepPen);
    p.drawLine(cx, cy,
               cx + radius * cos(sweepRad),
               cy - radius * sin(sweepRad));

    // Echo noktaları
    for (const RadarPoint &rp : points) {

        // 🔴 life → alpha (silikleşme)
        int alpha = qBound(0, rp.life, 255);

        QColor fadeRed(255, 0, 0, alpha);
        p.setBrush(fadeRed);
        p.setPen(Qt::NoPen);

        p.drawEllipse(rp.pos, 4, 4);
    }

}
