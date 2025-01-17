#include <gtk/gtk.h>
#include <cairo.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <ctime>
#include <sstream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Reminder {
    std::string title;
    std::string description;
    std::string datetime; // Хранение времени в строковом формате

    // Функция для записи напоминания в бинарный файл
    void write_to_file(std::ofstream &out) const {
        size_t title_len = title.size();
        size_t description_len = description.size();
        size_t datetime_len = datetime.size();

        out.write(reinterpret_cast<const char*>(&title_len), sizeof(title_len));
        out.write(title.c_str(), title_len);

        out.write(reinterpret_cast<const char*>(&description_len), sizeof(description_len));
        out.write(description.c_str(), description_len);

        out.write(reinterpret_cast<const char*>(&datetime_len), sizeof(datetime_len));
        out.write(datetime.c_str(), datetime_len);
    }

    // Функция для чтения напоминания из бинарного файла
    void read_from_file(std::ifstream &in) {
        size_t title_len, description_len, datetime_len;

        in.read(reinterpret_cast<char*>(&title_len), sizeof(title_len));
        title.resize(title_len);
        in.read(&title[0], title_len);

        in.read(reinterpret_cast<char*>(&description_len), sizeof(description_len));
        description.resize(description_len);
        in.read(&description[0], description_len);

        in.read(reinterpret_cast<char*>(&datetime_len), sizeof(datetime_len));
        datetime.resize(datetime_len);
        in.read(&datetime[0], datetime_len);
    }

    // Преобразование времени из строки в формат std::tm для сравнения
    std::tm to_tm() const {
        std::tm time = {};
        std::stringstream ss(datetime);
        ss >> std::get_time(&time, "%Y-%m-%d %H:%M");
        return time;
    }

    // Проверка, наступило ли время напоминания
    bool is_time_to_remind() const {
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm = *std::localtime(&now_time_t);

        std::tm reminder_tm = to_tm();
        return std::difftime(std::mktime(&now_tm), std::mktime(&reminder_tm)) >= 0;
    }
};

std::vector<Reminder> reminders;

// Функции для работы с напоминаниями
void add_reminder(GtkWidget *widget, gpointer data);
void show_reminder_editor(Reminder* reminder, GtkWidget* parent);
void edit_reminder(GtkWidget *widget, gpointer data);
void check_reminders();
void show_reminder_dialog(const Reminder &reminder);

// Функция для загрузки напоминаний из бинарного файла
void load_reminders_from_file(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (file.is_open()) {
        while (file.peek() != EOF) {
            Reminder reminder;
            reminder.read_from_file(file);
            reminders.push_back(reminder);
        }
    } else {
        std::cerr << "Error: Failed to open file for reading." << std::endl;
    }
}

// Функция для сохранения всех напоминаний в бинарный файл
void save_reminders_to_file(const std::string &filename) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        for (const auto &reminder : reminders) {
            reminder.write_to_file(file);
        }
    } else {
        std::cerr << "Error: Failed to open file for writing." << std::endl;
    }
}

static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    std::cout << "on_draw: Start" << std::endl;
    cairo_set_source_rgb(cr, 0.0, 0.5, 1.0);
    cairo_arc(cr, 100, 100, 80, 0, 2 * M_PI);
    cairo_fill(cr);

    Reminder *next_reminder = reinterpret_cast<Reminder*>(data);
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

// Окно для ввода данных нового напоминания
void add_reminder(GtkWidget *widget, gpointer data) {
    GtkWidget *window = GTK_WIDGET(data);  // Это теперь указатель на окно
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Add Reminder", GTK_WINDOW(window), GTK_DIALOG_MODAL, "_Save", GTK_RESPONSE_ACCEPT, "_Cancel", GTK_RESPONSE_REJECT, NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Заголовок
    GtkWidget *title_label = gtk_label_new("Title:");
    gtk_box_pack_start(GTK_BOX(content_area), title_label, FALSE, FALSE, 5);
    GtkWidget *title_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(content_area), title_entry, FALSE, FALSE, 5);

    // Описание
    GtkWidget *description_label = gtk_label_new("Description:");
    gtk_box_pack_start(GTK_BOX(content_area), description_label, FALSE, FALSE, 5);
    GtkWidget *description_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(content_area), description_entry, FALSE, FALSE, 5);

    // Дата и время
    GtkWidget *datetime_label = gtk_label_new("Datetime (YYYY-MM-DD HH:MM):");
    gtk_box_pack_start(GTK_BOX(content_area), datetime_label, FALSE, FALSE, 5);
    GtkWidget *datetime_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(content_area), datetime_entry, FALSE, FALSE, 5);

    gtk_widget_show_all(dialog);

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

    if (result == GTK_RESPONSE_ACCEPT) {
        // Получаем введенные данные
        const char *title = gtk_entry_get_text(GTK_ENTRY(title_entry));
        const char *description = gtk_entry_get_text(GTK_ENTRY(description_entry));
        const char *datetime = gtk_entry_get_text(GTK_ENTRY(datetime_entry));

        // Создаем новое напоминание и добавляем в список
        Reminder new_reminder;
        new_reminder.title = title;
        new_reminder.description = description;
        new_reminder.datetime = datetime;

        reminders.push_back(new_reminder);

        // Обновляем список в интерфейсе
        GtkWidget *reminder_list = GTK_WIDGET(data);
        GtkWidget *row = gtk_list_box_row_new();
        GtkWidget *label = gtk_label_new(new_reminder.title.c_str());
        gtk_container_add(GTK_CONTAINER(row), label);
        gtk_list_box_insert(GTK_LIST_BOX(reminder_list), row, -1);
        gtk_widget_show_all(row);

        // Сохраняем напоминания в файл
        save_reminders_to_file("reminders.dat");
    }

    gtk_widget_destroy(dialog);
}

