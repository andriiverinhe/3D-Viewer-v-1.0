#include <QApplication>
#include <QLocale>

#include "mainwindow.hpp"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QLocale::setDefault(QLocale::c());

  setlocale(LC_NUMERIC, "C");

  MainWindow w;

  w.show();

  return a.exec();
}
