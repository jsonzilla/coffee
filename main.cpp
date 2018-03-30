#include "coffeewindow.h"

#include <QtWidgets>

/***************************************************************************/

int main(
    int argc,
    char * argv[]
    )
{
  Q_INIT_RESOURCE(coffees);

  QApplication app(argc, argv);

  CoffeeWindow win;
  win.show();

  return app.exec();
}

/***************************************************************************/
