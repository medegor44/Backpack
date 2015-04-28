/**
  * Окно создания нового атласа
  **/
#ifndef CREATENEWDIALOG
#define CREATENEWDIALOG

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>

class CreateNewdialog : public QDialog
{
    Q_OBJECT

private:
    QPushButton *buttonReject;
    QPushButton *buttonAccept;
    QLineEdit *widthSize;
    QLineEdit *heightSize;
    QGridLayout *mainLayout;
    QLabel *wLabel;
    QLabel *hLabel;

public:
    CreateNewdialog(QWidget *parent = 0);
    QRectF getSize();
    void clear();
};

#endif // CREATENEWDIALOG

