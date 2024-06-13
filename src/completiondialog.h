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
    void setElapsedTime(int seconds, int lvl, int wordsAmount);
private:
    QLabel *elapsedTimeLabel;
    QVBoxLayout *layout;
    ~CompletionDialog();
signals:
    void nextClicked();
};
#endif // COMPLETIONDIALOG_H
