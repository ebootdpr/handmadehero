#include <gtk/gtk.h>

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);  // Initialize GTK 
    GtkWidget *window;  // Create a pointer to hold the window widget
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);  // Create a new top-level window
    
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);  // Handle window close event
    
    gtk_widget_show_all(window);  // Show the window and its contents
    
    gtk_main();  // Enter the GTK main loop
    
    return 0;
}

