#include "mainwindow.h"
#include "ui_mainwindow.h"
// git

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

QBitArray MainWindow::bytesToBits(const QByteArray &bytes) const
{
    QBitArray bits;
    bits.resize(bytes.size()*8);
    // Convert from QByteArray to QBitArray
    for(int i=0; i<bytes.count(); ++i)
      {
        for(int b=0; b<8; ++b)
          {
            bits.setBit(i*8+b, bytes.at(i)&(1<<(7-b)));
          }
      }
    return bits;
}
QString MainWindow::bitsToString(const QBitArray &array )
{
	QString str;
	int value = 0;
	for(int i = 0; i < array.size(); i++)
	{
		if(array[i] == true)
		  str = str + "1";
		else
		  str = str + "0";
	}
	return str;
}

QByteArray MainWindow::bitsToBytes(const QBitArray &bits) const
{
    QByteArray bytes;
    if (bits.size()%8 != 0)
      bytes.resize(bits.size()/8 + 1);
    else
      bytes.resize(bits.size()/8);
    bytes.fill(0);
    for(int b=0; b<bits.count();++b) {
        bytes[b/8] = (bytes.at(b/8) | ((bits[b]?1:0)<<(((7-b%8)))));
    }
    return bytes;
}


QBitArray MainWindow::haming(const QBitArray &array )
{
	QBitArray newbits;
	newbits.resize(array.size() + 5);
	int n = 0;
	for(int i = 0; i < newbits.size(); i++)
	{
		if (  i == 0 || i == 1 || i == 3 || i == 7 || i == 15 )
		  {
		    newbits[i] = 0;
		    n++;
		  }
		else
		  newbits[i] = array[i-n];
	}
	for(int i = 0; i < 5; i++)
	{
	    int n = qPow(2,i);
	    for(int j = 0; j < 21; j++)
	      {
		if (j+1 & n)
		newbits[n - 1] = newbits[n - 1] ^ newbits[j];
	      }
	}

	return newbits;
}

QBitArray MainWindow::hamingMistake(const QBitArray &array )
{
	QBitArray newbits;
	newbits.resize(array.size());
	newbits = array;
	for(int i = 0; i < 5; i++)
	  {
	    int n = qPow(2,i) - 1;
	    newbits[n] = newbits[n] ^ newbits[n];
	  }
	for(int i = 0; i < 5; i++)
	{
	    int n = qPow(2,i);
	    for(int j = 0; j < 21; j++)
	      {
		if (j+1 & n)
		newbits[n - 1] = newbits[n - 1] ^ newbits[j];
	      }
	}
	int k = 0;
	for(int i = 0; i < 5; i++)
	  {
	    int n = qPow(2,i) - 1;
	    if(newbits[n] != array[n])
	      {
	      k = k + qPow(2,i);
	      newbits[n] = !newbits[n];
	      }
	  }
	k--;
	if (k > 0)
	  newbits[k] = !newbits[k];
	return newbits;
}
void MainWindow::on_pushButton_clicked()
{
  QByteArray text,conv;
  QBitArray bits,newbits,mist;
  QString str;
  conv.resize(2);
  text = (ui->textEdit->toPlainText().toUtf8());
  newbits.resize((text.size()/2 + text.size()%2) * 21);
  for (int i = 0; i < text.size()/2 + text.size()%2; i++)
  {
      for (int j = 0; j < 2; j++)
        {
          if(i == text.size()/2 + text.size()%2 || j == 2)
            conv[j] = 0;
          else
          conv[j] = text[j + 2*i];
        }
    bits = bytesToBits(conv);
    str = bitsToString(bits);
    ui->textEdit_2->setText(ui->textEdit_2->toPlainText().toUtf8() + "Исходное сообщение: " + str + "\n");
    bits = haming(bits);
    str = bitsToString(bits);
    ui->textEdit_2->setText(ui->textEdit_2->toPlainText().toUtf8() + "Закодированная часть сообщения: " + str + "\n");
    for (int j = 0; j < 21; j++)
      {
       newbits[j + 21*i] = bits[j];
      }
    }
  qDebug() << newbits;
  str = bitsToString(newbits);
  ui->textEdit_2->setText(ui->textEdit_2->toPlainText().toUtf8() + "Закодированное сообщение: " + str + "\n");
  newbits[2] = !newbits[2];
  qDebug() << newbits;
  str = bitsToString(newbits);
  ui->textEdit_2->setText(ui->textEdit_2->toPlainText().toUtf8() + "Закодированное сообщение с ошибкой: " + str + "\n");

  bits.resize(21);
  for (int i = 0; i < newbits.size()/21 ; i++)
  {
       for (int j = 0; j < 21; j++)
         {
           bits[j] = newbits[j + 21*i];
         }
       bits = hamingMistake(bits);
       for (int j = 0; j < 21; j++)
         {
          newbits[j + 21*i] = bits[j];
         }
  }
  str = bitsToString(newbits);
  ui->textEdit_2->setText(ui->textEdit_2->toPlainText().toUtf8() + "Исправленное сообщение: " + str + "\n");

  bits.resize(newbits.size() - 5*(newbits.size()/21) );
  int n = 0;
  for (int i = 0; i < newbits.size()/21; i++)
  {
      for (int j = 0; j < 21; j++)
      if (  j == 0 || j == 1 || j == 3 || j == 7 || j == 15 )
        n++;
      else
        bits[j + 21*i - n] = newbits[j + 21*i];
    }
  str = bitsToString(bits);
  ui->textEdit_2->setText(ui->textEdit_2->toPlainText().toUtf8() + "Исходное сообщение в битах: " + str + "\n");
  text = bitsToBytes(bits);
  ui->textEdit_2->setText(ui->textEdit_2->toPlainText().toUtf8() + "Исходное сообщение: " + text + "\n");
}
