#ifndef BOOKWINDOW_H
#define BOOKWINDOW_H

#include <QtWidgets>
#include <QtSql>

#include "ui_coffeewindow.h"

/***************************************************************************/
class DatabaseMamager;
class QDataWidgetMapper;
class QSqlRelationalTableModel;
/***************************************************************************/

class CoffeeWindow : public QMainWindow
{
  Q_OBJECT
public:
  CoffeeWindow();

public slots:
  void CreateBrewModel();
  void CreateGroundModel();
  void CreateMethodModel();

  void AddGround();
  void AddMethod();
  void AddBrew();

private:
  void MapGroundForm(QDataWidgetMapper* mapper);
  void MapMethodForm(QDataWidgetMapper* mapper);
  void MapBrewForm(QDataWidgetMapper* mapper);

  Ui::CoffeeWindow ui;
  QSqlRelationalTableModel* brewModel = nullptr;
  QSqlRelationalTableModel* groundModel = nullptr;
  QSqlRelationalTableModel* methodModel = nullptr;
  DatabaseMamager* db = nullptr;
  int methodIdx;
  int groundIdx;
  int lastBrewIndex = 0;
  int lastGroundIndex = 0;
  int lastMethodIndex = 0;
};

/***************************************************************************/

#endif
