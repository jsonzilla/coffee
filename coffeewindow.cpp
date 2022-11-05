#include "coffeewindow.h"
#include "coffeedelegate.h"
#include "databasemamager.h"
#include "tablemodelutils.h"

#include <QtSql>

/***************************************************************************/

CoffeeWindow::CoffeeWindow()
{
  db = DatabaseMamager::GetInstance();
  ui.setupUi(this);

  CreateBrewModel();
  CreateGroundModel();
  CreateMethodModel();

  connect(ui.mainTabs, &QTabWidget::currentChanged, this, [this]() {  ui.brewTable->setCurrentIndex(brewModel->index(0, 0)); });
  connect(ui.mainTabs, &QTabWidget::currentChanged, this, [this]() {  ui.groundTable->setCurrentIndex(groundModel->index(0, 0)); });
  connect(ui.mainTabs, &QTabWidget::currentChanged, this, [this]() {  ui.methodTable->setCurrentIndex(methodModel->index(0, 0)); });

  //HIRO gambiarra
  ui.brewTable->setCurrentIndex(brewModel->index(0, 0));
}

/***************************************************************************/

void CoffeeWindow::MapGroundForm(
  QDataWidgetMapper* mapper
)
{
  mapper->addMapping(ui.groundName, groundModel->fieldIndex("name"));
  mapper->addMapping(ui.originEdit, groundModel->fieldIndex("origin"));
  mapper->addMapping(ui.roastEdit, groundModel->fieldIndex("roast"));
  mapper->addMapping(ui.dateRoast, groundModel->fieldIndex("dateroast"));
}

/***************************************************************************/

void CoffeeWindow::CreateGroundModel()
{
  groundModel = new QSqlRelationalTableModel(ui.groundTable);
  auto mapper = TableModelUtils::SetupModel(this, ui.groundTable, groundModel, "grounds");
  TableModelUtils::PopulateModel(*groundModel);
  MapGroundForm(mapper);

  TableModelUtils::HideColumns(*ui.groundTable, *groundModel, { "id" });

  groundModel->setHeaderData(groundModel->fieldIndex("name"), Qt::Horizontal, tr("Name"));
  groundModel->setHeaderData(groundModel->fieldIndex("origin"), Qt::Horizontal, tr("Origin"));
  groundModel->setHeaderData(groundModel->fieldIndex("roast"), Qt::Horizontal, tr("Roast"));
  groundModel->setHeaderData(groundModel->fieldIndex("dateroast"), Qt::Horizontal, tr("Date roast"));

  connect(ui.buttonAddGround, &QPushButton::clicked, this, [this]() { AddGround(); });
  connect(ui.buttonRemoveGround, &QPushButton::clicked, this, [this]() { TableModelUtils::DeleteRow(*ui.groundTable, *groundModel); });
}

/***************************************************************************/

void CoffeeWindow::MapMethodForm(
  QDataWidgetMapper* mapper
)
{
  mapper->addMapping(ui.methodNameEdit, methodModel->fieldIndex("name"));
  mapper->addMapping(ui.varianteEdit, methodModel->fieldIndex("variant"));
}

/***************************************************************************/

void CoffeeWindow::CreateMethodModel()
{
  methodModel = new QSqlRelationalTableModel(ui.methodTable);
  auto mapper = TableModelUtils::SetupModel(this, ui.methodTable, methodModel, "methods");
  TableModelUtils::PopulateModel(*methodModel);
  MapMethodForm(mapper);

  QStringList toHide;
  toHide << "id";
  TableModelUtils::HideColumns(*ui.methodTable, *methodModel, toHide);

  methodModel->setHeaderData(methodModel->fieldIndex("name"), Qt::Horizontal, tr("Name"));
  methodModel->setHeaderData(methodModel->fieldIndex("variant"), Qt::Horizontal, tr("Variant"));

  connect(ui.buttonAddMethod, &QPushButton::clicked, this, [this]() { AddMethod(); });
  connect(ui.buttonRemoveMethod, &QPushButton::clicked, this, [this]() { TableModelUtils::DeleteRow(*ui.methodTable, *methodModel); });
}

/***************************************************************************/

void CoffeeWindow::MapBrewForm(
  QDataWidgetMapper* mapper
)
{
  mapper->addMapping(ui.nameEdit, brewModel->fieldIndex("name"));
  mapper->addMapping(ui.methodEdit, methodIdx);
  mapper->addMapping(ui.groundEdit, groundIdx);
  mapper->addMapping(ui.dateTimeEdit, brewModel->fieldIndex("date"));
  mapper->addMapping(ui.timeEdit, brewModel->fieldIndex("time"));
  mapper->addMapping(ui.waterDoubleSpin, brewModel->fieldIndex("water"));
  mapper->addMapping(ui.weightDoubleSpin, brewModel->fieldIndex("weight"));
  mapper->addMapping(ui.grindEdit, brewModel->fieldIndex("grind"));
  mapper->addMapping(ui.temperatureDoubleSpin, brewModel->fieldIndex("temperature"));
  mapper->addMapping(ui.notesTextEdit, brewModel->fieldIndex("notes"));
  mapper->addMapping(ui.ratingEdit, brewModel->fieldIndex("rating"));
  mapper->addMapping(ui.spinSweetness, brewModel->fieldIndex("sweetness"));
  mapper->addMapping(ui.sliderSweetness, brewModel->fieldIndex("sweetness"));
  mapper->addMapping(ui.spinAcidity, brewModel->fieldIndex("acidity"));
  mapper->addMapping(ui.sliderAcidity, brewModel->fieldIndex("acidity"));
  mapper->addMapping(ui.spinClarity, brewModel->fieldIndex("clarity"));
  mapper->addMapping(ui.sliderClarity, brewModel->fieldIndex("clarity"));
  mapper->addMapping(ui.spinBody, brewModel->fieldIndex("body"));
  mapper->addMapping(ui.sliderBody, brewModel->fieldIndex("body"));
  mapper->addMapping(ui.spinAftertaste, brewModel->fieldIndex("aftertaste"));
  mapper->addMapping(ui.sliderAftertaste, brewModel->fieldIndex("aftertaste"));
  mapper->addMapping(ui.ratioEdit, brewModel->fieldIndex("ratio"));

  //connect button to tab timer
  connect(ui.timerButton, &QPushButton::clicked, [this]() {ui.mainTabs->setCurrentIndex(3); });
}