// Функция для отображения диалогового окна напоминания
void show_reminder_dialog(const Reminder &reminder) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Reminder: %s\n%s", reminder.title.c_str(), reminder.description.c_str());
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Окно для редактирования напоминания
void edit_reminder(GtkWidget *widget, gpointer data) {
    GtkWidget *row = GTK_WIDGET(data);
    GtkWidget *reminder_list = gtk_widget_get_parent(row);  // Получаем список напоминаний
    gint row_index = gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(row));  // Получаем индекс выбранного напоминания

    Reminder *reminder = &reminders[row_index];

    GtkWidget *dialog = gtk_dialog_new_with_buttons("Edit Reminder", GTK_WINDOW(gtk_widget_get_toplevel(widget)), GTK_DIALOG_MODAL, "_Save", GTK_RESPONSE_ACCEPT, "_Cancel", GTK_RESPONSE_REJECT, NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Заголовок
    GtkWidget *title_label = gtk_label_new("Title:");
    gtk_box_pack_start(GTK_BOX(content_area), title_label, FALSE, FALSE, 5);
    GtkWidget *title_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(title_entry), reminder->title.c_str());
    gtk_box_pack_start(GTK_BOX(content_area), title_entry, FALSE, FALSE, 5);

    // Описание
    GtkWidget *description_label = gtk_label_new("Description:");
    gtk_box_pack_start(GTK_BOX(content_area), description_label, FALSE, FALSE, 5);
    GtkWidget *description_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(description_entry), reminder->description.c_str());
    gtk_box_pack_start(GTK_BOX(content_area), description_entry, FALSE, FALSE, 5);

    // Дата и время
    GtkWidget *datetime_label = gtk_label_new("Datetime (YYYY-MM-DD HH:MM):");
    gtk_box_pack_start(GTK_BOX(content_area), datetime_label, FALSE, FALSE, 5);
    GtkWidget *datetime_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(datetime_entry), reminder->datetime.c_str());
    gtk_box_pack_start(GTK_BOX(content_area), datetime_entry, FALSE, FALSE, 5);

    gtk_widget_show_all(dialog);

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

    if (result == GTK_RESPONSE_ACCEPT) {
        // Получаем новые данные
        reminder->title = gtk_entry_get_text(GTK_ENTRY(title_entry));
        reminder->description = gtk_entry_get_text(GTK_ENTRY(description_entry));
        reminder->datetime = gtk_entry_get_text(GTK_ENTRY(datetime_entry));

        // Обновляем список напоминаний
        GtkWidget *label = gtk_bin_get_child(GTK_BIN(row));
        gtk_label_set_text(GTK_LABEL(label), reminder->title.c_str());

        // Сохраняем напоминания в файл
        save_reminders_to_file("reminders.dat");
    }

    gtk_widget_destroy(dialog);
}

// Функция для проверки времени наступления напоминаний
void check_reminders() {
    for (const auto &reminder : reminders) {
        if (reminder.is_time_to_remind()) {
            show_reminder_dialog(reminder);
        }
    }
}

// Главная функция
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    load_reminders_from_file("reminders.dat");

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Reminder App");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *reminder_list = gtk_list_box_new();
    gtk_box_pack_start(GTK_BOX(vbox), reminder_list, TRUE, TRUE, 0);

    GtkWidget *add_button = gtk_button_new_with_label("Add Reminder");
    gtk_box_pack_start(GTK_BOX(vbox), add_button, FALSE, FALSE, 5);
    g_signal_connect(add_button, "clicked", G_CALLBACK(add_reminder), reminder_list);

    for (const auto &reminder : reminders) {
        GtkWidget *row = gtk_list_box_row_new();
        GtkWidget *label = gtk_label_new(reminder.title.c_str());
        gtk_container_add(GTK_CONTAINER(row), label);
        gtk_list_box_insert(GTK_LIST_BOX(reminder_list), row, -1);
        g_signal_connect(row, "activate", G_CALLBACK(edit_reminder), row);
    }

    gtk_widget_show_all(window);

    // Запуск потока для проверки напоминаний
    std::thread reminder_thread([]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::minutes(1));  // Проверяем напоминания раз в минуту
            check_reminders();
        }
    });

    gtk_main();
    reminder_thread.join();

    return 0;
}
