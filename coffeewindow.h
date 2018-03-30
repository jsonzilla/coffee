#ifndef BOOKWINDOW_H
#define BOOKWINDOW_H

#include <QtWidgets>
#include <QtSql>

#include "ui_bookwindow.h"

/***************************************************************************/

class CoffeeWindow: public QMainWindow
{
    Q_OBJECT
  public:
    CoffeeWindow();

  private:
    void showError(
        const QSqlError &err
        );

    bool InitializeDatabase();
    void CreateDataModel();
    void InitializeComboBoxes();
    bool PopulateModel();
    void SetModel();
    void HideIdColum();

    Ui::BookWindow ui;
    QSqlRelationalTableModel *model = nullptr;
    int methodIdx;
    int groundIdx;
};

/***************************************************************************/

#endif
