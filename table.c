/*
 * table.c
 *
 *  Created on: 26.09.2011
 *      Author: ATK
 */

#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include "fixp.h"
#include "table.h"

fixp *tbl;
unsigned char k;

// возращенный массив после работы должен быть очищен с помощью free()
fixp* gen_table(unsigned char k)
{
	printf("gen_table: Generation table with %d elem\n", (1<<k));
	fixp * tbl = (fixp*)malloc(sizeof(*tbl)*(1<<k)*N_COEF);
	if (tbl != NULL)
	{
		int i;
		for (i=0;i<(1<<k); ++i)
		{
			*(tbl+i*3+0) = fixp_rconst( cosf( ((float)i) / (1<<k) ) ) ;
			*(tbl+i*3+1) = fixp_rconst(-sinf( ((float)i) / (1<<k)  ) / (1)) ;
			*(tbl+i*3+2) = fixp_rconst(-cosf( ((float)i) / (1<<k)  ) / (1*2));
		}
	}
	return tbl;
}


fixp FixTableGorner(fixp x)
{
	printf("FixTable12Gorner: index = %ld\n", x);
	int i = fixp_fracpart(x) >> (FIXP_FBITS-k);
	printf("FixTable12Gorner: index = %d\n", i  );
	fixp x_ml = x & ((1 << (FIXP_FBITS-k)) - 1);
	printf("FixTable12Gorner: x_ml = %ld\n", fixp_fracpart(x_ml));
	return fixp_add( (*(tbl+i*3+0)), fixp_xmul(x_ml, (*(tbl+i*3+1)) ) );
}

void test_table(void)
{
	k = 11;
	tbl = gen_table(k);
	fixp x;
    // 0b00100000000000000000000000000000
	x= FixTableGorner(fixp_rconst(1.0/32768));
	printf("%.20f\n", fixp_tofloat(x));
	x = FixTableGorner(0b00100011111111111111111111111111);
    printf("%.20f\n", fixp_tofloat(x));
    printf("%.20f\n", cosf(fixp_tofloat(0b00100011111111111111111111111111)));

}

void free_table(void)
{
	free(tbl);
}
