#include "mainwindow.h"
#include "ticketdialog.h"
#include "transport/VehicleType.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QScrollArea>
#include <QMessageBox>
#include <QInputDialog>
#include <QTableWidget>
#include <QHeaderView>
struct TicketOption {
    QString title;
    QString type;
    QString from;
    QString to;
    RigaTicketType rigaType;
};
MainWindow::MainWindow(int userId, QWidget *parent)
    : QMainWindow(parent), currentUserId(userId)
{
    db = new Database("js/data/users.json", "js/data/tickets.json");
    bm = new BookingManager(*db);
    train = new Transport(VehicleType::TRAIN, "VIVI");
    bus   = new Transport(VehicleType::BUS, "REGULAR");
    bm->addTrip(Trip("V_GENERIC", train, "Riga", "Any", "00:00", "VIVI"));
    bm->addTrip(Trip("R_GENERIC", bus, "Riga", "Riga", "00:00"));
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    QLabel *title = new QLabel("🎫 Ticket Manager");
    title->setStyleSheet("font-size: 24px; font-weight: bold;");
    mainLayout->addWidget(title);
    User* user = db->findUserById(currentUserId);
    balanceLabel = new QLabel(
        "Balance: €" + QString::number(user ? user->balance : 0.0, 'f', 2)
    );
    balanceLabel->setAlignment(Qt::AlignCenter);
    balanceLabel->setStyleSheet("font-size: 16px; color: lightgreen;");
    mainLayout->addWidget(balanceLabel);
    QPushButton *topUpBtn = new QPushButton("Top Up");
    QPushButton *myTicketsBtn = new QPushButton("My Tickets");
    QPushButton *logOutBtn = new QPushButton("Log Out");
    logOutBtn->setStyleSheet("background-color: #f0ad4e; color: white; font-weight: bold;");
    QPushButton *adminBtn = nullptr;
    mainLayout->addWidget(topUpBtn);
    mainLayout->addWidget(myTicketsBtn);
    mainLayout->addWidget(logOutBtn);
    connect(logOutBtn, &QPushButton::clicked, this, [=]() {
        QMessageBox::StandardButton res = QMessageBox::question(this, "Log Out", "Are you sure you want to log out?", QMessageBox::Yes | QMessageBox::No);
        if (res == QMessageBox::Yes) {
            this->close();
        }
    });
    if (user && user->role == "admin") {
        adminBtn = new QPushButton("Admin Panel");
        adminBtn->setStyleSheet("background-color: #5bc0de; color: white; font-weight: bold;");
        mainLayout->addWidget(adminBtn);
        connect(adminBtn, &QPushButton::clicked, this, [=]() {
            QDialog adminDialog(this);
            adminDialog.setWindowTitle("Admin Panel - Manage Users");
            adminDialog.resize(600, 400);
            QVBoxLayout *adminLayout = new QVBoxLayout(&adminDialog);
            QTableWidget *userTable = new QTableWidget(&adminDialog);
            userTable->setColumnCount(4);
            userTable->setHorizontalHeaderLabels({"ID", "Login", "Role", "Balance"});
            auto refreshUsers = [&]() {
                db->reload();
                auto users = db->allUsers();
                userTable->setRowCount(users.size());
                for (int i = 0; i < (int)users.size(); ++i) {
                    userTable->setItem(i, 0, new QTableWidgetItem(QString::number(users[i].id)));
                    userTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(users[i].login)));
                    userTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(users[i].role)));
                    userTable->setItem(i, 3, new QTableWidgetItem("€" + QString::number(users[i].balance, 'f', 2)));
                }
            };
            refreshUsers();
            adminLayout->addWidget(userTable);
            QHBoxLayout *editLayout = new QHBoxLayout();
            QPushButton *changeBalanceBtn = new QPushButton("Change Balance select", &adminDialog);
            QPushButton *deleteUserBtn = new QPushButton("Delete User selected", &adminDialog);
            deleteUserBtn->setStyleSheet("background-color: #d9534f; color: white;");
            editLayout->addWidget(changeBalanceBtn);
            editLayout->addWidget(deleteUserBtn);
            adminLayout->addLayout(editLayout);
            connect(changeBalanceBtn, &QPushButton::clicked, &adminDialog, [&]() {
                int row = userTable->currentRow();
                if (row < 0) {
                    QMessageBox::warning(&adminDialog, "Error", "Select a user first");
                    return;
                }
                int userId_sel = userTable->item(row, 0)->text().toInt();
                User* target = db->findUserById(userId_sel);
                if (!target) return;
                bool ok;
                double newBal = QInputDialog::getDouble(&adminDialog, "Balance", 
                    "Enter new balance for " + QString::fromStdString(target->login), 
                    target->balance, 0, 1000000, 2, &ok);
                if (ok) {
                    target->balance = newBal;
                    db->updateUser(*target);
                    refreshUsers();
                    if (target->id == currentUserId) updateBalance();
                }
            });
            connect(deleteUserBtn, &QPushButton::clicked, &adminDialog, [&]() {
                int row = userTable->currentRow();
                if (row < 0) {
                    QMessageBox::warning(&adminDialog, "Error", "Select a user first");
                    return;
                }
                int userId_sel = userTable->item(row, 0)->text().toInt();
                User* target = db->findUserById(userId_sel);
                if (!target) return;
                if (target->login == "admin") {
                    QMessageBox::warning(&adminDialog, "Error", "Cannot delete admin account");
                    return;
                }
                QMessageBox::StandardButton res = QMessageBox::question(&adminDialog, "Confirm Deletion", 
                    "Are you sure you want to delete user: " + QString::fromStdString(target->login) + "?",
                    QMessageBox::Yes | QMessageBox::No);
                if (res == QMessageBox::Yes) {
                    db->deleteUser(userId_sel);
                    refreshUsers();
                }
            });
            QPushButton *closeBtn = new QPushButton("Close", &adminDialog);
            adminLayout->addWidget(closeBtn);
            connect(closeBtn, &QPushButton::clicked, &adminDialog, &QDialog::accept);
            adminDialog.exec();
        });
    }
    connect(topUpBtn, &QPushButton::clicked, this, [=]() {
        bool ok;
        double amount = QInputDialog::getDouble(this, "Top Up", "Enter amount:", 10.0, 0.0, 1000.0, 2, &ok);
        if (ok && amount > 0) {
            User* user = db->findUserById(currentUserId);
            if (user) {
                user->balance += amount;
                db->updateUser(*user);
                updateBalance();
                QMessageBox::information(this, "Success", "Added €" + QString::number(amount, 'f', 2));
            }
        }
    });
    connect(myTicketsBtn, &QPushButton::clicked, this, [=]() {
        db->reload();
        TicketDialog dialog(db, currentUserId, this);
        dialog.exec();
    });
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("border: none;");
    QWidget *scrollWidget = new QWidget();
    QVBoxLayout *ticketsLayout = new QVBoxLayout(scrollWidget);
    auto createCard = [&](TicketOption opt) {
        QFrame *card = new QFrame();
        card->setStyleSheet("QFrame { background: #1e1e1e; border-radius: 10px; padding: 10px; }");
        QHBoxLayout *hlay = new QHBoxLayout(card);
        QLabel *titleLabel = new QLabel(opt.title);
        QPushButton *btn = new QPushButton("Buy");
        connect(btn, &QPushButton::clicked, this, [=]() {
            User* user = db->findUserById(currentUserId);
            if (!user) return;
            bool success = (opt.type == "train") 
                ? bm->bookTrainTicket(*user, "V_GENERIC", opt.from.toStdString(), opt.to.toStdString())
                : bm->bookRigaTicket(*user, "R_GENERIC", opt.rigaType);
            if (success) {
                updateBalance();
                QMessageBox::information(this, "Success", "Ticket purchased!");
            } else {
                QMessageBox::warning(this, "Error", "Not enough balance.");
            }
        });
        hlay->addWidget(titleLabel);
        hlay->addStretch();
        hlay->addWidget(btn);
        ticketsLayout->addWidget(card);
    };
    std::vector<TicketOption> options = {
        {"90 min ticket (€1.50)", "riga", "", "", RigaTicketType::SINGLE_90MIN},
        {"24h ticket (€5.00)", "riga", "", "", RigaTicketType::DAILY_24H},
        {"3 day ticket (€8.00)", "riga", "", "", RigaTicketType::DAILY_72H},
        {"5 day ticket (€10.00)", "riga", "", "", RigaTicketType::DAILY_120H},
        {"Monthly pass (€30.00)", "riga", "", "", RigaTicketType::MONTHLY_30D},
        {"Train Riga -> Imanta (€1.50)", "train", "Riga", "Imanta"},
        {"Train Riga -> Dubulti (€2.30)", "train", "Riga", "Dubulti"},
        {"Train Riga -> Sigulda (€2.80)", "train", "Riga", "Sigulda"},
        {"Train Riga -> Tukums II (€3.30)", "train", "Riga", "Tukums II"},
        {"Train Riga -> Aizkraukle (€3.80)", "train", "Riga", "Aizkraukle"},
        {"Train Riga -> Daugavpils (€8.90)", "train", "Riga", "Daugavpils"},
        {"Train Riga -> Liepaja (€9.10)", "train", "Riga", "Liepaja"}
    };
    for (const auto &opt : options) createCard(opt);
    scrollWidget->setLayout(ticketsLayout);
    scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(scrollArea);
}
void MainWindow::updateBalance()
{
    User* user = db->findUserById(currentUserId);
    if (user) {
        balanceLabel->setText("Balance: €" + QString::number(user->balance, 'f', 2));
    }
}
