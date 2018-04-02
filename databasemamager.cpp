#include "databasemamager.h"

DatabaseMamager* DatabaseMamager::instance = nullptr;

/***************************************************************************/

DatabaseMamager* DatabaseMamager::GetInstance()
{
  if (!instance) {
    instance = new DatabaseMamager();
  }
  return instance;
}

/***************************************************************************/

DatabaseMamager::DatabaseMamager()
{
  SetupDb();
}

/***************************************************************************/

void DatabaseMamager::AddBrew(
    QSqlQuery &q,
    const QString &name,
    const QVariant &methodId,
    const QVariant &groundId,
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
    )
{
  q.prepare(QLatin1String("insert into brews("
                          "name,"
                          "method,"
                          "ground,"
                          "date,"
                          "ratio,"
                          "time,"
                          "water,"
                          "weight,"
                          "grind,"
                          "temperature,"
                          "notes,"
                          "rating,"
                          "sweetness,"
                          "acidity,"
                          "clarity,"
                          "body,"
                          "aftertaste)"
                          "values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"));
  q.addBindValue(name);
  q.addBindValue(methodId);
  q.addBindValue(groundId);
  q.addBindValue(date);
  q.addBindValue(ratio);
  q.addBindValue(time);
  q.addBindValue(water);
  q.addBindValue(weight);
  q.addBindValue(grind);
  q.addBindValue(temperature);
  q.addBindValue(notes);
  q.addBindValue(rating);
  q.addBindValue(sweetness);
  q.addBindValue(acidity);
  q.addBindValue(clarity);
  q.addBindValue(body);
  q.addBindValue(aftertaste);
  q.exec();
}

/***************************************************************************/

QVariant DatabaseMamager::AddGround(
    QSqlQuery &q,
    const QString &name,
    int origin,
    int roast,
    const QDate& dateroast
    )
{
  !q.prepare(QLatin1String("insert into grounds("
                           "name,"
                           "origin,"
                           "roast,"
                           "dateroast"
                           ") values(?,?,?,?)"));
  q.addBindValue(name);
  q.addBindValue(origin);
  q.addBindValue(roast);
  q.addBindValue(dateroast);
  q.exec();
  return q.lastInsertId();
}

/***************************************************************************/

QVariant DatabaseMamager::AddMethod(
    QSqlQuery &q,
    const QString &name,
    const QString &variant
    )
{
  !q.prepare(QLatin1String("insert into methods("
                           "name,"
                           "variant)"
                           "values(?, ?)"));

  q.addBindValue(name);
  q.addBindValue(variant);
  q.exec();
  return q.lastInsertId();
}

/***************************************************************************/

QSqlError DatabaseMamager::SetupDb()
{
  auto db = QSqlDatabase::addDatabase("QSQLITE");
  //db.setDatabaseName(":memory:"); //HIRO change to another name save to disk
  db.setDatabaseName("coffee.db");

  if (!db.open()) {
    return db.lastError();
  }

  QStringList tables = db.tables();
  if (tables.contains("brews", Qt::CaseInsensitive)
      && tables.contains("methods", Qt::CaseInsensitive)) {
    return QSqlError();
  }

  auto err = CreateTables();

  if (err.type() != QSqlError::NoError) {
    return err;
  }

  return CreateDefaultData();
}

/***************************************************************************/

QSqlError DatabaseMamager::CreateTables()
{
  QSqlQuery q;
  if (!q.exec(QLatin1String("create table brews("
                            "id integer primary key AUTOINCREMENT,"
                            "name varchar,"
                            "method integer,"
                            "ground integer,"
                            "date varchar,"
                            "ratio integer,"
                            "time varchar,"
                            "water real,"
                            "weight real,"
                            "grind integer,"
                            "temperature real,"
                            "notes varchar,"
                            "rating integer,"
                            "sweetness interger,"
                            "acidity interger,"
                            "clarity interger,"
                            "body interger,"
                            "aftertaste interger)"))) {
    return q.lastError();
  }

  if (!q.exec(QLatin1String("create table methods("
                            "id integer primary key AUTOINCREMENT, "
                            "name varchar, "
                            "variant varchar)"))) {
    return q.lastError();
  }

  if (!q.exec(QLatin1String("create table grounds("
                            "id integer primary key AUTOINCREMENT, "
                            "name varchar,"
                            "origin integer,"
                            "roast integer,"
                            "dateroast varchar)"))) {
    return q.lastError();
  }

  return QSqlError();
}

/***************************************************************************/

QSqlError DatabaseMamager::CreateDefaultData()
{
  QSqlQuery q;
  AddMethod(q, QLatin1String("Pressca"), QLatin1String("French Press"));
  AddMethod(q, QLatin1String("French Press"), QLatin1String("French Press"));
  AddMethod(q, QLatin1String("Mellita"), QLatin1String("Drip"));
  AddMethod(q, QLatin1String("Nanopresso"), QLatin1String("Expresso"));

  AddGround(q, QLatin1String("Baggio Bourbon"), 1, 1, QDate::currentDate());
  AddGround(q, QLatin1String("Baggio Expresso"), 1, 1, QDate::currentDate());
  AddGround(q, QLatin1String("Orfeu Pisseli"),  1, 1, QDate::currentDate());
  AddGround(q, QLatin1String("Baggio Aromas - Caramelo"), 1, 1, QDate::currentDate());
  AddGround(q, QLatin1String("Superior"),  1, 1, QDate::currentDate());

  CreateNewBrew();

  return q.lastError();
}

/***************************************************************************/

QSqlError DatabaseMamager::CreateNewGround()
{
  QSqlQuery q;
  AddGround(q, "", 1, 1, QDate::currentDate());

  return q.lastError();
}

/***************************************************************************/

QSqlError DatabaseMamager::CreateNewMethod()
{
  QSqlQuery q;
  AddMethod(q,"", "");

  return q.lastError();
}

/***************************************************************************/

QSqlError DatabaseMamager::CreateNewBrew()
{
  QSqlQuery m(QLatin1String("select id from methods"));
  m.last();
  const int methodId = m.record().value(0).toInt();

  QSqlQuery g(QLatin1String("select id from grounds"));
  g.last();
  const int groundId = g.record().value(0).toInt();

  QSqlQuery q;
  AddBrew(q, "empty", methodId, groundId, QDateTime::currentDateTime(), 4, QTime(), 80.0, 20.0, 3, 93.0, "", 5, 50, 50, 50, 50, 50);

  return q.lastError();
}

/***************************************************************************/
