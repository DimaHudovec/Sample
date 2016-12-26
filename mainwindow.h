#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBitArray>
#include <QDebug>
#include <QtCore/qmath.h>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  QBitArray bytesToBits(const QByteArray &bytes) const;
  QString bitsToString(const QBitArray &array ) ;
  QByteArray bitsToBytes(const QBitArray &bits) const;
  QBitArray haming(const QBitArray &array) ;
  QBitArray hamingMistake(const QBitArray &array) ;
private slots:
  void on_pushButton_clicked();

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
