#include "widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    dialog = new CreateNewdialog(this);
    setWindowTitle("Backpack");

    createLayouts();
    createMenu();

    connect(checkBox, SIGNAL(stateChanged(int)), scene, SLOT(setMode(int)));
    connect(dialog, SIGNAL(accepted()), this, SLOT(createNewImage()));
    connect(button, SIGNAL(clicked()), this, SLOT(startAlgo()));
    connect(addAll, SIGNAL(clicked()), this, SLOT(addAllImages()));
    connect(scene, SIGNAL(finished(QList<QGraphicsPixmapItem*>)),
            listWidget, SLOT(addReturnedItems(QList<QGraphicsPixmapItem*>)));
}

void Widget::save()
// Сохранение зображения
{
    QImage img(scene->width(), scene->height(), QImage::Format_ARGB32);
    QPainter painter(&img);
    scene->setUnsetSaveMode(true);

    scene->render(&painter);

    QString name = QFileDialog::getSaveFileName(this, "Сохранить...", "img.png",
                                                "Изображение (*png), (*.jpg)");
    //qDebug() << currentDir;

    if(img.save(name))
        qDebug() << "Yes";

    scene->setUnsetSaveMode(false);
}

void Widget::startAlgo()
{
    scene->startAlgorythm(algorythmsBox->currentIndex());
    atlasSize->setText("Размер атласа: " + QString::number(scene->width()) +
                       " x " + QString::number(scene->height()));
}

void Widget::createLayouts()
{
    mainLayout = new QHBoxLayout();
    layout = new QVBoxLayout();

    scene = new Scene(QRect(0, 0, 512, 512));
    view = new QGraphicsView(scene, this);
    mainLayout->addWidget(view);

    listWidget = new ListWidget(this);
    listWidget->setFixedWidth(150);
    layout->addWidget(listWidget);

    atlasSize = new QLabel("Размер атласа: " + QString::number(scene->width()) +
                           " x " + QString::number(scene->height()));
    layout->addWidget(atlasSize);

    checkBox = new QCheckBox("Минимальный атлас", this);
    layout->addWidget(checkBox);

    algorythmsBox = new QComboBox(this);
    algorythmsBox->addItem("Next Fit");
    algorythmsBox->addItem("Best Fit");
    algorythmsBox->addItem("Guillotine");
    layout->addWidget(algorythmsBox);

    button = new QPushButton("Упаковать изображения", this);
    layout->addWidget(button);
    addAll = new QPushButton("Упаковать все изображения", this);
    layout->addWidget(addAll);

    mainLayout->addLayout(layout);
    setLayout(mainLayout);
}

void Widget::createMenu()
{
    mainMenuBar = new QMenuBar(this);
    fileMenu = new QMenu("Файл", this);
    about = new QMenu("О программе", this);

    mainMenuBar->addMenu(fileMenu);
    mainMenuBar->addMenu(about);

    fileMenu->addAction("Создать", dialog, SLOT(show()));
    fileMenu->addAction("Открыть папку", this, SLOT(open()));
    fileMenu->addSeparator();

    fileMenu->addAction("Сохранить", this, SLOT(save()));
    fileMenu->addSeparator();

    fileMenu->addAction("Выход", this, SLOT(close()));

    about->addAction("О программе", this, SLOT(showAbout()));

    mainLayout->setMenuBar(mainMenuBar);
}

void Widget::showAbout()
{
    QMessageBox::information(this, "О программе",
                             "Backpack 1.0\nРазработчик: Медведев Егор");
}
void Widget::open()
// открытие папки с изображениями
{
    QDir currentDirectory(QFileDialog::getExistingDirectory(this,
                                                            "Выберите папку"));
    QStringList strings = currentDirectory.entryList();
    // получение имен файлов в папке

    QRegExp pattern("(.(pn|jpe|jp)g)"); // шаблон поддерживаемых файлов

    for(QString str : strings) {
        if(str.contains(pattern)) { // файл соответствует шаблону
            qDebug() << str;
            listWidget->addImage(QPixmap(currentDirectory.path() + "/" + str));
            // добавление изображения в контейнер
        }
    }
}

void Widget::createNewImage()
// вызов диалога создания новго атласа
{
    scene->clear();

    try {
        scene->setSceneRect(dialog->getSize());
    }
    catch(QString str) {
        QMessageBox::critical(this, "Ошибка!", str);
    }
    // установить размеры сцены, заданные пользователем
    dialog->clear();
    atlasSize->setText("Размер атлса: " + QString::number(scene->width()) +
                       " x " + QString::number(scene->height()));
}

void Widget::addAllImages()
{
    qDebug() << listWidget->count();
    for(int i = 0; i < listWidget->count(); ++i) {
        QPixmap pix = qvariant_cast <QPixmap>
                      (listWidget->item(i)->data(Qt::UserRole));
        scene->addPixmap(pix);
    }

    listWidget->clear();

    startAlgo();
}

Widget::~Widget()
{
}