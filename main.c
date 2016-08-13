#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <ev.h>
#include "npspi.h"

#define FPS 30
#define PERIOD (1.0 / FPS)
#ifndef N_PIXELS
#define N_PIXELS 64
#endif
#define BRIGHTNESS_MIN 0
#define BRIGHTNESS_MAX 0.4

struct animation_ctx {

  ev_timer timer;
  NpSpiContext *npspi_ctx;

};

static void
frame_cb (EV_P_ ev_timer *const timer, int const revents)
{

  struct animation_ctx *const animation = (struct animation_ctx *) timer;
  NpSpiContext *const npspi = animation->npspi_ctx;

  ev_tstamp const now = ev_now (loop);

  double const value = pow (sin (now), 2);
  double const brightness = BRIGHTNESS_MIN + (value * BRIGHTNESS_MAX);
  double const brightness_inv = BRIGHTNESS_MIN + ((1 - value) * BRIGHTNESS_MAX);

  for (int i = 0; i < N_PIXELS; i++) {

    uint8_t const level = (uint8_t) (0xff * brightness);
    uint8_t const level_inv = (uint8_t) (0xff * brightness_inv);

    switch (i % 3) {
      case 0:
        npspi_set_color (npspi, i, level);
        break;
      case 1:
        npspi_set_color (npspi, i, (level_inv << 16) | level_inv);
        break;
      case 2:
        npspi_set_color (npspi, i, (level << 8) | level_inv);
        break;
    }

  }

  npspi_show (npspi);

}

int
main (int argc, char **const argv)
{

  struct ev_loop *const loop = EV_DEFAULT;
  NpSpiContext *const ctx = npspi_new (N_PIXELS);
  struct animation_ctx animation = {
    .npspi_ctx = ctx,
  };
  ev_timer *const timer = &(animation.timer);

  npspi_open (ctx);

  ev_timer_init (timer, frame_cb, 0, PERIOD);

  ev_timer_start (loop, timer);
  ev_run (loop, 0);

  npspi_free (ctx);

  return 0;
}
