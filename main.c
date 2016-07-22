#include <unistd.h>
#include <math.h>
#include <ev.h>
#include "npspi.h"

#define FPS 30
#define PERIOD (1.0 / FPS)

struct animation_ctx {

  ev_timer timer;
  NpSpiContext *npspi_ctx;

};

static void
frame_cb (EV_P_ ev_timer *const timer, const int revents)
{

  struct animation_ctx *const animation = (struct animation_ctx *) timer;
  NpSpiContext *const npspi = animation->npspi_ctx;

  const ev_tstamp now = ev_now (loop);

  /* const double brightness = 0.05 + (pow (sin (now), 2) * 0.10); */
  const double value = pow (sin (now), 2);
  const double brightness = 0.05 + (value * 0.95);
  const double brightness_inv = 0.05 + ((1 - value) * 0.95);

  for (int i = 0; i < 8; i++) {

    const uint8_t level = (uint8_t) (0xff * brightness);
    const uint8_t level_inv = (uint8_t) (0xff * brightness_inv);

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
  NpSpiContext *const ctx = npspi_new (8);
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
