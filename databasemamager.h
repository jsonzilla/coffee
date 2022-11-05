#ifndef DATABASEMAMAGER_H
#define DATABASEMAMAGER_H

#include <QtSql>

/***************************************************************************/

class DatabaseManager
{
public:
  QSqlError CreateNewGround();
  QSqlError CreateNewMethod();
  QSqlError CreateNewBrew();

  static DatabaseManager* GetInstance();

private:
  DatabaseManager();
  DatabaseManager(const DatabaseManager&);

  QSqlError SetupDb();
  QSqlError CreateTables();
  QSqlError CreateDefaultData();

  void AddBrew(
    QSqlQuery& q,
    const QString& name,
    const QVariant& methodId,
    const QVariant& groundId,
    const QDateTime& date,
    int ratio,
    const QTime& time,
    double water,
    double weight,
    int grind,
    double temperature,
    const QString& notes,
    int rating,
    int sweetness,
    int acidity,
    int clarity,
    int body,
    int aftertaste
  );


  QVariant AddGround(
    QSqlQuery& q,
    const QString& name,
    int origin,
    int roast,
    const QDate& dateroast
  );

  QVariant AddMethod(
    QSqlQuery& q,
    const QString& name,
    const QString& variant
  );

  static DatabaseManager* instance;
};

/***************************************************************************/

#endif
