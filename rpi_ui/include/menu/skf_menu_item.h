#ifndef _SKF_MENU_ITEM_H_
#define _SKF_MENU_ITEM_H_

typedef class SKF_MENU_ITEM;
typedef void (*MENU_HANDLER)();

class SKF_MENU_ITEM
{
private:
	SKF_MENU_ITEM *prev;
	SKF_MENU_ITEM *next;
	SKF_MENU_ITEM *parent; 
	SKF_MENU_ITEM *child; 
	char *title;
	MENU_HANDLER menu_handler;
};

#endif