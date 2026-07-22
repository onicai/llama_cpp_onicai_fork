#include "ggml-impl.h"

#include <cstdlib>
#include <exception>

static std::terminate_handler previous_terminate_handler;

GGML_NORETURN static void ggml_uncaught_exception() {
    ggml_print_backtrace();
    if (previous_terminate_handler) {
        previous_terminate_handler();
    }
    abort(); // unreachable unless previous_terminate_handler was nullptr
}

static bool ggml_uncaught_exception_init = []{
    // ICPP-PATCH-START
    // On WASI this lambda runs during C++ static initialization. Static-init
    // ORDER across translation units is unspecified, so it can run before
    // icpp's __wasi_polyfill global ctor initializes the ic-wasi-polyfill.
    // Calling getenv() before that faults ("heap out of bounds", IC0502 at
    // install) rather than safely returning NULL. The terminate/backtrace
    // handler it installs is meaningless in a canister anyway (no TTY; C++
    // exceptions trap via src/wasi-exception-stubs.cpp before std::terminate).
    // So skip the whole thing on WASI. (Native/MockIC keeps it: real libc
    // getenv works at static-init and the handler aids debugging.)
#ifdef __wasi__
    return false;
#else
    const char * GGML_NO_BACKTRACE = getenv("GGML_NO_BACKTRACE");
    if (GGML_NO_BACKTRACE) {
        return false;
    }
    const auto prev{std::get_terminate()};
    GGML_ASSERT(prev != ggml_uncaught_exception);
    previous_terminate_handler = prev;
    std::set_terminate(ggml_uncaught_exception);
    return true;
#endif
    // ICPP-PATCH-END
}();
