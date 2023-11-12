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


std::string isWifiEnabled() {
    FILE* fp = popen("nmcli radio wifi | grep -E '(enabled|disabled)'", "r");
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

    // Check if the result is "enabled"
    return result;
}

std::string getWifiStrength() {
 std::string wifiStatus = isWifiEnabled();
    if (wifiStatus == "disabled") {
        // Wi-Fi is disabled, return a different icon or status
        return "-2";
    }

    FILE* fp = popen("nmcli -t -f IN-USE,SIGNAL dev wifi list | grep -E '^\\*|yes' | awk -F: '{print $2}'", "r");
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



// Function to update the system tray icon based on the warp-cli status
void updateIcon() {
    std::string status = getWifiStrength();
    GIcon* icon = nullptr;
 int numericStatus = std::stoi(status);
    if (status == "-1") {
        icon = g_themed_icon_new("network-wireless-no-route"); // Adjust the icon name
    } else if (numericStatus >= 0 && numericStatus < 25) {
        icon = g_themed_icon_new("network-wireless-connected-00");
    } else if (numericStatus >= 25 && numericStatus < 45) {
        icon = g_themed_icon_new("network-wireless-connected-25");
    } else if (numericStatus >= 45 && numericStatus < 70) {
        icon = g_themed_icon_new("network-wireless-connected-50");
    } else if (numericStatus >= 70 && numericStatus < 90) {
        icon = g_themed_icon_new("network-wireless-connected-75");
    } else if (numericStatus >= 90 && numericStatus <= 100) {
        icon = g_themed_icon_new("network-wireless-connected-100");
    }else {
        icon = g_themed_icon_new("network-wireless-disconnected");
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
    std::string status = isWifiEnabled();
    
    if (status == "enabled") {
        system("nmcli radio wifi off");
    } else{
        system("nmcli radio wifi on");
    }

    // Update the icon after the connect/disconnect operation
    updateIcon();
}

void menuItemDisconnectCallback(GtkWidget *widget, gpointer data) {
    // Execute the disconnect command
    system("nmcli d disconnect wlan0");
     // Update the icon after the connect/disconnect operation
    updateIcon();
}


int main(int argc, char* argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create an AppIndicator
    indicator = app_indicator_new("warp-indicator", "indicator-messages", APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
    app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);

    // Create a menu
    GtkWidget *menu = gtk_menu_new();
    GtkWidget *menuItem = gtk_menu_item_new_with_label("Wifi on/off");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
    g_signal_connect(G_OBJECT(menuItem), "activate", G_CALLBACK(menuItemToggleCallback), NULL);
    
    GtkWidget *disconnectMenuItem = gtk_menu_item_new_with_label("Disconnect");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), disconnectMenuItem);
    g_signal_connect(G_OBJECT(disconnectMenuItem), "activate", G_CALLBACK(menuItemDisconnectCallback), NULL);
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
