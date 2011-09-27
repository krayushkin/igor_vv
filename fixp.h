/* 
 * File:   fixp.h
 * Author: Nikorai
 *
 * Created on 22 Р�СЋРЅСЊ 2011 Рі., 13:30
 */

#ifndef FIXP_H
#define	FIXP_H

#include <stdio.h>



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
#define fixp_fromint(I) ((overfixp)I << FIXP_FBITS)
#define fixp_toint(F) (F >> FIXP_FBITS)
#define fixp_add(A,B) (A + B)
#define fixp_sub(A,B) (A - B)
#define fixp_xmul(A,B) (fixp)(((overfixp)A * (overfixp)B) >> FIXP_FBITS)
#define fixp_xdiv(A,B) (fixp)(((overfixp)A << FIXP_FBITS) / (overfixp)B)
#define fixp_fracpart(A) (A & FIXP_FMASK)
#define fixp_tofloat(A)  ((float) ((A) / (float) (1L << FIXP_FBITS)))



#define FIXP_ONE	(fixp)(1 << FIXP_FBITS)
#define FIXP_ONE_HALF (FIXP_ONE >> 1)
#define FIXP_TWO	(FIXP_ONE + FIXP_ONE)
#define FIXP_PI	fixp_rconst(3.14159265)
#define FIXP_TWO_PI	fixp_rconst(2*3.14159265)
#define FIXP_HALF_PI fixp_rconst(3.14159265/2)
#define FIXP_E	fixp_rconst(2.71828183)

#define fixedpt_abs(A) (A < 0 ? -A : A)


fixp FixCyclGorner(fixp x);
fixp FixNoCyclGorner(fixp x);
int fixverify(float  fl, PFIX_FUNC p);

static inline long
fixp_mul(fixp A, fixp B)
{
    return(((overfixp)A * (overfixp)B) >> FIXP_FBITS);
}

static inline long
fixp_div(fixp A, fixp B)
{
    return (((overfixp)A << FIXP_FBITS)/(overfixp)B);
}

static inline void
fixp_str(fixp A, char *str)
{
    sprintf(str, "%f.10", fixp_tofloat(A));
}

static inline char*
fixp_cstr(fixp A)
{
    static char str[50];
    fixp_str(A, str);
    return (str);
}
static inline fixp
fixp_sqrt(fixp A)
{
    int invert = 0;
    int iter = FIXP_FBITS;
    int l, i;
    
    if(A < 0)
        return (-1);
    if(A == 0 || A == FIXP_ONE)
        return (A);
    if(A < FIXP_ONE && A > 6){
        invert = 1;
        A = fixp_div(FIXP_ONE, A);
    }
    if(A > FIXP_ONE){
        int s = A;
        
        iter = 0;
        while(s > 0){
            s >>= 2;
            iter++;
        }
    }
    l = (A >> 1) + 1;
    for(i = 0; i < iter; i++)
        l = (l+ fixp_div(A, l)) >> 1;
    if(invert)
        return (fixp_div(FIXP_ONE, l));
    return (l);
}

#endif	/* FIXP_H */

