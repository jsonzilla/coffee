#ifndef TABLEMODELUTILS_H
#define TABLEMODELUTILS_H

#include <QtSql>
#include <QTableView>
#include <QSqlRelationalTableModel>
#include <QMessageBox>
#include <QDataWidgetMapper>

namespace TableModelUtils
  {

  /***************************************************************************/

  void ShowError(const QSqlError &err)
  {
    QMessageBox::critical(nullptr, "Unable to initialize Database",
                          "Error initializing database: " + err.text());
  }

  /***************************************************************************/

  void PopulateModel(
      QSqlRelationalTableModel* model
      )
  {
    if (!model->select()) {
      ShowError(model->lastError());
      return;
    }
  }

  /***************************************************************************/

  int SelectedRow(
      QTableView* table
      )
  {
    int row = -1;
    QModelIndexList selection = table->selectionModel()->selectedRows();
    for (auto&& s : selection) {
      row = s.row();
    }
    return row;
  }

  /***************************************************************************/

  void DeleteRow(
      QTableView* table,
      QSqlRelationalTableModel* model
      )
  {
    model->removeRow(SelectedRow(table), QModelIndex());
    PopulateModel(model);
    table->setCurrentIndex(model->index(0, 0));
  }

  /***************************************************************************/

  QDataWidgetMapper* SetupModel(
      QWidget* parrent,
      QTableView* table,
      QSqlRelationalTableModel* model,
      const QString& tableName
      )
  {
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->setTable(tableName);

    table->setModel(model);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setSortingEnabled(true);

    auto mapper = new QDataWidgetMapper(parrent);
    mapper->setModel(model);
    QObject::connect(table->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), mapper, SLOT(setCurrentModelIndex(QModelIndex)));
    table->setCurrentIndex(model->index(0, 0));

    return mapper;
  }

  /***************************************************************************/

  void HideColumns(
      QTableView* table,
      QSqlRelationalTableModel* model,
      const QStringList& toHide
      )
  {
    for (auto&& column : toHide) {
      table->setColumnHidden(model->fieldIndex(column), true);
    }
  }

  /***************************************************************************/

  }

#endif
