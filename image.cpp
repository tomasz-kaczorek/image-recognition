#include "image.h"

#include "segment.h"

#include <QDebug>
#include <QThread>

#include <QQueue>
#include <QtMath>

static QRgb const black = qRgb(0, 0, 0);
static QRgb const white = qRgb(255, 255, 255);

Image::Image(const QImage & image) :
        QImage(image)
{
}

Image::Image(const QString & file) :
        QImage(file)
{
}

QVector<Segment> Image::segmentate()
{
        QVector<Segment> segments;
        QQueue<QPoint> queue;

        QImage temp(size(), format());
        temp.fill(black);

        for(int x = 1; x < width() - 1; ++x)
        {
                for(int y = 1; y < height() - 1; ++y)
                {
                        if(pixel(x, y) != black && temp.pixel(x, y) == black)
                        {
                                queue.enqueue(QPoint(x, y));
                                segments.append(Segment());
                                while(!queue.isEmpty())
                                {
                                        QPoint p = queue.dequeue();
                                        QPoint west = QPoint(p.x() - 1, p.y());
                                        QPoint east = QPoint(p.x() + 1, p.y());
                                        QPoint north = QPoint(p.x(), p.y() - 1);
                                        QPoint south = QPoint(p.x(), p.y() + 1);
                                        if(temp.pixel(p) == black)
                                        {
                                                bool border = false;
                                                temp.setPixel(p, white);
                                                if(pixel(west) != black)
                                                {
                                                        queue.enqueue(west);
                                                }
                                                else
                                                {
                                                        border = true;
                                                }
                                                if(pixel(east) != black)
                                                {
                                                        queue.enqueue(east);
                                                }
                                                else
                                                {
                                                        border = true;
                                                }
                                                if(pixel(north) != black)
                                                {
                                                        queue.enqueue(north);
                                                }
                                                else
                                                {
                                                        border = true;
                                                }
                                                if(pixel(south) != black)
                                                {
                                                        queue.enqueue(south);
                                                }
                                                else
                                                {
                                                        border = true;
                                                }
                                                ++segments.last().area;
                                                if(border)
                                                {
                                                        ++segments.last().perimeter;
                                                }
                                                if(p.x() < segments.last().rectangle.left())
                                                {
                                                        segments.last().rectangle.setLeft(p.x());
                                                }
                                                if(p.x() > segments.last().rectangle.right())
                                                {
                                                        segments.last().rectangle.setRight(p.x());
                                                }
                                                if(p.y() < segments.last().rectangle.top())
                                                {
                                                        segments.last().rectangle.setTop(p.y());
                                                }
                                                if(p.y() > segments.last().rectangle.bottom())
                                                {
                                                        segments.last().rectangle.setBottom(p.y());
                                                }
                                        }
                                }
                        }
                }
        }
        return segments;
}

void Image::add(int term)
{
        if(term < 0)
        {
                for(int x = 0; x < width(); ++x)
                {
                        for(int y = 0; y < height(); ++y)
                        {
                                QRgb p = pixel(x, y);
                                int red = qMax(qRed(p) - term, 0);
                                int green = qMax(qGreen(p) - term, 0);
                                int blue = qMax(qBlue(p) - term, 0);
                                setPixel(x, y, qRgb(red, green, blue));
                        }
                }
        }
        else if(term > 0)
        {
                for(int x = 0; x < width(); ++x)
                {
                        for(int y = 0; y < height(); ++y)
                        {
                                QRgb p = pixel(x, y);
                                int red = qMax(qRed(p) + term, 255);
                                int green = qMax(qGreen(p) + term, 255);
                                int blue = qMax(qBlue(p) + term, 255);
                                setPixel(x, y, qRgb(red, green, blue));
                        }
                }
        }
}

void Image::multiply(qreal factor)
{
        if(factor < 1.0)
        {
                for(int x = 0; x < width(); ++x)
                {
                        for(int y = 0; y < height(); ++y)
                        {
                                QRgb p = pixel(x, y);
                                int red = qMax(qRed(p) * factor, 0.0);
                                int green = qMax(qGreen(p) * factor, 0.0);
                                int blue = qMax(qBlue(p) * factor, 0.0);
                                setPixel(x, y, qRgb(red, green, blue));
                        }
                }
        }
        else if(factor > 1.0)
        {
                for(int x = 0; x < width(); ++x)
                {
                        for(int y = 0; y < height(); ++y)
                        {
                                QRgb p = pixel(x, y);
                                int red = qMax(qRed(p) * factor, 255.0);
                                int green = qMax(qGreen(p) * factor, 255.0);
                                int blue = qMax(qBlue(p) * factor, 255.0);
                                setPixel(x, y, qRgb(red, green, blue));
                        }
                }
        }

}

void Image::red()
{
        for(int x = 0; x < width(); ++x)
        {
                for(int y = 0; y < height(); ++y)
                {
                        QRgb p = pixel(x, y);
                        int red = qRed(p);
                        int green = qGreen(p);
                        int blue = qBlue(p);
                        if(red < 130 || red < 2.5 * green || red < 2 * blue || green > 2 * blue)
                        {
                                setPixel(x, y, black);
                        }
                }
        }
}

