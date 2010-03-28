#include<QWidget>
#include<QLabel>
#include<QObject>
#include<QPushButton>
#include<QApplication>
#include<QMainWindow>
class MyWidget :public QMainWindow
    {
        Q_OBJECT
	public:		
	QWidget *box;
	QPushButton *quit,*generate;
	QLabel *label;
	MyWidget();
	
public slots:void display_parts();
   
    };

