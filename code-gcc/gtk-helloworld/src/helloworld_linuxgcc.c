/*
 ============================================================================
 Name        : helloworld_linuxgcc.c
 Author      : span
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#if 0

#include <stdio.h>
#include <stdlib.h>

int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}

#else

/*
 *
 * add `pkg-config --cflags gtk+-2.0` to compiler
 * add `pkg-config --libs gtk+-2.0` to linker
 *
 * http://developer.gimp.org/api/2.0/gtk/GtkTextView.html
*/

#include <gtk/gtk.h>

#include "dbuscall.h"
#include "sysPowerSRV-dbus.h"



#define		USE_NEW_BTN
#define		USE_SCROLL_WIN
#define		USE_TEXT_VIEW


#ifdef USE_NEW_BTN
struct s_buttonWdgItem {
	gint		btnId;
	GtkWidget* 	btnWidget;
	void*		pbtnFn;
	gchar		btnName[10]; /* fix length needed */
};

static struct s_buttonWdgItem buttonWdgItemList[] = {
	{	0,	NULL, NULL,	"button 01" },
	{	1,	NULL, NULL,	"button 02" },
	{	2,	NULL, NULL,	"DVFS ON" },
	{	3,	NULL, NULL,	"DVFS OFF" },
	{	4,	NULL, NULL,	"CPU 1 core" },
	{	5,	NULL, NULL,	"CPU 2 core" },
	{	6,	NULL, NULL,	"CPU 3 core" },
	{	7,	NULL, NULL,	"CPU 4 core" },
	{	8,	NULL, NULL,	"CPU 400MHz" },
	{	9,	NULL, NULL,	"CPU 800MHz" },
	{	10,	NULL, NULL,	"CPU 1GHz" },
	{	11,	NULL, NULL,	"button 12" },
	{	12,	NULL, NULL,	"Go suspend" },
	{	13,	NULL, NULL,	"button 14" },
	{	14,	NULL, NULL,	"button 15" },
	{	15,	NULL, NULL,	"button 16" },
	{	-1,	NULL, NULL,	"---EOL---" },
};
#else
///static void *buttonWdgItemList = NULL;
#endif





#ifndef USE_TEXT_VIEW
void AddListItem (GtkWidget *listbox, char *sText);
#endif


#ifndef USE_TEXT_VIEW
static GtkWidget *g_listbox;
#else
static GtkTextBuffer *g_buffer;
#endif
static pthread_t dbus_thread_id;


/*
 * After it's connected to the right event, this will run when
 * the window is closed
 */
gint Delete(GtkWidget* widget, gpointer* data)
{
	g_print("Screw you guys, I'm going home!\n");
	sysPowerSRVDeinit();
	gtk_main_quit();                               /* stop gtk_main */
	return FALSE;                                  /* Kill the window */
}


gint go_Button_nofunction( void )
{
	gint iRet = 0;
	GtkTextIter iter;

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	gtk_text_buffer_insert(g_buffer, &iter, "No function on this button yet!!\n", -1);

	return iRet;
}


gint go_Button_01( void )
{
	gint iRet = 0;
	GtkTextIter iter;

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	gtk_text_buffer_insert(g_buffer, &iter, "Plain text\n", -1);

	return iRet;
}


gint go_Button_03( void )
{
	gint iRet = 0;
	GtkTextIter iter;
	int result = 0;

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	gtk_text_buffer_insert(g_buffer, &iter, "set CPU DVFS ON!\n", -1);

	result = sPSsetCPUDVFS(1);

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	if( result == 0 )
		gtk_text_buffer_insert(g_buffer, &iter, ">>>ok!\n", -1);
	else
		gtk_text_buffer_insert(g_buffer, &iter, ">>>fail!\n", -1);

	return iRet;
}


gint go_Button_04( void )
{
	gint iRet = 0;
	GtkTextIter iter;
	int result = 0;

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	gtk_text_buffer_insert(g_buffer, &iter, "set CPU DVFS OFF!\n", -1);

	result = sPSsetCPUDVFS(0);

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	if( result == 0 )
		gtk_text_buffer_insert(g_buffer, &iter, ">>>ok!\n", -1);
	else
		gtk_text_buffer_insert(g_buffer, &iter, ">>>fail!\n", -1);

	return iRet;
}


