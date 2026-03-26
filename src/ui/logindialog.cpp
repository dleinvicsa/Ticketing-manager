#include "logindialog.h"
#include "registerdialog.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
LoginDialog::LoginDialog(Database *db, QWidget *parent)
    : QDialog(parent), db(db)
{
    setWindowTitle("Login");
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(10);
    layout->setContentsMargins(40, 40, 40, 40);
    QLabel *title = new QLabel("Login");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    loginEdit = new QLineEdit();
    loginEdit->setPlaceholderText("Login");
    passwordEdit = new QLineEdit();
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    QPushButton *loginBtn = new QPushButton("Login");
    QPushButton *registerBtn = new QPushButton("Create account");
    layout->addWidget(title);
    layout->addWidget(loginEdit);
    layout->addWidget(passwordEdit);
    layout->addWidget(loginBtn);
    layout->addWidget(registerBtn);
    connect(loginBtn, &QPushButton::clicked, this, [=]() {
        auth.loadUsers("js/data/users.json");
        std::string login = loginEdit->text().toStdString();
        std::string password = passwordEdit->text().toStdString();
        if (auth.login(login, password)) {
            User* user = db->findUserById(
                auth.findUserByLogin(login)->id
                );
            if (user) {
                userId = user->id;
                accept();
            }
        }
    });
    connect(registerBtn, &QPushButton::clicked, this, [=]() {
        RegisterDialog reg(db, this);
        reg.exec();
    });
}
int LoginDialog::getUserId() const {
    return userId;
}
