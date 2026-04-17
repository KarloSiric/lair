#include <QApplication>

#include "ui/qt/main_window.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    lair::ui::qt::MainWindow window;
    window.show();

    return app.exec();
}
