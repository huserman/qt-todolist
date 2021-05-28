![GUI Todo List](https://github.com/huserman/qt-todolist/blob/main/app-screenshot.png?raw=true)
TodoList is a simple Qt/QML application written on Qt 6.0.3 with qmake for learning purposes
Initial structure is taken from https://doc.qt.io/qt-5/qtquick-modelviewsdata-cppmodels.html
Added database SQLite3 for saving/updating/removing tasks

DB file: todolist.db
Connection string: dbhelper.h
DB model class: dbhelper.h dbhelper.cpp
