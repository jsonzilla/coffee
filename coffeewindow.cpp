#include "coffeewindow.h"
#include "coffeedelegate.h"
#include "initdb.h"

#include <QtSql>

/***************************************************************************/

CoffeeWindow::CoffeeWindow()
{
  ui.setupUi(this);

  if (!QSqlDatabase::drivers().contains("QSQLITE")) {
    QMessageBox::critical(this, "Unable to load database", "This demo needs the SQLITE driver");
  }

  if (!InitializeDatabase()) return;

  CreateDataModel();

  if (!PopulateModel()) return;

  SetModel();
  HideIdColum();

  ui.brewTable->setSelectionMode(QAbstractItemView::SingleSelection);

  InitializeComboBoxes();

  QDataWidgetMapper *mapper = new QDataWidgetMapper(this);
  mapper->setModel(model);
  mapper->setItemDelegate(new CoffeeDelegate(this));
  mapper->addMapping(ui.titleEdit, model->fieldIndex("title"));
  mapper->addMapping(ui.yearEdit, model->fieldIndex("year"));
  mapper->addMapping(ui.methodEdit, methodIdx);
  mapper->addMapping(ui.groundEdit, groundIdx);
  mapper->addMapping(ui.ratingEdit, model->fieldIndex("rating"));

  connect(ui.brewTable->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
          mapper, SLOT(setCurrentModelIndex(QModelIndex)));

  ui.brewTable->setCurrentIndex(model->index(0, 0));
}

/***************************************************************************/

bool CoffeeWindow::InitializeDatabase()
{
  bool ok = true;
  QSqlError err = initDb();
  if (err.type() != QSqlError::NoError) {
    showError(err);
    ok = false;
  }
  return ok;
}

/***************************************************************************/

void CoffeeWindow::CreateDataModel()
{
  model = new QSqlRelationalTableModel(ui.brewTable);
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  model->setTable("brews");

  // Remember the indexes of the columns
  methodIdx = model->fieldIndex("method");
  groundIdx = model->fieldIndex("genre");

  // Set the relations to the other database tables
  model->setRelation(methodIdx, QSqlRelation("methods", "id", "name"));
  model->setRelation(groundIdx, QSqlRelation("ground", "id", "name"));

  // Set the localized header captions
  model->setHeaderData(methodIdx, Qt::Horizontal, tr("Method Name"));
  model->setHeaderData(groundIdx, Qt::Horizontal, tr("Ground"));
  model->setHeaderData(model->fieldIndex("title"), Qt::Horizontal, tr("Title"));
  model->setHeaderData(model->fieldIndex("year"), Qt::Horizontal, tr("Year"));
  model->setHeaderData(model->fieldIndex("rating"), Qt::Horizontal, tr("Rating"));
}

/***************************************************************************/

void CoffeeWindow::InitializeComboBoxes()
{
  ui.methodEdit->setModel(model->relationModel(methodIdx));
  ui.methodEdit->setModelColumn(model->relationModel(methodIdx)->fieldIndex("name"));

  ui.groundEdit->setModel(model->relationModel(groundIdx));
  ui.groundEdit->setModelColumn(model->relationModel(groundIdx)->fieldIndex("name"));

}

/***************************************************************************/

bool CoffeeWindow::PopulateModel()
{
  bool ok = true;
  if (!model->select()) {
    showError(model->lastError());
    ok = false;
  }
  return ok;
}

/***************************************************************************/

void CoffeeWindow::SetModel()
{
  ui.brewTable->setModel(model);
  ui.brewTable->setItemDelegate(new CoffeeDelegate(ui.brewTable));
}

/***************************************************************************/

void CoffeeWindow::HideIdColum()
{
  ui.brewTable->setColumnHidden(model->fieldIndex("id"), true);
}

/***************************************************************************/

void CoffeeWindow::showError(const QSqlError &err)
{
  QMessageBox::critical(this, "Unable to initialize Database",
                        "Error initializing database: " + err.text());
}

/***************************************************************************/

