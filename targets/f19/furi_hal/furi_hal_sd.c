#include <furi_hal_sd.h>
#include <string.h>

void furi_hal_sd_presence_init(void) {}
bool furi_hal_sd_is_present(void) { return false; }
uint8_t furi_hal_sd_max_mount_retry_count(void) { return 0; }
FuriStatus furi_hal_sd_init(bool power_reset) { (void)power_reset; return FuriStatusError; }
FuriStatus furi_hal_sd_read_blocks(uint32_t* buff, uint32_t sector, uint32_t count) { (void)buff; (void)sector; (void)count; return FuriStatusError; }
FuriStatus furi_hal_sd_write_blocks(const uint32_t* buff, uint32_t sector, uint32_t count) { (void)buff; (void)sector; (void)count; return FuriStatusError; }
FuriStatus furi_hal_sd_info(FuriHalSdInfo* info) { if(info) memset(info, 0, sizeof(*info)); return FuriStatusError; }
FuriStatus furi_hal_sd_get_card_state(void) { return FuriStatusError; }
