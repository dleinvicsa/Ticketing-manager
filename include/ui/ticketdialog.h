#pragma once
#include <QDialog>
#include "core/Database.hpp"
class QTableWidget;
class TicketDialog : public QDialog
{
    Q_OBJECT
public:
    TicketDialog(Database *db, int userId, QWidget *parent = nullptr);
private:
    void loadTickets();
    Database *db;
    int userId;
    QTableWidget *table;
};
