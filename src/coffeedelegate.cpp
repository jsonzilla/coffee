#include "coffeedelegate.h"

#include <QtWidgets>

/***************************************************************************/

CoffeeDelegate::CoffeeDelegate(
  QObject* parent
)
  : QSqlRelationalDelegate(parent)
{
}

/***************************************************************************/

void CoffeeDelegate::paint(
  QPainter* painter,
  const QStyleOptionViewItem& option,
  const QModelIndex& index
) const
{
  if (index.column() != 12) {
    QStyleOptionViewItem opt = option;
    opt.rect.adjust(0, 0, -1, -1); // since we draw the grid ourselves
    QSqlRelationalDelegate::paint(painter, opt, index);
  }
  else if (const QAbstractItemModel* model = index.model()) {
    auto currentActiveState = (option.state & QStyle::State_Active) ? QPalette::Normal : QPalette::Inactive;
    QPalette::ColorGroup cg = (option.state & QStyle::State_Enabled) ? currentActiveState : QPalette::Disabled;

    if (option.state & QStyle::State_Selected) {
      painter->fillRect(option.rect, option.palette.color(cg, QPalette::Highlight));
    }

    const int rating = model->data(index, Qt::DisplayRole).toInt();
    const int width = bean.width();
    const int height = bean.height();
    int x = option.rect.x();
    const int y = option.rect.y() + (option.rect.height() / 2) - (height / 2);
    for (int i = 0; i < rating; ++i) {
      painter->drawPixmap(x, y, bean);
      x += width;
    }
  }

  QPen pen = painter->pen();
  painter->setPen(option.palette.color(QPalette::Mid));
  painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
  painter->drawLine(option.rect.topRight(), option.rect.bottomRight());
  painter->setPen(pen);
}

/***************************************************************************/

QSize CoffeeDelegate::sizeHint(
  const QStyleOptionViewItem& option,
  const QModelIndex& index
) const
{
  if (index.column() == 12) {
    return QSize(5 * bean.width(), bean.height()) + QSize(1, 1);
  }

  return QSqlRelationalDelegate::sizeHint(option, index) + QSize(1, 1); // since we draw the grid ourselves
}

/***************************************************************************/

bool CoffeeDelegate::editorEvent(
  QEvent* event,
  QAbstractItemModel* model,
  const QStyleOptionViewItem& option,
  const QModelIndex& index
)
{
  if (index.column() != 12) {
    return QSqlRelationalDelegate::editorEvent(event, model, option, index);
  }

  if (event->type() == QEvent::MouseButtonPress) {
    auto const* mouseEvent = static_cast<QMouseEvent*>(event);
    int stars = qBound(0, int(0.7 + qreal(mouseEvent->pos().x()
      - option.rect.x()) / bean.width()), 5);
    model->setData(index, QVariant(stars));
    return false; //so that the selection can change
  }

  return true;
}

/***************************************************************************/

QWidget* CoffeeDelegate::createEditor(
  QWidget* parent,
  const QStyleOptionViewItem& option,
  const QModelIndex& index
) const
{
  if (index.column() != 4) {
    return QSqlRelationalDelegate::createEditor(parent, option, index);
  }

  // for editing the year, return a spinbox with a range from -1000 to 2100.
  auto* sb = new QSpinBox(parent);
  sb->setFrame(false);
  sb->setMaximum(2100);
  sb->setMinimum(-1000);

  return sb;
}

/***************************************************************************/
