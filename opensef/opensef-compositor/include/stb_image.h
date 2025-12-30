/**
 * stb_image.h - Single-file image loader
 *
 * This is a wrapper that includes stb_image with implementation.
 * stb_image supports: JPEG, PNG, BMP, GIF, TGA, PSD, HDR, PIC, PNM
 *
 * Download from: https://github.com/nothings/stb/blob/master/stb_image.h
 *
 * For now, this is a placeholder. The actual stb_image.h needs to be
 * downloaded and placed here, or linked via the build system.
 */

#ifndef STB_IMAGE_INCLUDED
#define STB_IMAGE_INCLUDED

// When building, download stb_image.h from:
// https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
// and replace this file with it, or include it via CMake

// Placeholder declarations for compilation
#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char stbi_uc;

// Load image from file
stbi_uc *stbi_load(const char *filename, int *x, int *y, int *channels_in_file,
                   int desired_channels);

// Free image memory
void stbi_image_free(void *retval_from_stbi_load);

// Get failure reason
const char *stbi_failure_reason(void);

#ifdef __cplusplus
}
#endif

#endif // STB_IMAGE_INCLUDED
