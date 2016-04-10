#include "listwidget.h"

ListWidget::ListWidget(QWidget *parent) : QListWidget(parent)
{
    setIconSize(QSize(100, 100));
    setViewMode(QListView::IconMode);
    setAcceptDrops(true);
    setDragEnabled(true);
}

void ListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("Picture"))
        event->accept();
    else
        event->ignore();
}

void ListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasFormat("Picture")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else
        event->ignore();
}

void ListWidget::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasFormat("Picture")) {
        QByteArray arr = event->mimeData()->data("Picture");
        QDataStream stream(&arr, QIODevice::ReadOnly);
        QPixmap pic;

        stream >> pic;
        addImage(pic);

        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else
        event->ignore();
}


void ListWidget::addReturnedItems(QList<QGraphicsPixmapItem *> items)
{
    if(!items.isEmpty())
        for (QGraphicsPixmapItem *item : items) { // перебор всех элементов
                addImage(item->pixmap());
                items.removeOne(item);
                delete item;
        }

    qDebug("Count of items = %d", count());
}

bool ListWidget::isContain(QPixmap pix)
// поиск изображения в списке
{
    for(int i = 0; i < count(); i++) {
        if(qvariant_cast<QImage>(item(i)->data(Qt::UserRole)) == pix.toImage())
            return true;
    }

    return false;
}

void ListWidget::addImage(QPixmap pic)
// Добавление изодбражения в список картинок
{
    if(pic.isNull() || isContain(pic)) {
#ifdef DEBUG
        qDebug() << "error";
#endif
        return;
    }

    QListWidgetItem *item = new QListWidgetItem(this);
    item->setIcon(QIcon(pic));
    item->setData(Qt::UserRole, QVariant(pic));
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable |
                   Qt::ItemIsDragEnabled);
}

void ListWidget::startDrag(Qt::DropActions supportedActions)
{
    Q_UNUSED(supportedActions);

    QByteArray arr;
    QDataStream stream(&arr, QIODevice::WriteOnly);
    QListWidgetItem *item = currentItem(); // Взятие текущего элемента

    QPixmap pic = qvariant_cast<QPixmap> (item->data(Qt::UserRole));
    stream << pic; // запись изображения в массив байтов

    QDrag *drag = new QDrag(this);
    QMimeData *data = new QMimeData();

    data->setData("Picture", arr);
    drag->setMimeData(data);
    drag->setPixmap(pic); // установка картинки на курсор
    // установка курсора на цент картинки
    drag->setHotSpot(QPoint(pic.width()/2, pic.height()/2));

    if(drag->exec(Qt::MoveAction) == Qt::MoveAction)
    // если совершилось перетаскивание, то удалить элемент из виджета
        delete takeItem(row(item));
}
