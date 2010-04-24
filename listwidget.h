#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QDialog>
#include <QtGui>
#include <QLabel>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

class ListWidget : public QWidget
{
  Q_OBJECT

  public:
    ListWidget(QWidget *parent = 0);

    QListWidget *lw;
    QPushButton *add;
    //QPushButton *unmount;
    QPushButton *tree;
    QPushButton *remove;
    QPushButton *removeAll;
    QPushButton *quit,*generate;
    QPushButton *recover;
    bool is_root;
    char image[100];    
	
  public slots:
    void addItem();
    void removeItem();
    void clearItems();
    //void unmount_fs();
    void display_tree();
    void recover_file();
    void display_parts(){
	if(is_root){
		char string[100],*aux;
		
		lw->clear();
	
		FILE *f=fopen("file.txt","r");
		
		while(fgets(string,100,f)!=NULL)
		{
			aux=strdup(string);
			aux[strlen(aux)-1]='\0';
			if(lw->findItems(aux,Qt::MatchExactly).count() == 0)
				lw->addItem(aux);
			free(aux);
		} 		
		fclose(f);
	}
    }   
};


#endif