gint go_Button_05( void )
{
	gint iRet = 0;
	GtkTextIter iter;
	int result = 0;

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	gtk_text_buffer_insert(g_buffer, &iter, "set CPU on 1 core!\n", -1);

	result = sPSsetCPUActivatedNum(1);

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	if( result == 0 )
		gtk_text_buffer_insert(g_buffer, &iter, ">>>ok!\n", -1);
	else
		gtk_text_buffer_insert(g_buffer, &iter, ">>>fail!\n", -1);

	return iRet;
}


gint go_Button_06( void )
{
	gint iRet = 0;
	GtkTextIter iter;
	int result = 0;

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	gtk_text_buffer_insert(g_buffer, &iter, "set CPU on 3 core!\n", -1);

	result = sPSsetCPUActivatedNum(2);

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	if( result == 0 )
		gtk_text_buffer_insert(g_buffer, &iter, ">>>ok!\n", -1);
	else
		gtk_text_buffer_insert(g_buffer, &iter, ">>>fail!\n", -1);

	return iRet;
}


gint go_Button_07( void )
{
	gint iRet = 0;
	GtkTextIter iter;
	int result = 0;

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	gtk_text_buffer_insert(g_buffer, &iter, "set CPU on 3 core!\n", -1);

	result = sPSsetCPUActivatedNum(3);

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	if( result == 0 )
		gtk_text_buffer_insert(g_buffer, &iter, ">>>ok!\n", -1);
	else
		gtk_text_buffer_insert(g_buffer, &iter, ">>>fail!\n", -1);

	return iRet;
}


gint go_Button_08( void )
{
	gint iRet = 0;
	GtkTextIter iter;
	int result = 0;

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	gtk_text_buffer_insert(g_buffer, &iter, "set CPU on 4 core!\n", -1);

	result = sPSsetCPUActivatedNum(4);

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	if( result == 0 )
		gtk_text_buffer_insert(g_buffer, &iter, ">>>ok!\n", -1);
	else
		gtk_text_buffer_insert(g_buffer, &iter, ">>>fail!\n", -1);

	return iRet;
}


gint go_Button_09( void )
{
	gint iRet = 0;
	GtkTextIter iter;
	int result = 0;

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	gtk_text_buffer_insert(g_buffer, &iter, "set CPU at 400MHz!\n", -1);

	result = sPSsetCPUspeed(400);

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	if( result == 0 )
		gtk_text_buffer_insert(g_buffer, &iter, ">>>ok!\n", -1);
	else
		gtk_text_buffer_insert(g_buffer, &iter, ">>>fail!\n", -1);

	return iRet;
}


gint go_Button_10( void )
{
	gint iRet = 0;
	GtkTextIter iter;
	int result = 0;

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	gtk_text_buffer_insert(g_buffer, &iter, "set CPU at 800MHz!\n", -1);

	result = sPSsetCPUspeed(800);

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	if( result == 0 )
		gtk_text_buffer_insert(g_buffer, &iter, ">>>ok!\n", -1);
	else
		gtk_text_buffer_insert(g_buffer, &iter, ">>>fail!\n", -1);

	return iRet;
}


gint go_Button_11( void )
{
	gint iRet = 0;
	GtkTextIter iter;
	int result = 0;

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	gtk_text_buffer_insert(g_buffer, &iter, "set CPU at 1GHz!\n", -1);

	result = sPSsetCPUspeed(1000);

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	if( result == 0 )
		gtk_text_buffer_insert(g_buffer, &iter, ">>>ok!\n", -1);
	else
		gtk_text_buffer_insert(g_buffer, &iter, ">>>fail!\n", -1);

	return iRet;
}


gint go_Button_12( void )
{
	gint iRet = 0;
	GtkTextIter iter;
	int result = 0;

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	gtk_text_buffer_insert(g_buffer, &iter, "set SDRAM run at 1GHz!\n", -1);

	result = sPSsetCPUspeed(1000);

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	if( result == 0 )
		gtk_text_buffer_insert(g_buffer, &iter, ">>>ok!\n", -1);
	else
		gtk_text_buffer_insert(g_buffer, &iter, ">>>fail!\n", -1);

	return iRet;
}


