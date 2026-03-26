#include "bookingdialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
BookingDialog::BookingDialog(QString route, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Book Ticket");
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel("Confirm booking:\n" + route);
    layout->addWidget(label);
    QPushButton *confirmBtn = new QPushButton("Confirm");
    QPushButton *cancelBtn = new QPushButton("Cancel");
    layout->addWidget(confirmBtn);
    layout->addWidget(cancelBtn);
    connect(confirmBtn, &QPushButton::clicked, this, [=]() {
        confirmed = true;
        accept();
    });
    connect(cancelBtn, &QPushButton::clicked, this, [=]() {
        reject();
    });
}
bool BookingDialog::isConfirmed() const
{
    return confirmed;
}
