#include "test.h"

MyWidget::MyWidget()
{
		box = new QWidget;
		box->resize(600,600);
		quit = new QPushButton("Quit", box);
		generate = new QPushButton( "Generate Partition Table", box);
		quit->setFont( QFont( "Times", 18, QFont::Bold ) );
		quit->setGeometry(300,0,100, 50);
		generate->setFont( QFont( "Times", 18, QFont::Bold ) );
		QObject::connect( generate, SIGNAL(clicked()), this, SLOT(display_parts()) );
		label = new QLabel(box);
		label->setGeometry(0,100,800,500);
		box->show();
		label->show();
		quit->show();
}
void MyWidget::display_parts()
{    
	 	char string[1000],partition[10000];
		system("gksudo -m \"Introduceti parola\" \"fdisk -l\" > file.txt");
		sprintf(partition,"This is the partition table...\n");
		FILE *f=fopen("file.txt","r");
		while(fgets(string,100,f)!=NULL)
		strcat(partition,string);
		label->setText(partition);	
		label->show();
   	}
int main (int argc, char **argv )
{
    QApplication a( argc, argv );
	MyWidget s;    
	QObject::connect( s.quit, SIGNAL(clicked()), &a, SLOT(quit()) );
    return a.exec();
}
