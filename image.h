#ifndef IMAGE_H
#define IMAGE_H

#include "segment.h"

#include <QImage>
#include <QVector>

class Image : public QImage
{
        public:
                Image(QImage const & image);
                Image(QString const & file);

                QVector<Segment> segmentate();

                void add(int term);
                void multiply(qreal factor);

                void red();
                void green();
                void blue();
                void yellow();
                void greyscale();

                void blur(int radius, QImage * output = Q_NULLPTR);
                void erode(int radius, QImage * output = Q_NULLPTR);
                void dilate(int radius, QImage * output = Q_NULLPTR);

                void edges(int treshold, QImage * output = Q_NULLPTR);
                void unite(QImage const & other, QImage * output = Q_NULLPTR);
                void complement(QImage const & other, QImage * output = Q_NULLPTR);
};

#endif // IMAGE_H
