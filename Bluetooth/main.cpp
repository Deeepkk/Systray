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
GtkWidget* menuItemToggle;

std::string getBluetoothStatus() {
    FILE* fp = popen("bluetoothctl show | grep -Eio '(Powered: yes|Powered: no)'", "r");
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

    return result;
}


void updateIcon() {
    std::string status = getBluetoothStatus();
    GIcon* icon = nullptr;

    if (status == "Powered: yes") {
        icon = g_themed_icon_new("bluetooth-online"); // Adjust the icon name
    } else if (status == "Powered: no") {
        icon = g_themed_icon_new("bluetooth-offline"); // Adjust the icon name
    } else {
        icon = g_themed_icon_new("gnome-fs-loading-icon"); // Adjust the icon name
    }

    // Convert GIcon to string
    gchar* iconPath = g_icon_to_string(icon);

    // Set the icon using the GTK icon theme
    app_indicator_set_icon_full(indicator, iconPath, nullptr);

    // Free resources
    g_free(iconPath);
    g_object_unref(icon);
}

// Timer callback to periodically update the system tray icon
gboolean timerCallback(gpointer data) {
    updateIcon();
    return G_SOURCE_CONTINUE;
}

void menuItemToggleCallback(GtkWidget* /*widget*/, gpointer /*data*/) {
    std::string status = getBluetoothStatus();
    
    if (status == "Powered: yes") {
        // If connected, run the disconnect command
        system("bluetoothctl power off");
    } else if (status == "Powered: no") {
        // If disconnected, run the connect command
        system("bluetoothctl power on");
    }

    // Update the icon after the connect/disconnect operation
    updateIcon();
}


void menuItemBLManagerCallback(GtkWidget *widget, gpointer data) {
    // Execute the disconnect command
    system("/home/deepak/.config/rofi/rofi-bl");
}

int main(int argc, char* argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create an AppIndicator
    indicator = app_indicator_new("warp-indicator", "indicator-messages", APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
    app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);

    // Create a menu
    GtkWidget *menu = gtk_menu_new();
    GtkWidget *menuItem = gtk_menu_item_new_with_label("Toggle Connection");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
    g_signal_connect(G_OBJECT(menuItem), "activate", G_CALLBACK(menuItemToggleCallback), NULL);
    
    GtkWidget *blmanagermenuItem = gtk_menu_item_new_with_label("Manager");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), blmanagermenuItem);
    g_signal_connect(G_OBJECT(blmanagermenuItem), "activate", G_CALLBACK(menuItemBLManagerCallback), NULL);
    
    
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