gint go_Button_13( void )
{
	gint iRet = 0;
	GtkTextIter iter;
	int result = 0;

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	gtk_text_buffer_insert(g_buffer, &iter, "set CPU to suspend!\n", -1);

	result = sPSsetCPUsuspend();

	gtk_text_buffer_get_iter_at_offset(g_buffer, &iter, 0);
	if( result == 0 )
		gtk_text_buffer_insert(g_buffer, &iter, ">>>ok!\n", -1);
	else
		gtk_text_buffer_insert(g_buffer, &iter, ">>>fail!\n", -1);

	return iRet;
}

/*
 * After it's connected to the right event, this will run when
 * the button is pressed
 */
gint Button01Pressed(GtkWidget* widget, gpointer data)
{
	g_print("You pressed the button01.\n");
#ifdef USE_TEXT_VIEW
#else
	AddListItem(g_listbox, "You pressed the button01.");
#endif
	return FALSE;
}


/*
 * After it's connected to the right event, this will run when
 * the button is pressed
 */
gint ButtonXXPressed(GtkWidget* widget, gpointer data)
{
	gchar *name;
#ifdef USE_NEW_BTN
    if( buttonWdgItemList )
    {
    	int iTmp = 0;

    	while( buttonWdgItemList[iTmp].btnId != -1 )
    	{
    		if( buttonWdgItemList[iTmp].btnWidget == widget )
    		{
    			name = buttonWdgItemList[iTmp].btnName;
    			break;
    		}
    		name = "not found!";
    		iTmp++;
    	}

    	if( buttonWdgItemList[iTmp].btnId != -1 )
    	{
    		g_print("call number %d function for button.\n", buttonWdgItemList[iTmp].btnId );

    		switch(buttonWdgItemList[iTmp].btnId)
    		{
    			case 0:
    				go_Button_nofunction();
    				break;
    			case 1:
    				go_Button_nofunction();
    				break;
    			case 2:
    				go_Button_03();
    				break;
    			case 3:
    				go_Button_04();
    				break;
    			case 4:
    				go_Button_05();
    				break;
    			case 5:
    				go_Button_06();
    				break;
    			case 6:
    				go_Button_07();
    				break;
    			case 7:
    				go_Button_08();
    				break;
    			case 8:
    				go_Button_09();
    				break;
    			case 9:
    				go_Button_10();
    				break;
    			case 10:
    				go_Button_11();
    				break;
    			case 11:
    				go_Button_nofunction();
    				break;
    			case 12:
    				go_Button_13();
    				break;
    			case 13:
    				go_Button_nofunction();
    				break;
    			case 14:
    				go_Button_nofunction();
    				break;
    			case 15:
    				go_Button_nofunction();
    				break;

    			default:
    				break;
    		}//switch
    	}
    }
#endif

	g_print("You pressed the %s.\n", name );
#ifdef USE_TEXT_VIEW
#else
	AddListItem(g_listbox, "You pressed the button01.");
#endif
	return FALSE;
}


#ifndef USE_TEXT_VIEW
/*
* listitem_selected
*
* Event handler called when an item is selected.
*/
void listitem_selected (GtkWidget *widget, gpointer *data)
{
    g_print ("item selected - %s\n", data);
}


/*
* GetItemLabel
*
* Returns the label's string given a widget that contains the label.
*/
char *GetItemLabel (GtkWidget *widget)
{
    GtkWidget *label = NULL;
    char *str = NULL;
    GList *children;
    GtkWidget *child;

    /* --- Get list of children in the list item --- */
    children = gtk_container_children (GTK_CONTAINER (widget));

    /* --- Get the widget --- */
    child = GTK_WIDGET (children->data);

    /* --- If the widget is a label --- */
    if (GTK_IS_LABEL (child)) {

        /* --- Get the string in the label --- */
        gtk_label_get (GTK_LABEL (child), &str);
    }

    /* --- Return the string --- */
    return (str);
}


/*
* DisplaySelectedItems
*
* Display the contents of the list items in the listbox.
* Show state and label information
*/
void DisplaySelectedItems (GtkWidget *widget)
{
    GList *children;
    GtkWidget *child;

    /* --- Get children in the listbox (list items) --- */
    children = gtk_container_children (GTK_CONTAINER (widget));

    /* --- Loop through them all --- */
    while (children) {

        /* --- Get child's widget information --- */
        child = GTK_WIDGET (children->data);

        /* --- Display list item information --- */
        printf ("state = %d, label = %s\n",
                (int) GTK_WIDGET_STATE (child),
                GetItemLabel (child));

        /* --- Go to the next child --- */
        children = children->next;
    }
}


