#include "listwidget.h"
#include <QInputDialog>
#include <QDesktopWidget>
#include <QApplication>
#include <QDir>
#include <QFileDialog>

ListWidget::ListWidget(QWidget *parent)
    : QWidget(parent)
{

  QVBoxLayout *vbox = new QVBoxLayout();
  QVBoxLayout *vbox2 = new QVBoxLayout(this);
  vbox->setSpacing(10);


  //layouts
  QHBoxLayout *hbox = new QHBoxLayout(this);
  QHBoxLayout *hbox2 = new QHBoxLayout(this);

  lw = new QListWidget(this);

  add = new QPushButton("Browse Filesystem", this);
  unmount = new QPushButton("Unmount", this);
  tree = new QPushButton("Scan...", this);
  tree->setEnabled(FALSE);
  remove = new QPushButton("Remove", this);
  removeAll = new QPushButton("Remove All", this);
  quit= new QPushButton("Quit", this);
  generate = new QPushButton( "Generate Partition Table", this);
  

  vbox->setSpacing(5);
  vbox->addStretch(5);
  vbox->addWidget(unmount);
  vbox->addWidget(tree);
  vbox->addStretch(5);
  vbox->addWidget(remove);
  vbox->addWidget(removeAll);
  vbox->addWidget(quit);
  vbox->addStretch(1);

  hbox->addWidget(lw);
  hbox->addLayout(vbox);

  vbox2->setSpacing(10);
  hbox2->addWidget(generate,1, Qt::AlignLeft);
  hbox2->addWidget(add);
  vbox2->addLayout(hbox2);

  vbox2->addLayout(hbox);
  vbox->addStretch(1);

  connect(add, SIGNAL(clicked()), this, SLOT(addItem()));
  connect(unmount, SIGNAL(clicked()), this, SLOT(unmount_fs()));
  connect(tree, SIGNAL(clicked()), this, SLOT(display_tree()));
  connect(remove, SIGNAL(clicked()), this, SLOT(removeItem()));
  connect(removeAll, SIGNAL(clicked()), this, SLOT(clearItems()));
  connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
  connect( generate, SIGNAL(clicked()), this, SLOT(display_parts()) );

  setLayout(vbox2);
  display_parts();
}


void ListWidget::addItem()
{
    QString path;

    /* browse pt fisiere 
    path = QFileDialog::getOpenFileName(this, "Choose a file to open", QString::null, QString::null);*/

   //browse pt directorul unde este montat un sistem de fisiere
    path = QFileDialog::getExistingDirectory(this,tr("Find Files"), QDir::currentPath());

    if (!path.isEmpty() && lw->findItems(path,Qt::MatchExactly).count() == 0) 
   	  lw->addItem(path);

 }


void ListWidget::unmount_fs()
{ QListWidgetItem *current_selection = lw->currentItem();

  if (current_selection) { 

	  QString item = lw->currentItem()->text();

	  //urmatoarele 2 randuri convertesc din QString in char *
	  QByteArray text = item.toLatin1();
	  char *name = text.data(); 

	//aflare loop device montat la adresa din lista
	

	char command[100],image[100];
	strcpy(command,"mount | grep ");
	strcat(command,name);
	strcat(command," | cut -d ' ' -f 1 > file.txt ");
	system(command);
	FILE *f=fopen("file.txt","r");
	fgets(image,100,f);
	image[strlen(image)-1]='\0';
	fclose(f);

	//scrie stage 1 in stage1.txt
	strcpy(command,"sudo ext3grep ");
	strcat(command,image);
	strcat(command," --dump-names > stage1.txt");
	system(command);

	//strcpy(command," ");

	  char string[100];
	  char message[100];
	  strcpy(string,"sudo umount ");
	  strcat(string,name);
	  strcpy(message,name);
	  if(system(string) == 0){
		strcat(message," was successfully unmounted!");
		tree->setEnabled(TRUE);
	}
  	  else
		strcat(message," could not be unmounted");
	  if(QMessageBox::information(0,"Unmount",message,1));
	  lw->clear();
  }
 else
	if(QMessageBox::information(0,"Unmount","Please select a item from the list."));
}



void ListWidget::display_tree()
{	
	QLabel *label;
	label = new QLabel(0);
	
	
	FILE *f=fopen("stage1.txt","r");

	char string[100],text[10000];
	while(fgets(string,100,f)!=NULL)
		strcat(text,string);
	fclose(f);

  	label->setText(text);
	label->show();
	tree->setEnabled(FALSE);
}


void ListWidget::removeItem()
{
  QListWidgetItem *item = lw->currentItem();

  if (item) {
    int r = lw->row(item);
    lw->takeItem(r);
    delete item;
  }
  else
	if(QMessageBox::information(0,"Unmount","Please select a partition"));
}

void ListWidget::clearItems()
{
  if (lw->count() != 0)
    lw->clear();
}



void center(QWidget &widget)
{
  int x, y;
  int screenWidth;
  int screenHeight;
  int width, height;
  QSize windowSize;

  QDesktopWidget *desktop = QApplication::desktop();

  width = widget.frameGeometry().width();
  height = widget.frameGeometry().height();  

  screenWidth = desktop->width();
  screenHeight = desktop->height();
 
  x = (screenWidth - width) / 2;
  y = (screenHeight - height) / 2;

  widget.move( x, y );
}

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);  

  ListWidget window;
	
  window.setWindowTitle("Ext3grep");
  window.show();
  center(window);

  return app.exec();
}

