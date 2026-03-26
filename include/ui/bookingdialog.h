#pragma once
#include <QDialog>
class QPushButton;
class QLabel;
class BookingDialog : public QDialog
{
    Q_OBJECT
public:
    BookingDialog(QString route, QWidget *parent = nullptr);
    bool isConfirmed() const;
private:
    bool confirmed = false;
};
