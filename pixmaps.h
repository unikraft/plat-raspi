/*  Call this macro repeatedly.  After each use, the pixel data can be extracted  */

#include "pixmaps/unikraft_logo.h"
#include "pixmaps/letter_0.h"
#include "pixmaps/letter_1.h"
#include "pixmaps/letter_2.h"
#include "pixmaps/letter_3.h"
#include "pixmaps/letter_4.h"
#include "pixmaps/letter_5.h"
#include "pixmaps/letter_6.h"
#include "pixmaps/letter_7.h"
#include "pixmaps/letter_8.h"
#include "pixmaps/letter_9.h"
#include "pixmaps/letter_a.h"
#include "pixmaps/letter_b.h"
#include "pixmaps/letter_c.h"
#include "pixmaps/letter_d.h"
#include "pixmaps/letter_e.h"
#include "pixmaps/letter_f.h"
#include "pixmaps/letter_g.h"
#include "pixmaps/letter_h.h"
#include "pixmaps/letter_i.h"
#include "pixmaps/letter_j.h"
#include "pixmaps/letter_k.h"
#include "pixmaps/letter_l.h"
#include "pixmaps/letter_m.h"
#include "pixmaps/letter_n.h"
#include "pixmaps/letter_o.h"
#include "pixmaps/letter_p.h"
#include "pixmaps/letter_q.h"
#include "pixmaps/letter_r.h"
#include "pixmaps/letter_s.h"
#include "pixmaps/letter_t.h"
#include "pixmaps/letter_u.h"
#include "pixmaps/letter_v.h"
#include "pixmaps/letter_w.h"
#include "pixmaps/letter_x.h"
#include "pixmaps/letter_y.h"
#include "pixmaps/letter_z.h"
#include "pixmaps/letter_Au.h"
#include "pixmaps/letter_Bu.h"
#include "pixmaps/letter_Cu.h"
#include "pixmaps/letter_Du.h"
#include "pixmaps/letter_Eu.h"
#include "pixmaps/letter_Fu.h"
#include "pixmaps/letter_Gu.h"
#include "pixmaps/letter_Hu.h"
#include "pixmaps/letter_Iu.h"
#include "pixmaps/letter_Ju.h"
#include "pixmaps/letter_Ku.h"
#include "pixmaps/letter_Lu.h"
#include "pixmaps/letter_Mu.h"
#include "pixmaps/letter_Nu.h"
#include "pixmaps/letter_Ou.h"
#include "pixmaps/letter_Pu.h"
#include "pixmaps/letter_Qu.h"
#include "pixmaps/letter_Ru.h"
#include "pixmaps/letter_Su.h"
#include "pixmaps/letter_Tu.h"
#include "pixmaps/letter_Uu.h"
#include "pixmaps/letter_Vu.h"
#include "pixmaps/letter_Wu.h"
#include "pixmaps/letter_Xu.h"
#include "pixmaps/letter_Yu.h"
#include "pixmaps/letter_Zu.h"
#include "pixmaps/letter_space.h"

#define HEADER_PIXEL(data,pixel) {\
pixel[0] = (((data[0] - 33) << 2) | ((data[1] - 33) >> 4)); \
pixel[1] = ((((data[1] - 33) & 0xF) << 4) | ((data[2] - 33) >> 2)); \
pixel[2] = ((((data[2] - 33) & 0x3) << 6) | ((data[3] - 33))); \
data += 4; \
}

static unsigned int letter_width = 17;
static unsigned int letter_height = 26;

#define LETTER_MIN	0x20
#define LETTER_MAX	0x7F

static char *letters_data [96];
