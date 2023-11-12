#include <gtk/gtk.h>
#include <libappindicator/app-indicator.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <algorithm>
#include <unistd.h>

// Global variables
AppIndicator* indicator;
GtkWidget* menu;
GtkWidget* label;

std::string isPlugged() {
    FILE* fp = popen(" upower -i /org/freedesktop/UPower/devices/battery_BAT0 | awk '/state/ {print $2}'", "r");
    if (fp == nullptr) {
        perror("Error executing command");
        exit(EXIT_FAILURE);
    }

    char buffer[128];
    std::string result = "";

    while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
        result += buffer;
    }

    pclose(fp);

    result.erase(std::remove_if(result.begin(), result.end(), [](char c) { return c == '\n'; }), result.end());

    return result;
}

std::string getBatteryStatus() {
    FILE* fp = popen("upower -i /org/freedesktop/UPower/devices/battery_BAT0 | awk '/percentage/ {print $2}' | sed 's/%//'", "r");
    if (fp == nullptr) {
        perror("Error executing command");
        exit(EXIT_FAILURE);
    }

    char buffer[128];
    std::string result = "";

    while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
        result += buffer;
    }

    pclose(fp);

    // Remove newline characters from the result
    result.erase(std::remove_if(result.begin(), result.end(), [](char c) { return c == '\n'; }), result.end());

    // If the result is empty, return -1
    if (result.empty()) {
        return "-1";
    }

    return result;
}



void updateIcon() {
    std::string status = getBatteryStatus();
    std::string charge = isPlugged();

    GIcon* icon = nullptr;
 int numericStatus = std::stoi(status);
    if (charge == "charging") {
        if  (numericStatus >= 0 && numericStatus <= 10) {
        icon = g_themed_icon_new("battery-010-plugged");
    } else if (numericStatus >= 11 && numericStatus <= 30) {
        icon = g_themed_icon_new("battery-030-plugged");
    } else if (numericStatus >= 31 && numericStatus <= 50) {
        icon = g_themed_icon_new("battery-050-plugged");
    } else if (numericStatus >= 51 && numericStatus <= 70) {
        icon = g_themed_icon_new("battery-070-plugged");
    } else if (numericStatus >= 71 && numericStatus <= 90) {
        icon = g_themed_icon_new("battery-90-plugged");
    } else if (numericStatus >= 91 && numericStatus <= 100) {
        icon = g_themed_icon_new("battery-100-plugged");
    } else {
        icon = g_themed_icon_new("battery-action");
    }
    } else if (charge == "fully-charged")
    {
        icon = g_themed_icon_new("battery-100-plugged");
    }
    

      else {
    if (numericStatus >= 0 && numericStatus <= 10) {
        icon = g_themed_icon_new("battery-010");
    } else if (numericStatus >= 11 && numericStatus <= 30) {
        icon = g_themed_icon_new("battery-030");
    } else if (numericStatus >= 31 && numericStatus <= 50) {
        icon = g_themed_icon_new("battery-050");
    } else if (numericStatus >= 51 && numericStatus <= 70) {
        icon = g_themed_icon_new("battery-070");
    } else if (numericStatus >= 71 && numericStatus <= 90) {
        icon = g_themed_icon_new("battery-90");
    } else if (numericStatus >= 91 && numericStatus <= 100) {
        icon = g_themed_icon_new("battery-100");
    }  else {
        icon = g_themed_icon_new("battery-action");
    }

    }
    // Convert GIcon to string
    gchar* iconPath = g_icon_to_string(icon);

    // Set the icon using the GTK icon theme
    app_indicator_set_icon_full(indicator, iconPath, nullptr);
     std::string labelText = status + "%";
    gtk_label_set_text(GTK_LABEL(label), labelText.c_str());

    // Free resources
    g_free(iconPath);
    g_object_unref(icon);
}

// Timer callback to periodically update the system tray icon
gboolean timerCallback(gpointer data) {
    updateIcon();
    return G_SOURCE_CONTINUE;
}





int main(int argc, char* argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create an AppIndicator
    indicator = app_indicator_new("battery-indicator", "indicator-messages", APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
    app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);

    // Create a menu
    GtkWidget *menu = gtk_menu_new();

  GtkWidget* menuItem = gtk_menu_item_new();
    label = gtk_label_new("");
    gtk_container_add(GTK_CONTAINER(menuItem), label);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);


gtk_widget_show_all(menu);



    // Set the menu for the indicator
    app_indicator_set_menu(indicator, GTK_MENU(menu));

    // Update the icon initially
    updateIcon();

    // Set up a timer to periodically update the icon (every 2 seconds)
    guint timer = g_timeout_add_seconds(2, timerCallback, nullptr);

    // Start the GTK main loop
    gtk_main();

    // Clean up
    g_source_remove(timer);
    g_object_unref(G_OBJECT(indicator));

    return 0;
}
