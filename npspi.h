#ifndef __NPSPI_H__
#define __NPSPI_H__

#include <stddef.h>
#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct _NpSpiContext NpSpiContext;

NpSpiContext *
npspi_new       (size_t const n_pixels);

void
npspi_free      (NpSpiContext *const ctx);

int
npspi_open      (NpSpiContext *const ctx);

void
npspi_set_color (NpSpiContext *const ctx,
                 uint32_t      const pixel,
                 uint32_t      const color);

int
npspi_show      (NpSpiContext *const ctx);

#ifdef __cplusplus
}
#endif

#endif /* __NPSPI_H__ */
