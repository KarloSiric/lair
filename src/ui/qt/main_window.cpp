#include "ui/qt/main_window.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

namespace lair::ui::qt {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    BuildUi();
    setWindowTitle("LAIR Qt Client");
    resize(1200, 760);
}

void MainWindow::BuildUi() {
    auto *root = new QWidget(this);
    auto *main_layout = new QHBoxLayout(root);
    auto *chat_layout = new QVBoxLayout();
    auto *sidebar_layout = new QVBoxLayout();
    auto *composer_layout = new QHBoxLayout();

    chat_view_ = new QTextEdit(root);
    chat_view_->setReadOnly(true);
    chat_view_->setPlaceholderText("Chat timeline will appear here once the Qt client is wired to the networking layer.");

    message_input_ = new QLineEdit(root);
    message_input_->setPlaceholderText("Type a message...");

    send_button_ = new QPushButton("Send", root);
    status_label_ = new QLabel("Disconnected", root);
    user_list_ = new QListWidget(root);
    user_list_->addItem("Users will appear here");

    composer_layout->addWidget(message_input_);
    composer_layout->addWidget(send_button_);

    chat_layout->addWidget(new QLabel("Chat", root));
    chat_layout->addWidget(chat_view_, 1);
    chat_layout->addLayout(composer_layout);

    sidebar_layout->addWidget(new QLabel("Online Users", root));
    sidebar_layout->addWidget(user_list_, 1);
    sidebar_layout->addWidget(new QLabel("Status", root));
    sidebar_layout->addWidget(status_label_);

    main_layout->addLayout(chat_layout, 3);
    main_layout->addLayout(sidebar_layout, 1);

    setCentralWidget(root);
}

}  // namespace lair::ui::qt
