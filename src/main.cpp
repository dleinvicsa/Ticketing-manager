#include <QApplication>
#include "ui/mainwindow.h"
#include "ui/logindialog.h"
#include "core/Database.hpp"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyleSheet(R"(
        QWidget {
            background-color: #121212;
            color: white;
            font-family: Arial;
            font-size: 14px;
        }
        QPushButton {
            background-color: #0078d4;
            color: white;
            border-radius: 6px;
            padding: 6px;
        }
        QPushButton:hover {
            background-color: #005fa3;
        }
        QLineEdit {
            background-color: #1e1e1e;
            border: 1px solid #333;
            padding: 5px;
            border-radius: 5px;
            color: white;
        }
        QLabel {
            color: white;
        }
    )");
    Database db("js/data/users.json", "js/data/tickets.json");
    while (true) {
        LoginDialog login(&db);
        if (login.exec() == QDialog::Accepted) {
            MainWindow window(login.getUserId());
            window.setWindowTitle("Ticketing Manager");
            window.resize(450, 600);
            window.show();
            if (app.exec() == 0) {
                continue; 
            } else {
                break;
            }
        } else {
            break; 
        }
    }
    return 0;
}
