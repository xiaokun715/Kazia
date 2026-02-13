#include <QApplication>
#include <QFile>
#include "ui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 加载 QSS 样式表
    QFile styleFile("src/ui/styles.qss");
    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString styleSheet = styleFile.readAll();
        app.setStyleSheet(styleSheet);
        styleFile.close();
    }
    
    MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
}
