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

#include<QLabel>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

class ListWidget : public QWidget
{
  Q_OBJECT

  public:
    ListWidget(QWidget *parent = 0);

    //QLabel *label;
    QListWidget *lw;
    QPushButton *add;
    QPushButton *unmount;
    QPushButton *tree;
    QPushButton *remove;
    QPushButton *removeAll;
    QPushButton *quit,*generate;
 	bool is_root;
  public slots:
    void addItem();
    void unmount_fs();
    void removeItem();
    void clearItems();
    void display_tree();
    void display_parts(){
	if(is_root){
	char string[100],*aux;
	system("gksudo -m \"Introduceti parola\" \"fdisk -l\" > file.txt");
	system("sudo fdisk -l | tr -d '*' | tr -s ' ' | cut -d ' ' -f 1,6,7,8,9 | grep Linux | grep -v swap  > file.txt");
	
	FILE *f=fopen("file.txt","r");
		
	while(fgets(string,100,f)!=NULL)
		{aux=strdup(string);
		aux[strlen(aux)-1]='\0';
		if(lw->findItems(aux,Qt::MatchExactly).count() == 0)
			lw->addItem(aux);
		free(aux);} 		
	fclose(f);
	}

	}   
    
 
};


#endif
