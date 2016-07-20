#include "npspi.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <mpsse.h>

#define MHZ_20 ((unsigned int) 20e6)

struct _NpSpiContext {

  struct mpsse_context *mpsse_ctx;
  uint8_t              *pixels;
  size_t                n_pixels;

};

NpSpiContext *
npspi_new (const size_t n_pixels)
{

  NpSpiContext *const ctx = calloc (1, sizeof (NpSpiContext));

  memset (ctx, 0, sizeof (NpSpiContext));

  ctx->n_pixels = n_pixels;
  ctx->pixels = calloc (n_pixels * 3, sizeof (uint8_t));

  return ctx;

}

int
npspi_open (NpSpiContext *const self)
{

  self->mpsse_ctx = MPSSE (SPI0, MHZ_20, 0);

  return 0;

}

void
npspi_set_color (NpSpiContext *const self,
                 const uint32_t      pixel,
                 const uint32_t      color)
{
}

int
npspi_show (NpSpiContext *const self)
{

  struct mpsse_context *const mpsse = self->mpsse_ctx;

  Start (mpsse);
  Write (mpsse, (char *) self->pixels, (int) self->n_pixels);
  Stop (mpsse);

  return 0;

}

int
npspi_close (NpSpiContext *const self)
{

  Close (self->mpsse_ctx);

  return 0;

}
