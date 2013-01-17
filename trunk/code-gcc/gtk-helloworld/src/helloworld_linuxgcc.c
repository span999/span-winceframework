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
gint ButtonPressed(GtkWidget* widget, gpointer data)
{
	g_print("You pressed the button.\n");
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
    GtkWidget *hbox;
    GtkWidget *vbox;
    GtkWidget *listbox;
    GtkWidget *button1;
    GtkWidget *button2;
    GtkWidget *button3;
    GtkWidget *button4;

    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size (GTK_WINDOW(window),300, 120);
    gtk_window_set_position (GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title (GTK_WINDOW(window), "Span's launcher");

    /* hook window delete event*/
    gtk_signal_connect(GTK_OBJECT(window), "delete_event", GTK_SIGNAL_FUNC(Delete), NULL);

    /* create hbox object */
    hbox = gtk_hbox_new(TRUE, 2);

    /* assign hbox to window */
    gtk_container_add(GTK_CONTAINER(window), hbox);

    /* create listbox object */
    listbox = gtk_list_new();

    AddListItem(listbox, "This is a listbox");

    /* create vbox object */
    vbox = gtk_vbox_new(TRUE, 4);

    /* assign ... to hbox */
    ///gtk_container_add(GTK_CONTAINER(window), button);
    gtk_box_pack_start(GTK_BOX(hbox), listbox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, TRUE, 0);

    /* create button object */
    button1 = gtk_button_new_with_label("hello world1");
    button2 = gtk_button_new_with_label("hello world2");
    button3 = gtk_button_new_with_label("hello world3");
    button4 = gtk_button_new_with_label("hello world4");

    /* assign button to vbox */
    ///gtk_container_add(GTK_CONTAINER(window), button);
    gtk_box_pack_start(GTK_BOX(vbox), button1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), button2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), button3, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), button4, TRUE, TRUE, 0);

    /* hook button clicked event */
    gtk_signal_connect(GTK_OBJECT(button1), "clicked", GTK_SIGNAL_FUNC(ButtonPressed), NULL);

    gtk_widget_show(button1);	/* show button */
    gtk_widget_show(button2);	/* show button */
    gtk_widget_show(button3);	/* show button */
    gtk_widget_show(button4);	/* show button */
    gtk_widget_show(vbox);	/* show button */
    gtk_widget_show(hbox);	/* show button */
    gtk_widget_show(listbox);	/* show button */
    gtk_widget_show(window);	/* show window */

    gtk_main ();

    return 0;
}

#endif
