/**
  * Основное окно программы
  **/
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QMenuBar>
#include <QMenu>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QCheckBox>
#include <QLabel>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <QDebug>

#include "scene.h"
#include "listwidget.h"
#include "createnewdialog.h"

#ifdef bool
    #undef bool
#endif
class Widget : public QWidget
{
    Q_OBJECT

private:
    Scene *scene; // сцена, на которой собирается атлас
    ListWidget *listWidget; // контейнер изображений для атласа
    CreateNewdialog *dialog; // диалог создания нового атласа

    QLabel *atlasSize;
    QGraphicsView *view;
    QPushButton *startAlogButton;
    QPushButton *addAll;
    QPushButton *clearButton;
    QPushButton *removeButton;
    QHBoxLayout *mainLayout;
    QVBoxLayout *layout;
    QMenuBar *mainMenuBar;
    QComboBox *algorithmsBox;
    QCheckBox *checkBox;
    QMenu *fileMenu;
    QMenu *algorythms;
    QMenu *settings;
    QMenu *about;

    void createLayouts();
    void createMenu();

    bool writeCoords(QString path, QList<QGraphicsItem *> &items);

private slots:
    void open();
    void clearAtlas();
    void removeTextures();
    void createNewImage();
    void addAllImages();
    void save();
    void startAlgo();
    void showAbout();

public:
    Widget(QWidget *parent = 0);
    ~Widget();
};

#endif // WIDGET_H
