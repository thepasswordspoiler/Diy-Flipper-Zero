#include <furi_hal_version.h>

bool furi_hal_version_do_i_belong_here(void) {
    return furi_hal_version_get_hw_target() == 19;
}

const char* furi_hal_version_get_model_name(void) {
    return "UBYTE STM32WB55";
}

const char* furi_hal_version_get_model_code(void) {
    return "UBYTE.1";
}

const char* furi_hal_version_get_fcc_id(void) {
    return "N/A";
}

const char* furi_hal_version_get_ic_id(void) {
    return "N/A";
}

const char* furi_hal_version_get_mic_id(void) {
    return "N/A";
}

const char* furi_hal_version_get_srrc_id(void) {
    return "N/A";
}

const char* furi_hal_version_get_ncc_id(void) {
    return "N/A";
}
