#include "ticketdialog.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include "core/BookingManager.hpp"
TicketDialog::TicketDialog(Database *db, int userId, QWidget *parent)
    : QDialog(parent), db(db), userId(userId)
{
    setWindowTitle("My Tickets");
    resize(700, 400);
    QVBoxLayout *layout = new QVBoxLayout(this);
    table = new QTableWidget();
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({
        "Type", "Info", "Price", "Valid Until", "Action"
    });
    table->setColumnWidth(3, 180);
    table->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(table);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *deleteUsedBtn = new QPushButton("Delete Used Tickets");
    deleteUsedBtn->setStyleSheet("background-color: #d9534f; color: white; font-weight: bold; padding: 5px;");
    QPushButton *closeBtn = new QPushButton("Close");
    btnLayout->addWidget(deleteUsedBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(closeBtn);
    layout->addLayout(btnLayout);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(deleteUsedBtn, &QPushButton::clicked, this, [=]() {
        auto tickets = db->ticketsForUser(userId);
        int count = 0;
        for (const auto& t : tickets) {
            if (t.isUsed) {
                db->removeTicket(t.ticketId);
                count++;
            }
        }
        if (count > 0) {
            QMessageBox::information(this, "Success", QString("Deleted %1 used tickets").arg(count));
            loadTickets();
        } else {
            QMessageBox::information(this, "Info", "No used tickets to delete");
        }
    });
    loadTickets();
}
void TicketDialog::loadTickets()
{
    db->reload();
    auto tickets = db->ticketsForUser(userId);
    table->setRowCount(tickets.size());
    for (int i = 0; i < (int)tickets.size(); ++i) {
        QString fullInfo = QString::fromStdString(tickets[i].vehicleInfo);
        QString typeDesc = "General";
        QString routeInfo = fullInfo;
        int start = fullInfo.indexOf("[");
        int end = fullInfo.indexOf("]");
        if (start != -1 && end != -1) {
            typeDesc = fullInfo.mid(start + 1, end - start - 1);
            routeInfo = fullInfo.left(start).trimmed();
        } else if (fullInfo.contains("Train") || fullInfo.contains("VIVI")) {
            typeDesc = "Train";
        }
        table->setItem(i, 0, new QTableWidgetItem(typeDesc));
        table->setItem(i, 1, new QTableWidgetItem(routeInfo));
        table->setItem(i, 2, new QTableWidgetItem("€" + QString::number(tickets[i].price, 'f', 2)));
        QString validText = tickets[i].validUntil.empty() ? "Not used" : QString::fromStdString(tickets[i].validUntil);
        table->setItem(i, 3, new QTableWidgetItem(validText));
        if (tickets[i].isUsed) {
            QPushButton *active = new QPushButton("Active");
            active->setEnabled(false);
            table->setCellWidget(i, 4, active);
        } else {
            QPushButton *useBtn = new QPushButton("Use");
            std::string tId = tickets[i].ticketId;
            connect(useBtn, &QPushButton::clicked, this, [=]() {
                bool isTrain = fullInfo.contains("Train") || fullInfo.contains("VIVI");
                QString transport = "";
                if (!isTrain) {
                    bool ok;
                    transport = QInputDialog::getText(this, "Transport", "Enter transport number:", QLineEdit::Normal, "", &ok);
                    if (!ok || transport.isEmpty()) return;
                } else {
                    transport = "VIVI Train";
                }
                BookingManager bm(*db);
                if (bm.useTicket(tId)) {
                    QMessageBox::information(this, "Success", isTrain ? "Train ticket activated" : "Activated on: " + transport);
                    loadTickets();
                } else {
                    QMessageBox::warning(this, "Error", "Failed to activate ticket");
                }
            });
            table->setCellWidget(i, 4, useBtn);
        }
    }
}
