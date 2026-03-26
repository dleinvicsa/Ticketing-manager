#include "registerdialog.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
RegisterDialog::RegisterDialog(Database *db, QWidget *parent)
    : QDialog(parent), db(db)
{
    setWindowTitle("Register");
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(10);
    layout->setContentsMargins(40, 40, 40, 40);
    QLabel *title = new QLabel("Create Account");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    loginEdit = new QLineEdit();
    loginEdit->setPlaceholderText("Login");
    passwordEdit = new QLineEdit();
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    QPushButton *btn = new QPushButton("Register");
    layout->addWidget(title);
    layout->addWidget(loginEdit);
    layout->addWidget(passwordEdit);
    layout->addWidget(btn);
    connect(btn, &QPushButton::clicked, this, [=]() {
        auth.loadUsers("js/data/users.json");
        std::string login = loginEdit->text().toStdString();
        std::string password = passwordEdit->text().toStdString();
        if (auth.registerUser(login, password)) {
            db->reload();
            accept();
        }
    });
}
