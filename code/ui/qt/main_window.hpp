#ifndef LAIR_UI_QT_MAIN_WINDOW_HPP
#define LAIR_UI_QT_MAIN_WINDOW_HPP

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QLabel;
class QListWidget;
class QLineEdit;
class QPushButton;
class QTextEdit;
QT_END_NAMESPACE

namespace lair::ui::qt {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void BuildUi();

    QTextEdit *chat_view_ = nullptr;
    QListWidget *user_list_ = nullptr;
    QLineEdit *message_input_ = nullptr;
    QPushButton *send_button_ = nullptr;
    QLabel *status_label_ = nullptr;
};

}  // namespace lair::ui::qt

#endif
