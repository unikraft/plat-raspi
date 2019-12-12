/*  Call this macro repeatedly.  After each use, the pixel data can be extracted  */

#include <raspi/pixmaps/unikraft_logo.h>
#include <raspi/pixmaps/letter_0.h>
#include <raspi/pixmaps/letter_1.h>
#include <raspi/pixmaps/letter_2.h>
#include <raspi/pixmaps/letter_3.h>
#include <raspi/pixmaps/letter_4.h>
#include <raspi/pixmaps/letter_5.h>
#include <raspi/pixmaps/letter_6.h>
#include <raspi/pixmaps/letter_7.h>
#include <raspi/pixmaps/letter_8.h>
#include <raspi/pixmaps/letter_9.h>
#include <raspi/pixmaps/letter_a.h>
#include <raspi/pixmaps/letter_b.h>
#include <raspi/pixmaps/letter_c.h>
#include <raspi/pixmaps/letter_d.h>
#include <raspi/pixmaps/letter_e.h>
#include <raspi/pixmaps/letter_f.h>
#include <raspi/pixmaps/letter_g.h>
#include <raspi/pixmaps/letter_h.h>
#include <raspi/pixmaps/letter_i.h>
#include <raspi/pixmaps/letter_j.h>
#include <raspi/pixmaps/letter_k.h>
#include <raspi/pixmaps/letter_l.h>
#include <raspi/pixmaps/letter_m.h>
#include <raspi/pixmaps/letter_n.h>
#include <raspi/pixmaps/letter_o.h>
#include <raspi/pixmaps/letter_p.h>
#include <raspi/pixmaps/letter_q.h>
#include <raspi/pixmaps/letter_r.h>
#include <raspi/pixmaps/letter_s.h>
#include <raspi/pixmaps/letter_t.h>
#include <raspi/pixmaps/letter_u.h>
#include <raspi/pixmaps/letter_v.h>
#include <raspi/pixmaps/letter_w.h>
#include <raspi/pixmaps/letter_x.h>
#include <raspi/pixmaps/letter_y.h>
#include <raspi/pixmaps/letter_z.h>
#include <raspi/pixmaps/letter_Au.h>
#include <raspi/pixmaps/letter_Bu.h>
#include <raspi/pixmaps/letter_Cu.h>
#include <raspi/pixmaps/letter_Du.h>
#include <raspi/pixmaps/letter_Eu.h>
#include <raspi/pixmaps/letter_Fu.h>
#include <raspi/pixmaps/letter_Gu.h>
#include <raspi/pixmaps/letter_Hu.h>
#include <raspi/pixmaps/letter_Iu.h>
#include <raspi/pixmaps/letter_Ju.h>
#include <raspi/pixmaps/letter_Ku.h>
#include <raspi/pixmaps/letter_Lu.h>
#include <raspi/pixmaps/letter_Mu.h>
#include <raspi/pixmaps/letter_Nu.h>
#include <raspi/pixmaps/letter_Ou.h>
#include <raspi/pixmaps/letter_Pu.h>
#include <raspi/pixmaps/letter_Qu.h>
#include <raspi/pixmaps/letter_Ru.h>
#include <raspi/pixmaps/letter_Su.h>
#include <raspi/pixmaps/letter_Tu.h>
#include <raspi/pixmaps/letter_Uu.h>
#include <raspi/pixmaps/letter_Vu.h>
#include <raspi/pixmaps/letter_Wu.h>
#include <raspi/pixmaps/letter_Xu.h>
#include <raspi/pixmaps/letter_Yu.h>
#include <raspi/pixmaps/letter_Zu.h>
#include <raspi/pixmaps/letter_space.h>
#include <raspi/pixmaps/letter_ampersand.h>
#include <raspi/pixmaps/letter_apostrophe.h>
#include <raspi/pixmaps/letter_asterisk.h>
#include <raspi/pixmaps/letter_at.h>
#include <raspi/pixmaps/letter_backslash.h>
#include <raspi/pixmaps/letter_bar.h>
#include <raspi/pixmaps/letter_brackets_left.h>
#include <raspi/pixmaps/letter_brackets_right.h>
#include <raspi/pixmaps/letter_colon.h>
#include <raspi/pixmaps/letter_comma.h>
#include <raspi/pixmaps/letter_curly_braces_left.h>
#include <raspi/pixmaps/letter_curly_braces_right.h>
#include <raspi/pixmaps/letter_dollar.h>
#include <raspi/pixmaps/letter_dot.h>
#include <raspi/pixmaps/letter_equal.h>
#include <raspi/pixmaps/letter_exclamationmark.h>
#include <raspi/pixmaps/letter_greaterthan.h>
#include <raspi/pixmaps/letter_hashtag.h>
#include <raspi/pixmaps/letter_hat.h>
#include <raspi/pixmaps/letter_lesserthan.h>
#include <raspi/pixmaps/letter_lowerdash.h>
#include <raspi/pixmaps/letter_minus.h>
#include <raspi/pixmaps/letter_parentheses_left.h>
#include <raspi/pixmaps/letter_parentheses_right.h>
#include <raspi/pixmaps/letter_percentage.h>
#include <raspi/pixmaps/letter_plus.h>
#include <raspi/pixmaps/letter_questionmark.h>
#include <raspi/pixmaps/letter_quote.h>
#include <raspi/pixmaps/letter_semicolon.h>
#include <raspi/pixmaps/letter_similar.h>
#include <raspi/pixmaps/letter_slash.h>
#include <raspi/pixmaps/letter_tilde.h>


#define HEADER_PIXEL(data,pixel) {\
pixel[2] = (((data[0] - 33) << 2) | ((data[1] - 33) >> 4)); \
pixel[1] = ((((data[1] - 33) & 0xF) << 4) | ((data[2] - 33) >> 2)); \
pixel[0] = ((((data[2] - 33) & 0x3) << 6) | ((data[3] - 33))); \
data += 4; \
}

static unsigned int letter_width = 17;
static unsigned int letter_height = 26;

#define LETTER_MIN	0x20
#define LETTER_MAX	0x7F

static char *letters_data [96];