/*
* listbox_changed
*
* Event Handler for the listbox
*/
void listbox_changed (GtkWidget *widget, gpointer *data)
{
    GtkList *listbox;

    g_print ("%s\n", data);
    DisplaySelectedItems (widget);
}


/*
* AddListItem
*
* Add the text to the listbox.
*/
void AddListItem (GtkWidget *listbox, char *sText)
{
    GtkWidget *item;

    /* --- Create a list item from the data element --- */
    item = gtk_list_item_new_with_label (sText);

    /* --- Conigure for the "select" event --- */
    gtk_signal_connect (GTK_OBJECT (item), "select", GTK_SIGNAL_FUNC (listitem_selected), sText);

    /* --- Add item to the listbox --- */
    gtk_container_add (GTK_CONTAINER (listbox), item);

    /* --- Make it visible --- */
    gtk_widget_show (item);
}
#endif


int main( int   argc,
          char *argv[] )
{
    GtkWidget *window;
    GtkWidget *hbox1;
    GtkWidget *vbox1;
    GtkWidget *vbox2;
    GtkWidget *vbox3;
    GtkWidget *vbox4;
    GtkWidget *vbox5;
#ifdef USE_TEXT_VIEW
    GtkWidget *textview;
    GtkTextBuffer *buffer;
#else
    GtkWidget *listbox;
#endif
#ifdef USE_SCROLL_WIN
    GtkWidget *scroll_window;
#endif
#ifndef USE_NEW_BTN
    GtkWidget *button01;
    GtkWidget *button02;
    GtkWidget *button03;
    GtkWidget *button04;
    GtkWidget *button05;
    GtkWidget *button06;
    GtkWidget *button07;
    GtkWidget *button08;
    GtkWidget *button09;
    GtkWidget *button10;
    GtkWidget *button11;
    GtkWidget *button12;
    GtkWidget *button13;
    GtkWidget *button14;
    GtkWidget *button15;
    GtkWidget *button16;
#endif

    gtk_init (&argc, &argv);
    sysPowerSRVInit();

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size (GTK_WINDOW(window),300, 300);
    gtk_window_set_position (GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title (GTK_WINDOW(window), "Span's launcher");

    /* hook window delete event*/
    gtk_signal_connect(GTK_OBJECT(window), "delete_event", GTK_SIGNAL_FUNC(Delete), NULL);

    /* create vbox object */
    vbox1 = gtk_vbox_new(TRUE, 2);

    /* assign hbox to window */
    gtk_container_add(GTK_CONTAINER(window), vbox1);

#ifdef USE_TEXT_VIEW
    /* create textview object */
    textview = gtk_text_view_new();
    /* Obtaining the buffer associated with the widget. */
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview));
    g_buffer = buffer;
    /* Set the default buffer text. */
    gtk_text_buffer_set_text (buffer, "Hello Text View!", -1);
#else
    /* create listbox object */
    listbox = gtk_list_new();
    g_listbox = listbox;
    gtk_list_set_selection_mode(GTK_LIST (listbox), GTK_SELECTION_BROWSE);

    gtk_signal_connect (GTK_OBJECT (listbox), "selection_changed", GTK_SIGNAL_FUNC (listbox_changed), "selection_changed");
    AddListItem(listbox, "This is a listbox");
#endif


#ifdef USE_SCROLL_WIN
    /* create a scrollable container */
    scroll_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_window),
                                   GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_ALWAYS);
    ///gtk_container_add(GTK_CONTAINER(frame_window), scroll_window);
	#ifdef USE_TEXT_VIEW
    gtk_container_add(GTK_CONTAINER(scroll_window), textview);
	#else
    gtk_container_add(GTK_CONTAINER(scroll_window), listbox);
	#endif
#endif
    /* create hbox object */
    hbox1 = gtk_hbox_new(TRUE, 4);

    /* assign ... to vbox */
    ///gtk_container_add(GTK_CONTAINER(window), button);
#ifdef USE_SCROLL_WIN
    gtk_box_pack_start(GTK_BOX(vbox1), scroll_window, TRUE, TRUE, 0);
