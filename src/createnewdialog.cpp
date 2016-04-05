#include "createnewdialog.h"

CreateNewdialog::CreateNewdialog(QWidget *parent) : QDialog(parent)
{
    mainLayout = new QGridLayout();

    wLabel = new QLabel("Ширина", this);
    hLabel = new QLabel("Высота", this);
    widthSize = new QLineEdit(this);
    heightSize = new QLineEdit(this);
    buttonAccept = new QPushButton("Ок", this);
    buttonReject = new QPushButton("Отмена", this);

    mainLayout->addWidget(wLabel, 0, 0);
    mainLayout->addWidget(widthSize, 0, 1);
    mainLayout->addWidget(hLabel, 1, 0);
    mainLayout->addWidget(heightSize, 1, 1);
    mainLayout->addWidget(buttonAccept, 2, 0);
    mainLayout->addWidget(buttonReject, 2, 1);

    setLayout(mainLayout);
    setFixedSize(150, 100);

    connect(buttonAccept, SIGNAL(clicked()), this, SIGNAL(accepted()));
    connect(this, SIGNAL(accepted()), SLOT(hide()));
    connect(buttonReject, SIGNAL(clicked()), this, SIGNAL(rejected()));
    connect(this, SIGNAL(rejected()), SLOT(hide()));
}

void CreateNewdialog::clear()
{
    widthSize->clear();
    heightSize->clear();
}

QRectF CreateNewdialog::getSize()
{
    int w = widthSize->text().toInt();
    int h = heightSize->text().toInt();

    if(w > 0 && h > 0)
        return QRectF(0, 0, w, h);
    else
        throw QString("Размеры изображения введены неверно!");
}
