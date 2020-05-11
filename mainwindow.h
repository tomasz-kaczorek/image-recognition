#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QLabel;
class QMenu;
class QScrollArea;

class MainWindow : public QMainWindow
{
                Q_OBJECT
        public:
                explicit MainWindow(QWidget * parent = 0);
                ~MainWindow();
        private:
                QMenu * mFileMenu;
                QAction * mOpenAction;
                QScrollArea * mScrollArea;
                QLabel * mLabel;
        public slots:
                void open();
};

#endif // MAINWINDOW_H
