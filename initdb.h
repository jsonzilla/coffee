#ifndef INITDB_H
#define INITDB_H

#include <QtSql>

/***************************************************************************/

void addBrew(
    QSqlQuery &q,
    const QString &title,
    int year,
    const QVariant &methodId,
    const QVariant &groundId,
    int rating
    )
{
  q.addBindValue(title);
  q.addBindValue(year);
  q.addBindValue(methodId);
  q.addBindValue(groundId);
  q.addBindValue(rating);
  q.exec();
}

/***************************************************************************/

QVariant addGround(
    QSqlQuery &q,
    const QString &name
    )
{
  q.addBindValue(name);
  q.exec();
  return q.lastInsertId();
}

/***************************************************************************/

QVariant addMethod(
    QSqlQuery &q,
    const QString &name,
    const QDate &birthdate
    )
{
  q.addBindValue(name);
  q.addBindValue(birthdate);
  q.exec();
  return q.lastInsertId();
}

/***************************************************************************/

QSqlError initDb()
{
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName("coffeelog");

  if (!db.open())
    return db.lastError();

  QStringList tables = db.tables();
  if (tables.contains("brews", Qt::CaseInsensitive)
      && tables.contains("methods", Qt::CaseInsensitive))
    return QSqlError();

  QSqlQuery q;
  if (!q.exec(QLatin1String("create table brews(id integer primary key, title varchar, method integer, ground integer, year integer, rating integer)"))) {
    return q.lastError();
  }
  if (!q.exec(QLatin1String("create table methods(id integer primary key, name varchar, birthdate date)"))) {
    return q.lastError();
  }
  if (!q.exec(QLatin1String("create table ground(id integer primary key, name varchar)"))) {
    return q.lastError();
  }

  if (!q.prepare(QLatin1String("insert into methods(name, birthdate) values(?, ?)"))) {
    return q.lastError();
  }
  QVariant asimovId = addMethod(q, QLatin1String("Isaac Asimov"), QDate(1920, 2, 1));
  QVariant greeneId = addMethod(q, QLatin1String("Graham Greene"), QDate(1904, 10, 2));
  QVariant pratchettId = addMethod(q, QLatin1String("Terry Pratchett"), QDate(1948, 4, 28));

  if (!q.prepare(QLatin1String("insert into ground(name) values(?)"))) {
    return q.lastError();
  }
  QVariant sfiction = addGround(q, QLatin1String("Science Fiction"));
  QVariant fiction = addGround(q, QLatin1String("Fiction"));
  QVariant fantasy = addGround(q, QLatin1String("Fantasy"));

  if (!q.prepare(QLatin1String("insert into brews(title, year, method, ground, rating) values(?, ?, ?, ?, ?)"))) {
    return q.lastError();
  }
  addBrew(q, QLatin1String("Foundation"), 1951, asimovId, sfiction, 3);
//  addBrew(q, QLatin1String("Foundation and Empire"), 1952, asimovId, sfiction, 4);
//  addBrew(q, QLatin1String("Second Foundation"), 1953, asimovId, sfiction, 3);
//  addBrew(q, QLatin1String("Foundation's Edge"), 1982, asimovId, sfiction, 3);
//  addBrew(q, QLatin1String("Foundation and Earth"), 1986, asimovId, sfiction, 4);
//  addBrew(q, QLatin1String("Prelude to Foundation"), 1988, asimovId, sfiction, 3);
//  addBrew(q, QLatin1String("Forward the Foundation"), 1993, asimovId, sfiction, 3);
//  addBrew(q, QLatin1String("The Power and the Glory"), 1940, greeneId, fiction, 4);
//  addBrew(q, QLatin1String("The Third Man"), 1950, greeneId, fiction, 5);
//  addBrew(q, QLatin1String("Our Man in Havana"), 1958, greeneId, fiction, 4);
//  addBrew(q, QLatin1String("Guards! Guards!"), 1989, pratchettId, fantasy, 3);
//  addBrew(q, QLatin1String("Night Watch"), 2002, pratchettId, fantasy, 3);
//  addBrew(q, QLatin1String("Going Postal"), 2004, pratchettId, fantasy, 3);

  return QSqlError();
}

/***************************************************************************/

#endif
