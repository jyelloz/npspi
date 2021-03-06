#include "npspi.h"
#include "nplut.h"
#include "npgammalut.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <mpsse.h>

typedef enum {
  MHZ_30 = (unsigned int) 30e6,
  MHZ_20 = (unsigned int) 20e6,
  MHZ_06 = (unsigned int)  6e6,
} Hz;

struct _NpSpiContext {

  struct mpsse_context *mpsse_ctx;
  uint8_t              *fb;
  size_t                n_pixels;

  uint8_t              *spi_buffer;
  size_t                n_spi_bytes;

};

NpSpiContext *
npspi_new (size_t const n_pixels)
{

  NpSpiContext *const ctx = calloc (1, sizeof (NpSpiContext));

  memset (ctx, 0, sizeof (NpSpiContext));

  size_t const n_bytes = n_pixels * 3;
  size_t const n_spi_bytes = n_bytes * NPLUT_RATIO;

  ctx->n_pixels = n_pixels;
  ctx->fb = calloc (n_bytes, sizeof (uint8_t));

  memset (ctx->fb, 0x00, n_bytes);

  ctx->spi_buffer = calloc (n_spi_bytes, sizeof (uint8_t));
  ctx->n_spi_bytes = n_spi_bytes;

  return ctx;

}

void
npspi_free (NpSpiContext *const self)
{

  free (self->fb);
  free (self->spi_buffer);

  Close (self->mpsse_ctx);

  free (self);

}

int
npspi_open (NpSpiContext *const self)
{

  self->mpsse_ctx = MPSSE (SPI0, MHZ_20, MSB);

  return 0;

}

void
npspi_set_color (NpSpiContext *const self,
                 uint32_t      const pixel,
                 uint32_t      const color)
{

  size_t const offset = pixel * 3;
  uint8_t *const base = &(self->fb[offset]);

  base[0] = npgammalut_get (0xff & (color >> 8));
  base[1] = npgammalut_get (0xff & (color >> 16));
  base[2] = npgammalut_get (0xff & (color >> 0));

}

int
npspi_show (NpSpiContext *const self)
{

  size_t const n_bytes = self->n_pixels * 3;
  uint8_t const *const fb = self->fb;
  struct mpsse_context *const mpsse = self->mpsse_ctx;

  uint8_t *const spi_buffer = self->spi_buffer;
  size_t const n_spi_bytes = self->n_spi_bytes;

  for (int i = 0; i < n_bytes; i++) {
    uint8_t const *const byte_wave = nplut_get (fb[i]);
    memcpy (&(spi_buffer[i * NPLUT_RATIO]), byte_wave, NPLUT_RATIO);
  }

  Start (mpsse);
  Write (mpsse, (char *) spi_buffer, n_spi_bytes);
  Stop (mpsse);

  return 0;

}