void Image::green()
{
        for(int x = 0; x < width(); ++x)
        {
                for(int y = 0; y < height(); ++y)
                {
                        QRgb p = pixel(x, y);
                        int red = qRed(p);
                        int green = qGreen(p);
                        int blue = qBlue(p);
                        if(green < 50 || green < 1.5 * red || green < 1.5 * blue)
                        {
                                setPixel(x, y, black);
                        }
                }
        }
}

void Image::blue()
{
        for(int x = 0; x < width(); ++x)
        {
                for(int y = 0; y < height(); ++y)
                {
                        QRgb p = pixel(x, y);
                        int red = qRed(p);
                        int green = qGreen(p);
                        int blue = qBlue(p);
                        if(blue < 50 || blue < 1.75 * red || blue < 1.25 * green)
                        {
                                setPixel(x, y, black);
                        }
                }
        }
}

void Image::yellow()
{
        for(int x = 0; x < width(); ++x)
        {
                for(int y = 0; y < height(); ++y)
                {
                        QRgb p = pixel(x, y);
                        int red = qRed(p);
                        int green = qGreen(p);
                        int blue = qBlue(p);
                        if(red < 100 || green < 100 || red < 2 * blue || green < 2 * blue)
                        {
                                setPixel(x, y, black);
                        }
                }
        }

}

void Image::greyscale()
{
        for(int x = 0; x < width(); ++x)
        {
                for(int y = 0; y < height(); ++y)
                {
                        setPixel(x, y, qGray(pixel(x, y)));
                }
        }
}

void Image::blur(int radius, QImage * output)
{
        QImage const & input = output ? *this : (output = this, copy());
        int area = (1 + 2 * radius) * (1 + 2 * radius);
        for(int x = radius; x < width() - radius; ++x)
        {
                for(int y = radius; y < height() - radius; ++y)
                {
                        int red = 0;
                        int green = 0;
                        int blue = 0;
                        for(int rx = x - radius; rx <= x + radius; ++rx)
                        {
                                for(int ry = y - radius; ry <= y + radius; ++ry)
                                {
                                        QRgb p = input.pixel(rx, ry);
                                        red += qRed(p);
                                        green += qGreen(p);
                                        blue += qBlue(p);
                                }
                        }
                        output->setPixel(x, y, qRgb(red / area, green / area, blue / area));
                }
        }
}

void Image::erode(int radius, QImage * output)
{
        QImage const & input = output ? *this : (output = this, copy());
        for(int x = radius; x < width() - radius; ++x)
        {
                for(int y = radius; y < height() - radius; ++y)
                {
                        for(int rx = x - radius; rx <= x + radius; ++rx)
                        {
                                for(int ry = y - radius; ry <= y + radius; ++ry)
                                {
                                        QRgb p = input.pixel(rx, ry);
                                        if(qGray(p) < qGray(input.pixel(x, y)))
                                        {
                                                output->setPixel(x, y, p);
                                        }
                                }
                        }
                }
        }
}

void Image::dilate(int radius, QImage * output)
{
        QImage const & input = output ? *this : (output = this, copy());
        for(int x = radius; x < width() - radius; ++x)
        {
                for(int y = radius; y < height() - radius; ++y)
                {
                        for(int rx = x - radius; rx <= x + radius; ++rx)
                        {
                                for(int ry = y - radius; ry <= y + radius; ++ry)
                                {
                                        QRgb p = input.pixel(rx, ry);
                                        if(qGray(p) > qGray(input.pixel(x, y)))
                                        {
                                                output->setPixel(x, y, p);
                                        }
                                }
                        }
                }
        }
}

void Image::edges(int treshold, QImage * output)
{
        QImage const & input = output ? *this : (output = this, copy());
        for(int x = 0; x < width(); ++x)
        {
                output->setPixel(x, height() - 1, input.pixel(x, height() - 1));
        }
        for(int y = 0; y < height(); ++y)
        {
                output->setPixel(width() - 1, y, input.pixel(width() - 1, y));
        }
        for(int x = 0; x < width() - 1; ++x)
        {
                for(int y = 0; y < height() - 1; ++y)
                {
                        int ul = qGray(input.pixel(x, y));
                        int ur = qGray(input.pixel(x + 1, y));
                        int bl = qGray(input.pixel(x, y + 1));
                        int br = qGray(input.pixel(x + 1, y + 1));
                        int gradient = qSqrt((ul - br) * (ul - br) + (ur - bl) * (ur - bl));
                        output->setPixel(x, y, gradient > treshold ? white : black);
                }
        }
}

void Image::unite(const QImage & other, QImage * output)
{
        QImage const & input = output ? *this : (output = this, copy());
        for(int x = 0; x < width(); ++x)
        {
                for(int y = 0; y < height(); ++y)
                {
                        output->setPixel(x, y, (input.pixel(x, y) != black || other.pixel(x, y) != black) ? white : black);
                }
        }
}

void Image::complement(const QImage & other, QImage * output)
{
        QImage const & input = output ? *this : (output = this, copy());
        for(int x = 0; x < width(); ++x)
        {
                for(int y = 0; y < height(); ++y)
                {
                        output->setPixel(x, y, (input.pixel(x, y) != black && other.pixel(x, y) == black) ? white : black);
                }
        }
}
