#ifndef DELAY_HELP_H_
#define DELAY_HELP_H_

#define _TOVALUE(x)   _##x
#define TOVALUE(x)    _TOVALUE(x)

#define _CLOCK_XT1            (0)
#define _CLOCK_VLO            (1)
#define _CLOCK_REFO           (2)
#define _CLOCK_DCO            (3)
#define _CLOCK_DCO_DIV        (4)
#define _CLOCK_XT2            (5)

#define _CLOCK_DIV_1           1
#define _CLOCK_DIV_2           2
#define _CLOCK_DIV_4           4
#define _CLOCK_DIV_8           8
#define _CLOCK_DIV_16          16
#define _CLOCK_DIV_32          32

#endif