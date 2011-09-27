/*
 * fixp.c
 *
 *  Created on: 16.09.2011
 *      Author: dima
 */
#include <math.h>

#include "fixp.h"


//������� ������������� ��� ������ �������
static const fixp coff[] = {
				fixp_rconst(  1),
				fixp_rconst( -0.5),
				fixp_rconst(  0.041666667908430099),
				fixp_rconst(  -0.0013888889225199819),
				fixp_rconst(  0.000024801587642286904),
				fixp_rconst( -0.0000002755731998149713),
				fixp_rconst( 0.0000000020876755879584),
				fixp_rconst( -0.0000000000114707462279),
				fixp_rconst( 0.0000000000000477947759),
				fixp_rconst( -0.0000000000000001561921),
				fixp_rconst( 0.000000000000000000411),
				fixp_rconst( -0.0000000000000000000009)
};

// ����������� ���������� �� ����� �������
fixp FixNoCyclGorner(fixp x)
{
	fixp x2 = fixp_xmul(x, x);
	    return  fixp_add( fixp_xmul((
	    		fixp_add( fixp_xmul((
	    		fixp_add( fixp_xmul((
	    		fixp_add( fixp_xmul((
	    		fixp_add( fixp_xmul((
	    		fixp_add( fixp_xmul((
	    		fixp_add( fixp_xmul((
	    		fixp_add( fixp_xmul((
	    		fixp_add( fixp_xmul((
	    		fixp_add( fixp_xmul((
	    		fixp_add(fixp_xmul(
	    				coff[11],x2),
	    				coff[10])),x2),
	    				coff[9])),x2),
	    				coff[8])),x2),
	    				coff[7])),x2),
	    				coff[6])),x2),
	    				coff[5])),x2),
	    				coff[4])),x2),
	    				coff[3])),x2),
	    				coff[2])),x2),
	    				coff[1])),x2),
	    				coff[0]);
}


// ���������� �� ����� ������� � ������
fixp FixCyclGorner(fixp x)
{
	fixp retx = 0;
	fixp x2 = fixp_xmul(x, x);//������� �

	    int i;
	    for(i = 11; i>=0; i--)
	    {
	        retx = fixp_add( fixp_mul(retx, x2), coff[i] )  ;
	    }
	    return retx;
}

// ������� ��� �������� �������� ���������� ������� p. ���������� 0 � ������ ������ �������� � 1 � ������ �������.
int fixverify(float  fl, PFIX_FUNC p)
{
	// ����� ������� ��������
	static fixp s[] = {
			fixp_rconst( 0.1 ),
			fixp_rconst( 0.2 ),
			fixp_rconst( 0.3 ),
			fixp_rconst( 0.4 ),
			fixp_rconst( 0.5 ),
			fixp_rconst( 0.6 ),
			fixp_rconst( 0.7 ),
			fixp_rconst( 0.8 ),
			fixp_rconst( 0.9 )};
	// ��������� �������� ������� ��� ������ ������� ��������
	static fixp f[] = {
			fixp_rconst( 0.99500416527802582 ),
			fixp_rconst( 0.98006657784124163 ),
			fixp_rconst( 0.95533648912560598 ),
			fixp_rconst( 0.9210609940028851 ),
			fixp_rconst( 0.87758256189037276 ),
			fixp_rconst( 0.82533561490967833 ),
			fixp_rconst( 0.7648421872844885 ),
			fixp_rconst( 0.69670670934716539 ),
			fixp_rconst( 0.62160996827066439) } ;
	int i;
	// ���� ���� �� ���� �������� �� ������������� ���������� ��������, �� ������� �� �������� 1.
	// ����� 0
	for (i = 0; i < sizeof(s)/sizeof(s[0]); ++i)
	{
		if ( fabsf( fixp_tofloat( fixp_sub(f[i], p(s[i])) )   ) > fl )
			return 1;
	}
	return 0;
}
