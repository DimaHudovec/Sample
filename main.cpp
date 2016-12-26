#include "mainwindow.h"
#include <QApplication>
//dc

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
}
