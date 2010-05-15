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

  add = new QPushButton("Browse Filesystem Image", this);
  //unmount = new QPushButton("Unmount", this);
  tree = new QPushButton("Scan...", this);
  tree->setEnabled(FALSE);
  recover= new QPushButton("Recover Me", this);
  recover->setEnabled(FALSE);
  remove = new QPushButton("Remove", this);
  removeAll = new QPushButton("Remove All", this);
  quit= new QPushButton("Quit", this);
  generate = new QPushButton( "Generate Partition Table", this);
  

  vbox->setSpacing(5);
  vbox->addStretch(5);
  //vbox->addWidget(unmount);
  vbox->addWidget(tree);
  vbox->addWidget(recover); 
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
  //connect(unmount, SIGNAL(clicked()), this, SLOT(unmount_fs()));
  connect(tree, SIGNAL(clicked()), this, SLOT(display_tree()));
  connect(recover, SIGNAL(clicked()), this, SLOT(recover_file()));
  connect(remove, SIGNAL(clicked()), this, SLOT(removeItem()));
  connect(removeAll, SIGNAL(clicked()), this, SLOT(clearItems()));
  connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
  connect( generate, SIGNAL(clicked()), this, SLOT(display_parts()) );

  setLayout(vbox2);
  //display_parts();
}

//void ListWidget::unmount_fs(){}

void ListWidget::addItem()
{
	QString path;

    	//browse pt fisiere 
    	path = QFileDialog::getOpenFileName(this, "Choose a file to open", QString::null, QString::null);


	//urmatoarele 2 randuri convertesc din QString in char *
	  	QByteArray text = path.toLatin1();
	  	char *name = text.data(); 
		strcpy(image,name);
	
	char command[100];
	strcpy(command,"file ");
	strcat(command,image);
	strcat(command," > stage1.txt");
	//if (system(command) == -1){
	//	perror("system");
	//	exit(-1);
	//}
	system(command);
	FILE *f=fopen("stage1.txt","r");
	char string[100];
	fgets(string,100,f);
	fclose(f);

   	//browse pt directorul unde este montat un sistem de fisiere
    	//path = QFileDialog::getExistingDirectory(this,tr("Find Files"), QDir::currentPath());

    	if (!path.isEmpty() && lw->findItems(path,Qt::MatchExactly).count() == 0) {
		if(strstr(string," ext3 ")!=NULL){	
   	  		lw->addItem(path);
			if(QMessageBox::information(0,"Unmount","Please unmount the partition before you start the recovery!"));
			tree->setEnabled(TRUE);
			recover->setEnabled(FALSE);
			 }		
		else 
			if(QMessageBox::information(0,"Scan","Select ext3 filesystems only!"));
	}
}


void ListWidget::display_tree()
{	
	QListWidgetItem *current_selection = lw->currentItem();

  	if (current_selection) { 

		QString item = lw->currentItem()->text();

	 	//urmatoarele 2 randuri convertesc din QString in char *
	  	QByteArray text = item.toLatin1();
	  	char *name = text.data(); 
		strcpy(image,name);

		//scrie stage 1 in stage1.txt
		char string[100],command[100];
		strcpy(command,"sudo ext3grep ");
		strcat(command,image);
		strcat(command," --dump-names > stage1.txt");
		if (system(command) == -1) {
			perror("system");
			exit(-1);
		}

		if (lw->count() != 0)
    			lw->clear();
	
		FILE *f=fopen("stage1.txt","r");
		
		int s=0;	
		while(fgets(string,100,f)!=NULL){
			string[strlen(string)-1]='\0';
			
			if(strstr(string,"ext3grep.stage2")) {s=1;continue;}
	
			if(s==1) 
				lw->addItem(string);
		}
		fclose(f);
		recover->setEnabled(TRUE);
		tree->setEnabled(FALSE);
	}  
	else                                                                            
		if((QMessageBox::information(0,"Scan","Please select a item from the list."))) {;}
}


void ListWidget::recover_file()
{ 
	QListWidgetItem *current_selection = lw->currentItem();
	char command[100];

  	if (current_selection) { 

		QString item = lw->currentItem()->text();

	  	//urmatoarele 2 randuri convertesc din QString in char *
	  	QByteArray text = item.toLatin1();
	  	char *file_name = text.data(); 
	  

		strcpy(command,"ext3grep ");
		strcat(command,image);
		strcat(command," --restore-file ");
		strcat(command,file_name);	
		if (system(command) == -1) {
			perror("system");
			exit(-1);
		}
	        char message[100];	
		if(system(command) == 0)
		      	strcpy(message,"File was successfully recovered! yey!");
		else 
			strcpy(message,"Error. Sorry!");
		if(QMessageBox::information(0,"Recovery status",message,1)) {;}
  	}
 	else
	 	if(QMessageBox::information(0,"Recovery status","Please select a item from the list.")) {;}
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
	if(QMessageBox::information(0,"Unmount","Please select a partition")) {;}
}

void ListWidget::clearItems()
{
  if (lw->count() != 0)
    lw->clear();
  tree->setEnabled(FALSE);
  recover->setEnabled(FALSE);
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

	system("gksu mount > /dev/null");
	
  	QApplication app(argc, argv);  

  	ListWidget window;
	
  	window.setWindowTitle("Ext3grep");
  	window.show();
	window.setGeometry(0,0,400,300);
  	center(window);
   	if (system("sudo fdisk -l | tr -d '*' | tr -s ' ' | cut -d ' ' -f 1,6,7,8,9 |\
				grep Linux | grep -v swap  > file.txt") == -1) {
		perror("system");
		exit(-1);
	}
	  
    return app.exec();

}

