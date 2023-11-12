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

// Function to execute the warp-cli status command and return the result
std::string getWarpStatus() {
    FILE* fp = popen("warp-cli status | grep -Eio '(Connected|Disconnected|Network)'", "r");
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


// Function to update the system tray icon based on the warp-cli status
void updateIcon() {
    std::string status = getWarpStatus();
    GIcon* icon = nullptr;

    if (status == "Connected") {
        icon = g_themed_icon_new("warp-connected"); // Adjust the icon name
    } else if (status == "Disconnected") {
        icon = g_themed_icon_new("warp-disconnected"); // Adjust the icon name
    } else if (status == "Network") {
        icon = g_themed_icon_new("warp-nonetwork"); // Adjust the icon name
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



int main(int argc, char* argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create an AppIndicator
    indicator = app_indicator_new("warp-indicator", "indicator-messages", APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
    app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);

    // Create a menu
    menu = gtk_menu_new();

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
