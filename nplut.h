#ifndef __NPLUT_H__
#define __NPLUT_H__

#define NPLUT_RATIO 24

#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif

uint8_t const *
nplut_get (uint8_t const i);

#ifdef __cplusplus
}
#endif

#endif /* __NPLUT_H__ */
