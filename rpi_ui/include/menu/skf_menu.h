#ifndef _SKF_MENU_H_
#define _SKF_MENU_H_

#include "skf_menu_item.h"

typedef class SKF_MENU;

class SKF_MENU
{
private:
	SKF_MENU_ITEM *root;
	SKF_MENU_ITEM *current;
	char *title;
public:
	void debug_print();	
};

#endif