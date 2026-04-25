#include "recompui/i18n.h"

namespace {
    recompui::TranslationFn g_fn = nullptr;
}

namespace recompui {
    void set_translation_fn(TranslationFn fn) {
        g_fn = std::move(fn);
    }

    std::string tr(std::string_view key, std::string_view fallback) {
        if (g_fn) {
            return g_fn(key, fallback);
        }
        return std::string(fallback);
    }
}
