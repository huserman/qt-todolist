#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext> // To expose the object to QML
#include <QFileInfo>

#include "todolist.h"
#include "todomodel.h"
#include "dbhelper.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    //This function registers the C++ model with qml type system
    qmlRegisterType<TodoModel>("Todo", 1, 0, "TodoModel");
    qmlRegisterUncreatableType<TodoList>("Todo", 1, 0, "TodoList",
                                         QStringLiteral("TodoList should not be created in QML"));

    QFileInfo db_file(connectionString);
    DbHelper dbh;
    if (db_file.exists() && db_file.isFile())
    {
        dbh.ConnectToDb();
    }
    else
    {
        qDebug() << "Database file not found. Please check the connection string.";
        return 0;
    }

    TodoList todoList;

    QQmlApplicationEngine engine;

    //This is where todolist is actually exposed to qml as a context property (run qmake)
    engine.rootContext()->setContextProperty(QStringLiteral("todoList"), &todoList);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
