#pragma once

#include <functional>
#include <filesystem>
#include <vector>
#include <optional>
#include <list>
#include <string>

namespace recompui {
    namespace file {
        std::filesystem::path get_app_folder_path();
        std::filesystem::path get_program_path();
        std::filesystem::path get_asset_path(const char* asset);

        // Optional override for the program path. Used on Android where the
        // assets are extracted from the APK to internal storage and the
        // program "directory" therefore lives at a runtime-determined location.
        // Must be called before recompui creates its UIState.
        void set_program_path_override(const std::filesystem::path& path);
        void open_file_dialog(std::function<void(bool success, const std::filesystem::path& path)> callback);
        void open_file_dialog_multiple(std::function<void(bool success, const std::list<std::filesystem::path>& paths)> callback);
        void show_error_message_box(const char *title, const char *message);
    
        // Apple specific methods that usually require Objective-C. Implemented in support_apple.mm.
        #ifdef __APPLE__
        namespace apple {
            void dispatch_on_ui_thread(std::function<void()> func);
            std::optional<std::filesystem::path> get_application_support_directory();
            std::filesystem::path get_bundle_resource_directory();
            std::filesystem::path get_bundle_directory();
        }
        #endif
    }
}