/***************************************************************************/

void CoffeeWindow::CreateBrewModel()
{
  brewModel = new QSqlRelationalTableModel(ui.brewTable);
  auto mapper = TableModelUtils::SetupModel(this, ui.brewTable, brewModel, "brews");

  // Remember the indexes of the columns
  methodIdx = brewModel->fieldIndex("method");
  groundIdx = brewModel->fieldIndex("ground");

  // Set the relations to the other database tables
  brewModel->setRelation(methodIdx, QSqlRelation("methods", "id", "name"));
  brewModel->setRelation(groundIdx, QSqlRelation("grounds", "id", "name"));

  // Initialize the Method combo box
  ui.methodEdit->setModel(brewModel->relationModel(methodIdx));
  ui.methodEdit->setModelColumn(brewModel->relationModel(methodIdx)->fieldIndex("name"));

  ui.groundEdit->setModel(brewModel->relationModel(groundIdx));
  ui.groundEdit->setModelColumn(brewModel->relationModel(groundIdx)->fieldIndex("name"));

  QStringList toHide;
  toHide << "id" << "sweetness" << "acidity" << "clarity" << "body" << "aftertaste" << "temperature" << "notes" << "water" << "weight" << "ratio" << "time" << "grind" << "date";
  TableModelUtils::HideColumns(*ui.brewTable, *brewModel, toHide);

  // Set the localized header captions
  brewModel->setHeaderData(methodIdx, Qt::Horizontal, tr("Method"));
  brewModel->setHeaderData(groundIdx, Qt::Horizontal, tr("Ground"));
  brewModel->setHeaderData(brewModel->fieldIndex("name"), Qt::Horizontal, tr("Name"));
  brewModel->setHeaderData(brewModel->fieldIndex("rating"), Qt::Horizontal, tr("Rating"));

  MapBrewForm(mapper);

  // stars
  ui.brewTable->setItemDelegate(new CoffeeDelegate(this));
  mapper->setItemDelegate(new CoffeeDelegate(this));

  TableModelUtils::PopulateModel(*brewModel);

  connect(ui.buttonAddBrew, &QPushButton::clicked, this, [this]() {AddBrew(); });
  connect(ui.buttonRemoveBrew, &QPushButton::clicked, this, [this]() { TableModelUtils::DeleteRow(*ui.brewTable, *brewModel); });

  connect(ui.spinAcidity, SIGNAL(valueChanged(int)), ui.sliderAcidity, SLOT(setValue(int)));
  connect(ui.spinClarity, SIGNAL(valueChanged(int)), ui.sliderClarity, SLOT(setValue(int)));
  connect(ui.spinBody, SIGNAL(valueChanged(int)), ui.sliderBody, SLOT(setValue(int)));
  connect(ui.spinAftertaste, SIGNAL(valueChanged(int)), ui.sliderAftertaste, SLOT(setValue(int)));
  connect(ui.sliderAcidity, SIGNAL(valueChanged(int)), ui.spinAcidity, SLOT(setValue(int)));
  connect(ui.sliderClarity, SIGNAL(valueChanged(int)), ui.spinClarity, SLOT(setValue(int)));
  connect(ui.sliderBody, SIGNAL(valueChanged(int)), ui.spinBody, SLOT(setValue(int)));
  connect(ui.sliderAftertaste, SIGNAL(valueChanged(int)), ui.spinAftertaste, SLOT(setValue(int)));
}

/***************************************************************************/

void CoffeeWindow::AddGround()
{
  db->CreateNewGround();
  TableModelUtils::PopulateModel(*groundModel);
  ui.groundTable->setCurrentIndex(groundModel->index(0, 0));
}

/***************************************************************************/

void CoffeeWindow::AddMethod()
{
  db->CreateNewMethod();
  TableModelUtils::PopulateModel(*methodModel);
  ui.methodTable->setCurrentIndex(methodModel->index(0, 0));
}

/***************************************************************************/

void CoffeeWindow::AddBrew()
{
  db->CreateNewBrew();
  TableModelUtils::PopulateModel(*brewModel);
  ui.brewTable->setCurrentIndex(brewModel->index(0, 0));
}

/***************************************************************************/
