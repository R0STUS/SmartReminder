#include <gtk/gtk.h>
#include <cairo.h>
#include <vector>
#include <iostream>
#include <string>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Reminder {
    std::string title;
    std::string description;
    std::string datetime;
};

std::vector<Reminder> reminders;

void add_reminder(GtkWidget *widget, gpointer data);
void show_reminder_editor(Reminder* reminder, GtkWidget* parent);

static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    std::cout << "on_draw: Start" << std::endl;
    cairo_set_source_rgb(cr, 0.0, 0.5, 1.0);
    cairo_arc(cr, 100, 100, 80, 0, 2 * M_PI);
    cairo_fill(cr);
    Reminder *next_reminder = static_cast<Reminder*>(data);
    if (next_reminder) {
        std::cout << "Drawing reminder: " << next_reminder->title << std::endl;
        cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
        cairo_set_font_size(cr, 24);
        cairo_move_to(cr, 50, 105);
        cairo_show_text(cr, next_reminder->title.c_str());
    } else {
        std::cerr << "Error: No reminder to draw." << std::endl;
    }
    std::cout << "on_draw: End" << std::endl;
    return FALSE;
}

void add_reminder(GtkWidget *widget, gpointer data) {
    GtkWidget *list = GTK_WIDGET(data);
    std::cout << "add_reminder: Start" << std::endl;
    std::cout << "Current reminders.size() = " << reminders.size() << std::endl;
    try {
        Reminder new_reminder = {"New Reminder", "Description", "2038-01-19 3:14"};
        reminders.push_back(new_reminder);
        std::cout << "New reminder created: " << new_reminder.title << ", " << new_reminder.description << ", " << new_reminder.datetime << std::endl;
        GtkWidget *row = gtk_list_box_row_new();
        GtkWidget *label = gtk_label_new(new_reminder.title.c_str());
        if (!row || !label) {
            std::cerr << "Error: Failed to create GtkListBoxRow or GtkLabel." << std::endl;
            return;
        }
        gtk_container_add(GTK_CONTAINER(row), label);
        if (!list) {
            std::cerr << "Error: Reminder list is null." << std::endl;
            return;
        }
        gtk_list_box_insert(GTK_LIST_BOX(list), row, -1);
        gtk_widget_show_all(row);
        std::cout << "Reminder added to list box." << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Exception in add_reminder: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error in add_reminder." << std::endl;
    }
    std::cout << "add_reminder: End" << std::endl;
}

void show_reminder_editor(Reminder* reminder, GtkWidget* parent) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons( "Edit Reminder", GTK_WINDOW(parent), static_cast<GtkDialogFlags>(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT), "_Close", GTK_RESPONSE_CLOSE, NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *title_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(title_entry), reminder->title.c_str());
    gtk_box_pack_start(GTK_BOX(content_area), title_entry, FALSE, FALSE, 5);
    GtkWidget *description_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(description_entry), reminder->description.c_str());
    gtk_box_pack_start(GTK_BOX(content_area), description_entry, FALSE, FALSE, 5);
    GtkWidget *datetime_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(datetime_entry), reminder->datetime.c_str());
    gtk_box_pack_start(GTK_BOX(content_area), datetime_entry, FALSE, FALSE, 5);
    gtk_widget_show_all(dialog);
    g_signal_connect(dialog, "response", G_CALLBACK(gtk_widget_destroy), NULL);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Smart Reminder");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), main_box);
    GtkWidget *circle_box = gtk_drawing_area_new();
    gtk_widget_set_size_request(circle_box, 200, 200);
    gtk_box_pack_start(GTK_BOX(main_box), circle_box, FALSE, FALSE, 5);
    GtkWidget *reminder_list = gtk_list_box_new();
    gtk_box_pack_start(GTK_BOX(main_box), reminder_list, TRUE, TRUE, 5);
    GtkWidget *add_button = gtk_button_new_with_label("Add Reminder");
    g_signal_connect(add_button, "clicked", G_CALLBACK(add_reminder), reminder_list);
    gtk_box_pack_start(GTK_BOX(main_box), add_button, FALSE, FALSE, 5);
    reminders.push_back({"Meeting", "Discuss project", "2038-01-19 3:14"});
    Reminder *next_reminder = &reminders[0];
    g_signal_connect(circle_box, "draw", G_CALLBACK(on_draw), next_reminder);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}