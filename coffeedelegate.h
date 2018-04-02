#ifndef BOOKDELEGATE_H
#define BOOKDELEGATE_H

#include <QModelIndex>
#include <QPixmap>
#include <QSize>
#include <QSqlRelationalDelegate>

/***************************************************************************/
QT_FORWARD_DECLARE_CLASS(QPainter)
/***************************************************************************/
class CoffeeDelegate : public QSqlRelationalDelegate
{
  public:
    CoffeeDelegate(
        QObject *parent
        );

    void paint(
        QPainter *painter,
        const QStyleOptionViewItem &option,
        const QModelIndex &index
        ) const override;

    QSize sizeHint(
        const QStyleOptionViewItem &option,
        const QModelIndex &index
        ) const override;

    bool editorEvent(
        QEvent *event,
        QAbstractItemModel *model,
        const QStyleOptionViewItem &option,
        const QModelIndex &index
        ) override;

    QWidget *createEditor(
        QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index
        ) const override;

  private:
    QPixmap bean;
};

/***************************************************************************/

#endif
