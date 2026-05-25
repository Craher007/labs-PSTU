#include <QApplication>
#include <QFont>

#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setFont(QFont("Arial", 10));

    MainWindow window;
    window.show();

    return app.exec();
}
