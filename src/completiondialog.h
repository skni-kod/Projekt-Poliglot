#ifndef COMPLETIONDIALOG_H
#define COMPLETIONDIALOG_H
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
class CompletionDialog : public QFrame {
    Q_OBJECT
public:
    CompletionDialog(QWidget *parent);
    void setElapsedTime(int seconds, int lvl);
private:
    QLabel *elapsedTimeLabel;
};
#endif // COMPLETIONDIALOG_H
