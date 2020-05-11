#include "mainwindow.h"

#include "image.h"

#include <QDebug>

#include <QAction>
#include <QFileDialog>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QPainter>
#include <QScrollArea>

MainWindow::MainWindow(QWidget * parent) :
        QMainWindow(parent)
{
        mFileMenu = new QMenu(tr("File"));
        mOpenAction = new QAction(tr("Open..."));

        mScrollArea = new QScrollArea(this);
        mLabel = new QLabel(mScrollArea);

        mFileMenu->addAction(mOpenAction);
        menuBar()->addMenu(mFileMenu);

        mScrollArea->setWidget(mLabel);
        setCentralWidget(mScrollArea);

        connect(mOpenAction, SIGNAL(triggered(bool)), this, SLOT(open()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::open()
{
        QString file = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.bmp)"));
        Image red(file);
        Image green(file);
        Image blue(file);
        Image yellow(file);
        Image edges(file);
        Image output(file);

        red.red();
        red.edges(15, &edges);
        edges.dilate(1);
        red.unite(edges);
        QVector<Segment> segmentsR = red.segmentate();

        green.green();
        green.edges(15, &edges);
        edges.dilate(1);
        green.unite(edges);
        QVector<Segment> segmentsG = green.segmentate();

        blue.blue();
        blue.edges(15, &edges);
        edges.dilate(1);
        blue.unite(edges);
        QVector<Segment> segmentsB = blue.segmentate();

        yellow.yellow();
        yellow.edges(15, &edges);
        edges.dilate(1);
        yellow.unite(edges);
        QVector<Segment> segmentsY = yellow.segmentate();

        QVector<Segment> segments;

        foreach (Segment segment, segmentsR)
        {
                qreal W3 = segment.perimeter * segment.perimeter / (qreal) segment.area;
                if(W3 > 41.0 && W3 < 60.0)
                {
                        QPainter painter(&output);
                        painter.setPen(Qt::red);
                        painter.drawRect(segment.rectangle);

                        segment.rectangle.adjust(-5, -5, 5, 5);
                        segments.append(segment);
                }
        }

        foreach (Segment segment, segmentsG)
        {
                qreal W3 = segment.perimeter * segment.perimeter / (qreal) segment.area;
                if(W3 > 30.0 && W3 < 40.0)
                {
                        QPainter painter(&output);
                        painter.setPen(Qt::green);
                        painter.drawRect(segment.rectangle);

                        segment.rectangle.adjust(-5, -5, 5, 5);
                        segments.append(segment);
                }
        }

        foreach (Segment segment, segmentsB)
        {
                qreal W3 = segment.perimeter * segment.perimeter / (qreal) segment.area;
                if(W3 > 60.0 && W3 < 66.0 || W3 > 68.0 && W3 < 75.0)
                {
                        QPainter painter(&output);
                        painter.setPen(Qt::blue);
                        painter.drawRect(segment.rectangle);

                        segment.rectangle.adjust(-5, -5, 5, 5);
                        segments.append(segment);
                }
        }

        foreach (Segment segment, segmentsY)
        {
                qreal W3 = segment.perimeter * segment.perimeter / (qreal) segment.area;
                if(W3 > 35.0 && W3 < 50.0)
                {
                        QPainter painter(&output);
                        painter.setPen(Qt::yellow);
                        painter.drawRect(segment.rectangle);

                        segment.rectangle.adjust(-5, -5, 5, 5);
                        segments.append(segment);
                }
        }

        int left = INT_MAX;
        int right = INT_MIN;
        int top = INT_MAX;
        int bottom = INT_MIN;
        foreach (Segment segmentA, segments)
        {
                foreach (Segment segmentB, segments)
                {

                        if(segmentA.rectangle != segmentB.rectangle && segmentA.rectangle.intersects(segmentB.rectangle))
                        {
                                if(segmentA.rectangle.left() < left) left = segmentA.rectangle.left();
                                if(segmentA.rectangle.right() > right) right = segmentA.rectangle.right();
                                if(segmentA.rectangle.top() < top) top = segmentA.rectangle.top();
                                if(segmentA.rectangle.bottom() > bottom) bottom = segmentA.rectangle.bottom();
                        }
                }
        }

        QPainter painter(&output);
        painter.setPen(Qt::black);
        painter.drawRect(QRect(QPoint(left, top), QPoint(right, bottom)));

        mLabel->setPixmap(QPixmap::fromImage(output));
        mLabel->resize(blue.width(), blue.height());
}
