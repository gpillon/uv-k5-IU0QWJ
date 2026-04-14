#!/bin/sh

IMAGE_NAME="uvk5"
FIRMWARE_DIR="${PWD}/compiled-firmware"
BASE="${BASE:-alpine:3.22}"

case "$BASE" in
  alpine:*)  ALPINE_TAG="${BASE#alpine:}";;
  alpine)    ALPINE_TAG="3.22";;
  *)
    echo "BASE must be 'alpine:<tag>'. Got: '$BASE'"
    exit 1
    ;;
esac

mkdir -p "$FIRMWARE_DIR"
rm -f "$FIRMWARE_DIR"/*

echo "Cleaning up old Docker artifacts..."
docker system prune -f --volumes >/dev/null 2>&1 || true

echo "Building Docker image '$IMAGE_NAME' (base=${BASE})..."
docker rmi "$IMAGE_NAME" 2>/dev/null || true
if ! docker build --pull --build-arg "ALPINE_TAG=${ALPINE_TAG}" -t "$IMAGE_NAME" .; then
    echo "Failed to build docker image"
    exit 1
fi

clean() {
    echo "Cleaning all"
    docker rmi "$IMAGE_NAME" 2>/dev/null || true
    docker buildx prune -f || true
    make clean || true
}

# IU0QWJ edition: all features via EEPROM code overlay system
# Spectrum, FM radio, Aircopy loaded from EEPROM on demand
iu0qwj() {
    echo "Compiling IU0QWJ edition (24M02 EEPROM, 999 channels, overlay system)..."
    docker run -v "$FIRMWARE_DIR:/app/compiled-firmware" "$IMAGE_NAME" /bin/bash -c "\
        cd /app && make clean 2>/dev/null; make -s \
        ENABLE_SPECTRUM=1 \
        ENABLE_FMRADIO=1 \
        ENABLE_AIRCOPY=1 \
        ENABLE_NOAA=1 \
        ENABLE_VOX=1 \
        ENABLE_TX1750=1 \
        ENABLE_FLASHLIGHT=1 \
        ENABLE_DTMF_CALLING=1 \
        ENABLE_FEAT_F4HWN_CHARGING_C=1 \
        ENABLE_FEAT_F4HWN_GAME=0 \
        EDITION_STRING=IU0QWJ \
        TARGET=f4hwn \
        && cp f4hwn.bin f4hwn.packed.bin compiled-firmware/ \
        && cp f4hwn.ovly_spectrum.bin f4hwn.ovly_fm.bin f4hwn.ovly_aircopy.bin f4hwn.ovly_menu.bin compiled-firmware/ \
        && echo '' \
        && echo '=== Build Summary ===' \
        && echo \"Flash: \$(stat -c%s f4hwn.bin) / 61440 bytes\" \
        && echo \"Overlay spectrum: \$(stat -c%s f4hwn.ovly_spectrum.bin) bytes\" \
        && echo \"Overlay FM:       \$(stat -c%s f4hwn.ovly_fm.bin) bytes\" \
        && echo \"Overlay aircopy:  \$(stat -c%s f4hwn.ovly_aircopy.bin) bytes\" \
        && echo \"Overlay menu:     \$(stat -c%s f4hwn.ovly_menu.bin) bytes\" \
        && echo '=== Done ==='"
}

case "$1" in
    clean) clean ;;
    iu0qwj|"") iu0qwj ;;
    *)
        echo "Usage: BASE=alpine:<tag> $0 {clean|iu0qwj}"
        echo "Default (no arg) builds iu0qwj edition."
        exit 1
        ;;
esac
