#ifndef _RECOMP_UI_I18N_H_
#define _RECOMP_UI_I18N_H_

#include <functional>
#include <string>
#include <string_view>

// Optional translation hook for embedders (e.g. project-specific locale systems).
//
// The library has user-visible English strings sprinkled throughout. Embedders
// (such as BanjoRecomp) provide a callback that maps a stable key to the
// translated string. If no callback is registered, or the key is unknown to the
// embedder, the English fallback is returned and the library behaves as before.
//
// Keys are lowercase, dot-separated, and namespaced under "recompui.*" so they
// don't collide with embedder-defined keys.
namespace recompui {
    // Signature: (key, english_fallback) -> translated_string.
    // If the embedder has no translation for `key`, it should return `fallback`.
    using TranslationFn = std::function<std::string(std::string_view key, std::string_view fallback)>;

    // Register the translation callback. Pass nullptr to clear.
    // Safe to call before or after any UI is built. Existing widgets are NOT
    // automatically retranslated — embedders are expected to either rebuild
    // affected containers or restart the process.
    void set_translation_fn(TranslationFn fn);

    // Translate a key. Returns the embedder's translation if registered, or the
    // English fallback otherwise. Callers should always provide a non-empty
    // fallback so the UI degrades gracefully when no callback is set.
    std::string tr(std::string_view key, std::string_view fallback);
}

#endif // _RECOMP_UI_I18N_H_
