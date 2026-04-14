#ifndef OVERLAY_H
#define OVERLAY_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    OVERLAY_NONE = 0,
    OVERLAY_SPECTRUM,
    OVERLAY_FM,
    OVERLAY_AIRCOPY,
    OVERLAY_MENU,
    OVERLAY_COUNT
} OverlayID_t;

/* "OVLY" as little-endian uint32_t: O=0x4F is LSB, Y=0x59 is MSB */
#define OVERLAY_MAGIC  0x594C564F

extern volatile OverlayID_t gCurrentOverlay;

bool OVERLAY_Load(OverlayID_t id);

#endif
