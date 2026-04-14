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

/* Overlay header: 8 bytes total.
   Bytes 0-3: "OVLY" as little-endian uint32_t (0x594C564F)
   Bytes 4-7: build signature (must match firmware's _etext address)
   This ensures old overlays compiled against a different firmware layout
   are automatically rejected, preventing crashes from stale function pointers. */
#define OVERLAY_MAGIC  0x594C564F

extern volatile OverlayID_t gCurrentOverlay;

bool OVERLAY_Load(OverlayID_t id);

/* Build signature derived from firmware layout; defined in overlay.c */
uint32_t OVERLAY_GetBuildSig(void);

#endif
