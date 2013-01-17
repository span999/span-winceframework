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
*/

#include <gtk/gtk.h>



void AddListItem (GtkWidget *listbox, char *sText);


static GtkWidget *g_listbox;


/*
 * After it's connected to the right event, this will run when
 * the window is closed
 */
gint Delete(GtkWidget* widget, gpointer* data)
{
	g_print("Screw you guys, I'm going home!\n");
	gtk_main_quit();                               /* stop gtk_main */
	return FALSE;                                  /* Kill the window */
}

/*
 * After it's connected to the right event, this will run when
 * the button is pressed
 */
gint Button01Pressed(GtkWidget* widget, gpointer data)
{
	g_print("You pressed the button01.\n");
	AddListItem(g_listbox, "You pressed the button01.");
	return FALSE;
}


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
    GtkWidget *listbox;
    GtkWidget *scroll_window;
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

    gtk_init (&argc, &argv);

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

    /* create listbox object */
    listbox = gtk_list_new();
    g_listbox = listbox;
    gtk_list_set_selection_mode(GTK_LIST (listbox), GTK_SELECTION_BROWSE);

    gtk_signal_connect (GTK_OBJECT (listbox), "selection_changed", GTK_SIGNAL_FUNC (listbox_changed), "selection_changed");
    AddListItem(listbox, "This is a listbox");
#if 0
    /* create a scrollable container */
    scroll_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_window),
                                   GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_ALWAYS);
    ///gtk_container_add(GTK_CONTAINER(frame_window), scroll_window);
    gtk_container_add(GTK_CONTAINER(scroll_window), listbox);
#endif
    /* create hbox object */
    hbox1 = gtk_hbox_new(TRUE, 4);

    /* assign ... to vbox */
    ///gtk_container_add(GTK_CONTAINER(window), button);
#if 0
    gtk_box_pack_start(GTK_BOX(vbox1), scroll_window, TRUE, TRUE, 0);
#else
    gtk_box_pack_start(GTK_BOX(vbox1), listbox, TRUE, TRUE, 0);
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

    /* assign button to vbox */
    ///gtk_container_add(GTK_CONTAINER(window), button);
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

    /* hook button clicked event */
    gtk_signal_connect(GTK_OBJECT(button01), "clicked", GTK_SIGNAL_FUNC(Button01Pressed), NULL);

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
    gtk_widget_show(vbox1);	/* show button */
    gtk_widget_show(hbox1);	/* show button */
    gtk_widget_show(vbox2);	/* show button */
    gtk_widget_show(vbox3);	/* show button */
    gtk_widget_show(vbox4);	/* show button */
    gtk_widget_show(vbox5);	/* show button */
    gtk_widget_show(listbox);	/* show button */
    gtk_widget_show(window);	/* show window */

    gtk_main ();

    return 0;
}

#endif
