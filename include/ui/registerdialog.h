#pragma once
#include <QDialog>
#include "auth/AuthService.h"
#include "core/Database.hpp"
class QLineEdit;
class RegisterDialog : public QDialog
{
    Q_OBJECT
public:
    RegisterDialog(Database *db, QWidget *parent = nullptr);
private:
    AuthService auth;
    Database *db;
    QLineEdit *loginEdit;
    QLineEdit *passwordEdit;
};
