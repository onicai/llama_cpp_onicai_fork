#include "ggml-threading.h"
// #include <mutex>

// std::mutex ggml_critical_section_mutex;

void ggml_critical_section_start() {
    //ICPP-no-threading In a single-threaded context, this is a no-op.
    // ggml_critical_section_mutex.lock();
}

void ggml_critical_section_end(void) {
    //ICPP-no-threading In a single-threaded context, this is a no-op.
    // ggml_critical_section_mutex.unlock();
}
