/* 
 * File:   fixp.h
 * Author: Nikorai
 *
 * Created on 22 Р�СЋРЅСЊ 2011 Рі., 13:30
 */

#ifndef FIXP_H
#define	FIXP_H

#include <stdio.h>

//start fixp macros

typedef long fixp;
typedef long long overfixp;
typedef unsigned long long uoverfix;
typedef	fixp (*PFIX_FUNC)(fixp);

#define FIXP_BITS	32
#ifndef FIXP_WBITS
#define FIXP_WBITS	2
#endif
#define FIXP_FBITS	(FIXP_BITS - FIXP_WBITS)
#define FIXP_FMASK	((1 << FIXP_FBITS) - 1)

#define fixp_rconst(R) (fixp)(R * (1LL << FIXP_FBITS) + (R >= 0 ? 0.5 : -0.5))
#define fixp_add(A,B) (A + B)
#define fixp_sub(A,B) (A - B)
#define fixp_xmul(A,B) (fixp)(((overfixp)A * (overfixp)B) >> FIXP_FBITS)
#define fixp_xdiv(A,B) (fixp)(((overfixp)A << FIXP_FBITS) / (overfixp)B)
#define fixp_fracpart(A) (A & FIXP_FMASK)
#define fixp_tofloat(A)  ((float) ((A) / (float) (1L << FIXP_FBITS)))

//end fixp macros


void test_fixp(void);
fixp FixCyclGorner(fixp x);
fixp FixNoCyclGorner(fixp x);
int fixverify(float  fl, PFIX_FUNC p);



#endif	/* FIXP_H */

