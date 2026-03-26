#pragma once
#include <QMainWindow>
#include <QLabel>
#include "core/BookingManager.hpp"
#include "core/Database.hpp"
#include "transport/Transport.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(int userId, QWidget *parent = nullptr);
    void updateBalance();
private:
    int currentUserId;
    BookingManager *bm;
    Database *db;
    Transport *train;
    Transport *bus;
    QLabel *balanceLabel;
};
