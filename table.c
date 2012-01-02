#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include "fixp.h"
#include "table.h"

fixp *tbl__;
unsigned char k__;


// возращенный массив после работы должен быть очищен с помощью free()
void gen_table(unsigned char k)
{
	//printf("gen_table: Generation table with %d elem\n", (1<<k));
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
	tbl__ = tbl;
	k__ = k;
}


fixp FixTableGorner(fixp x)
{
	//printf("FixTable12Gorner: index = %ld\n", x);
	int i = fixp_fracpart(x) >> (FIXP_FBITS-k__);
	//printf("FixTable12Gorner: index = %d\n", i  );
	fixp x_ml = x & ((1 << (FIXP_FBITS-k__)) - 1);
	//printf("FixTable12Gorner: x_ml = %ld\n", fixp_fracpart(x_ml));
	return fixp_add( (*(tbl__+i*3+0)), fixp_xmul(x_ml, (*(tbl__+i*3+1)) ) );
}




void free_table(void)
{
	free(tbl__);
}
