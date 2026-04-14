#include "overlay.h"
#include "driver/eeprom.h"
#include "eeprom_map.h"

extern uint8_t _overlay_start[];
extern uint8_t _overlay_end[];

extern uint8_t _ovly_spectrum_size[];
extern uint8_t _ovly_fm_size[];
extern uint8_t _ovly_aircopy_size[];
extern uint8_t _ovly_menu_size[];

volatile OverlayID_t gCurrentOverlay = OVERLAY_NONE;

static const uint32_t ovly_eeprom_addr[] = {
    [OVERLAY_SPECTRUM] = EEPROM_OVLY_SPECTRUM,
    [OVERLAY_FM]       = EEPROM_OVLY_FM,
    [OVERLAY_AIRCOPY]  = EEPROM_OVLY_AIRCOPY,
    [OVERLAY_MENU]     = EEPROM_OVLY_MENU,
};

bool OVERLAY_Load(OverlayID_t id)
{
    if (id == gCurrentOverlay)
        return true;

    if (id == OVERLAY_NONE || id >= OVERLAY_COUNT)
        return false;

    uint32_t eeprom_addr = ovly_eeprom_addr[id];

    /* Each overlay slot starts with a 4-byte magic (OVERLAY_MAGIC).
       If the magic is missing the overlay hasn't been written yet. */
    uint32_t magic;
    EEPROM_ReadBuffer(eeprom_addr, &magic, 4);
    if (magic != OVERLAY_MAGIC) {
        gCurrentOverlay = OVERLAY_NONE;
        return false;
    }
    eeprom_addr += 4;  /* skip past magic */

    uint32_t size;
    switch (id) {
        case OVERLAY_SPECTRUM: size = (uint32_t)_ovly_spectrum_size; break;
        case OVERLAY_FM:       size = (uint32_t)_ovly_fm_size;       break;
        case OVERLAY_AIRCOPY:  size = (uint32_t)_ovly_aircopy_size;  break;
        case OVERLAY_MENU:     size = (uint32_t)_ovly_menu_size;     break;
        default:               return false;
    }

    uint8_t *dst = _overlay_start;
    uint32_t remaining = size;

    while (remaining > 0) {
        uint16_t chunk = (remaining > 128) ? 128 : (uint16_t)remaining;
        EEPROM_ReadBuffer(eeprom_addr, dst, chunk);
        eeprom_addr += chunk;
        dst += chunk;
        remaining -= chunk;
    }

    gCurrentOverlay = id;
    return true;
}
