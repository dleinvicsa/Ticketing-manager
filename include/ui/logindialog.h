#pragma once
#include <QDialog>
#include "auth/AuthService.h"
#include "core/Database.hpp"
class QLineEdit;
class QPushButton;
class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    LoginDialog(Database *db, QWidget *parent = nullptr);
    int getUserId() const;
private:
    AuthService auth;
    Database *db;
    QLineEdit *loginEdit;
    QLineEdit *passwordEdit;
    int userId = -1;
};
