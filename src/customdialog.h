#ifndef CUSTOMDIALOG_H
#define CUSTOMDIALOG_H
#include<QFrame>
#include <QLabel>
#include <QVBoxLayout>

class CustomDialog : public QFrame {
    Q_OBJECT
public:
    CustomDialog(QWidget *parent, QString message, bool okOnly);
private:
    QVBoxLayout *layout;
    QLabel *messageLabel;
    ~CustomDialog();
signals:
    void nextClicked();
};

#endif // CUSTOMDIALOG_H

// #ifndef COMPLETIONDIALOG_H
// #define COMPLETIONDIALOG_H
// #include <QFrame>
// #include <QLabel>
// #include <QVBoxLayout>
// #include <QPushButton>
// class CompletionDialog : public QFrame {
//     Q_OBJECT
// public:
//     CompletionDialog(QWidget *parent);
//     void setElapsedTime(int seconds, int lvl, int wordsAmount);
// private:
//     QLabel *elapsedTimeLabel;
//     QVBoxLayout *layout;
//     ~CompletionDialog();
// signals:
//     void nextClicked();
// };
// #endif // COMPLETIONDIALOG_H

