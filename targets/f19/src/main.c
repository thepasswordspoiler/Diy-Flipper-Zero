#include <furi.h>
#include <furi_hal.h>
#include <flipper.h>

int32_t init_task(void* context) {
    UNUSED(context);
    furi_hal_init();
    flipper_init();
    furi_background();
    return 0;
}

int main(void) {
    furi_init();
    furi_hal_init_early();
    FuriThread* main_thread = furi_thread_alloc_ex("InitSrv", 1024, init_task, NULL);
    furi_thread_set_priority(main_thread, FuriThreadPriorityInit);
    furi_thread_start(main_thread);
    furi_run();
    furi_crash("Kernel is Dead");
}

void Error_Handler(void) { furi_crash("ErrorHandler"); }
void abort(void) { furi_crash("AbortHandler"); }
