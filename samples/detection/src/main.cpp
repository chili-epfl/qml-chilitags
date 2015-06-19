#include <QGuiApplication>
#include <QQuickView>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QQuickView view;
    QSurfaceFormat format;
    format.setMajorVersion(3);
    //format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    //view.setFormat(format);
    view.resize(640, 480);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:///src/main.qml"));
    view.show();

    return app.exec();
}