#else
	#ifdef USE_TEXT_VIEW
    gtk_box_pack_start(GTK_BOX(vbox1), textview, TRUE, TRUE, 0);
	#else
    gtk_box_pack_start(GTK_BOX(vbox1), listbox, TRUE, TRUE, 0);
	#endif
#endif

    gtk_box_pack_start(GTK_BOX(vbox1), hbox1, TRUE, TRUE, 0);

    /* create vbox object */
    vbox2 = gtk_vbox_new(TRUE, 4);

    /* create vbox object */
    vbox3 = gtk_vbox_new(TRUE, 4);

    /* create vbox object */
    vbox4 = gtk_vbox_new(TRUE, 4);

    /* create vbox object */
    vbox5 = gtk_vbox_new(TRUE, 4);

    /* assign ... to hbox */
    ///gtk_container_add(GTK_CONTAINER(window), button);
    gtk_box_pack_start(GTK_BOX(hbox1), vbox2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox1), vbox3, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox1), vbox4, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox1), vbox5, TRUE, TRUE, 0);

    /* create button object */
#ifdef USE_NEW_BTN
    if( buttonWdgItemList )
    {
    	int iTmp = 0;

    	while( buttonWdgItemList[iTmp].btnId != -1 )
    	{
    		buttonWdgItemList[iTmp].btnWidget = gtk_button_new_with_label( buttonWdgItemList[iTmp].btnName );
    		g_print("id=%d[%d], create button <%s> ...\n", buttonWdgItemList[iTmp].btnId, iTmp, buttonWdgItemList[iTmp].btnName );
    		iTmp++;
    	}

    }
#else
    	button01 = gtk_button_new_with_label("button 01");
    	button02 = gtk_button_new_with_label("button 02");
    	button03 = gtk_button_new_with_label("button 03");
    	button04 = gtk_button_new_with_label("button 04");
    	button05 = gtk_button_new_with_label("button 05");
    	button06 = gtk_button_new_with_label("button 06");
    	button07 = gtk_button_new_with_label("button 07");
    	button08 = gtk_button_new_with_label("button 08");
    	button09 = gtk_button_new_with_label("button 09");
    	button10 = gtk_button_new_with_label("button 10");
    	button11 = gtk_button_new_with_label("button 11");
    	button12 = gtk_button_new_with_label("button 12");
    	button13 = gtk_button_new_with_label("button 13");
    	button14 = gtk_button_new_with_label("button 14");
    	button15 = gtk_button_new_with_label("button 15");
    	button16 = gtk_button_new_with_label("button 16");
#endif

    /* assign button to vbox */
    ///gtk_container_add(GTK_CONTAINER(window), button);
#ifdef USE_NEW_BTN
    if( buttonWdgItemList )
    {
    	int iTmp = 0;
    	///gint id = 0;

    	while( buttonWdgItemList[iTmp].btnId != -1 )
    	{
    		///id = buttonWdgItemList[iTmp].id;
    		if( iTmp < 4 )
    			gtk_box_pack_start(GTK_BOX(vbox2), buttonWdgItemList[iTmp].btnWidget, TRUE, TRUE, 0);
    		else
   			if( iTmp < 8 )
   				gtk_box_pack_start(GTK_BOX(vbox3), buttonWdgItemList[iTmp].btnWidget, TRUE, TRUE, 0);
    		else
   			if( iTmp < 12 )
   				gtk_box_pack_start(GTK_BOX(vbox4), buttonWdgItemList[iTmp].btnWidget, TRUE, TRUE, 0);
     		else
     			gtk_box_pack_start(GTK_BOX(vbox5), buttonWdgItemList[iTmp].btnWidget, TRUE, TRUE, 0);

    		iTmp++;
    	}

    }
