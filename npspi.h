#ifndef __NPSPI_H__
#define __NPSPI_H__

#include <stddef.h>
#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct _NpSpiContext NpSpiContext;

NpSpiContext *
npspi_new       (const size_t n_pixels);

void
npspi_free      (NpSpiContext *const ctx);

int
npspi_open      (NpSpiContext *const ctx);

void
npspi_set_color (NpSpiContext *const ctx,
                 const uint32_t      pixel,
                 const uint32_t      color);

int
npspi_show      (NpSpiContext *const ctx);

int
npspi_close     (NpSpiContext *const ctx);

#ifdef __cplusplus
}
#endif

#endif /* __NPSPI_H__ */