#else
        /* assign button to vbox */
    	gtk_box_pack_start(GTK_BOX(vbox2), button01, TRUE, TRUE, 0);
    	gtk_box_pack_start(GTK_BOX(vbox2), button02, TRUE, TRUE, 0);
    	gtk_box_pack_start(GTK_BOX(vbox2), button03, TRUE, TRUE, 0);
    	gtk_box_pack_start(GTK_BOX(vbox2), button04, TRUE, TRUE, 0);

    	/* assign button to vbox */
    	gtk_box_pack_start(GTK_BOX(vbox3), button05, TRUE, TRUE, 0);
    	gtk_box_pack_start(GTK_BOX(vbox3), button06, TRUE, TRUE, 0);
    	gtk_box_pack_start(GTK_BOX(vbox3), button07, TRUE, TRUE, 0);
    	gtk_box_pack_start(GTK_BOX(vbox3), button08, TRUE, TRUE, 0);

    	/* assign button to vbox */
    	gtk_box_pack_start(GTK_BOX(vbox4), button09, TRUE, TRUE, 0);
    	gtk_box_pack_start(GTK_BOX(vbox4), button10, TRUE, TRUE, 0);
    	gtk_box_pack_start(GTK_BOX(vbox4), button11, TRUE, TRUE, 0);
    	gtk_box_pack_start(GTK_BOX(vbox4), button12, TRUE, TRUE, 0);

    	/* assign button to vbox */
    	gtk_box_pack_start(GTK_BOX(vbox5), button13, TRUE, TRUE, 0);
    	gtk_box_pack_start(GTK_BOX(vbox5), button14, TRUE, TRUE, 0);
    	gtk_box_pack_start(GTK_BOX(vbox5), button15, TRUE, TRUE, 0);
    	gtk_box_pack_start(GTK_BOX(vbox5), button16, TRUE, TRUE, 0);
#endif

    /* hook button clicked event */
#ifndef USE_NEW_BTN
    gtk_signal_connect(GTK_OBJECT(button01), "clicked", GTK_SIGNAL_FUNC(Button01Pressed), NULL);
#else
    ///gtk_signal_connect(GTK_OBJECT(buttonWdgItemList[0].btnWidget), "clicked", GTK_SIGNAL_FUNC(Button01Pressed), NULL);

    if( buttonWdgItemList )
    {
    	int iTmp = 0;
    	while( buttonWdgItemList[iTmp].btnId != -1 )
    	{
    		gtk_signal_connect(GTK_OBJECT(buttonWdgItemList[iTmp].btnWidget), "clicked", GTK_SIGNAL_FUNC(ButtonXXPressed), NULL);
    		iTmp++;
    	}
    }
#endif

#ifdef USE_NEW_BTN
    if( buttonWdgItemList )
    {
    	int iTmp = 0;

    	while( buttonWdgItemList[iTmp].btnId != -1 )
    	{
    		gtk_widget_show( buttonWdgItemList[iTmp].btnWidget );	/* show button */
    		iTmp++;
    	}

    }

#else
    	gtk_widget_show(button01);	/* show button */
    	gtk_widget_show(button02);	/* show button */
    	gtk_widget_show(button03);	/* show button */
    	gtk_widget_show(button04);	/* show button */
    	gtk_widget_show(button05);	/* show button */
    	gtk_widget_show(button06);	/* show button */
    	gtk_widget_show(button07);	/* show button */
    	gtk_widget_show(button08);	/* show button */
    	gtk_widget_show(button09);	/* show button */
    	gtk_widget_show(button10);	/* show button */
    	gtk_widget_show(button11);	/* show button */
    	gtk_widget_show(button12);	/* show button */
    	gtk_widget_show(button13);	/* show button */
    	gtk_widget_show(button14);	/* show button */
    	gtk_widget_show(button15);	/* show button */
    	gtk_widget_show(button16);	/* show button */
#endif


    gtk_widget_show(vbox1);	/* show button */
    gtk_widget_show(hbox1);	/* show button */
    gtk_widget_show(vbox2);	/* show button */
    gtk_widget_show(vbox3);	/* show button */
    gtk_widget_show(vbox4);	/* show button */
    gtk_widget_show(vbox5);	/* show button */
#ifdef USE_SCROLL_WIN
    gtk_widget_show(scroll_window);	/* show button */
#endif
#ifdef USE_TEXT_VIEW
    gtk_widget_show(textview);	/* show button */
#else
    gtk_widget_show(listbox);	/* show button */
#endif
    gtk_widget_show(window);	/* show window */

#if 0
    pthread_create( &dbus_thread_id, NULL, &FakeKeyCall_listen, NULL );
#endif
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE );	///fixed window
    gtk_main ();

    return 0;
}

#endif
