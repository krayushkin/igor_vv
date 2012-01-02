#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <malloc.h>
#include <windows.h>
#include "fixp.h"
#include "table.h"

typedef	float (*PFLOAT_FUNC)(float);

//start FlFunc

//таблица коэффициентов для метода Горнера
static const float coff[] = {
 1, -0.5, 0.041666667908430099,
 -0.0013888889225199819,
 0.000024801587642286904,
 -0.0000002755731998149713,
 0.0000000020876755879584,
 -0.0000000000114707462279,
 0.0000000000000477947759,
 -0.0000000000000001561921,
 0.000000000000000000411,
 -0.0000000000000000000009
};
//таблица готовых факториалов для четных чисел от 0 до 22
static const float fact_2n[] = {
    1,2,24,720,40320,3628800.0,
    479001600,87178289152.0,
    20922788478976.0,6402373530419200.0,
    2432902023163674600.0,
    1124000724806013000000.0
};

// Релизация с использованием функций из библиотеки math
float FlMath(float x)
{
	return cosf(x);
}

// Прямая реализация с использованием цикла
float FlCyclNoGorner(float x)
{
	float fact2x[12];//чётные степени числа х
	    int i;
	    int sgn = -1;//контроль знака
	    float x2 = x*x;
	    float result = 0;//результат
	    fact2x[0] = 1;//х в нулевой степени
	    for(i = 1; i<6; i++)//предварительный расчёт таблицы степеней
	    {
	        fact2x[i] = fact2x[i-1]*x2;
	    }

	    for(i = 0; i<6; i++)
	    {
	        sgn=sgn*(-1);
	        if(sgn < 0){
	            result -=(fact2x[i]/fact_2n[i]);
	        }else
	        {
	            result +=(fact2x[i]/fact_2n[i]);
	        }
	    }
	    return result;
}

// Безцикловая реализация по схеме Горнера
float FlNoCyclGorner(float x)
{
	float x2 = x*x;
	    return ((((((((((coff[11]*x2+
	            coff[10])*x2+coff[9])*x2+
	            coff[8])*x2+coff[7])*x2+
	            coff[6])*x2+coff[5])*x2+
	            coff[4])*x2+coff[3])*x2+
	            coff[2])*x2+coff[1])*x2+coff[0];
}



// Реализация по схеме Горнера с циклом
float FlCyclGorner(float x)
{
	float retx = 0;//возвращаемое значение
	    float x2 = x*x;//квадрат х
	    int i;
	    for(i = 11; i>=0; i--)
	    {
	        retx = retx*x2 + coff[i];
	    }
	    return retx;
}


// Прямая реализация без цикла
float FlNoCyclNoGorner(float x)
{
	  float fact2x[12];
	    int i;
	    float x2 = x*x;
	    fact2x[0] = 1;
	    for(i = 1; i<12; i++)//предварительный расчёт таблицы степеней
	    {
	        fact2x[i] = fact2x[i-1]*x2;
	    }
	    return ((fact2x[0]/fact_2n[0])-
	    (fact2x[1]/fact_2n[1])+
	    (fact2x[2]/fact_2n[2])-
	    (fact2x[3]/fact_2n[3])+
	    (fact2x[4]/fact_2n[4])-
	    (fact2x[5]/fact_2n[5]));
}

//end FlFunc

//start flverify
// функция для проверки точности вычисления функции p. Возвращает 0 в случае успеха проверки и 1 в случае неудачи.
int flverify(float  fl, PFLOAT_FUNC p)
{
	// Набор входных значений
	static float s[] = {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9};
	// Эталонные значения функции для набора входных значений
	static float f[] = {
			0.99500416527802582,
			0.98006657784124163,
			0.95533648912560598,
			0.9210609940028851,
			0.87758256189037276,
			0.82533561490967833,
			0.7648421872844885,
			0.69670670934716539,
			0.62160996827066439};
	int i;
	// если хотя бы одно значение не удовлетворяет требованию точности, то выходим со статусом 1.
	// иначе 0
	for (i = 0; i < sizeof(s)/sizeof(s[0]); ++i)
	{
		if ( fabsf(f[i] - p(s[i]) ) > fl )
			return 1;
	}
	return 0;
}
//end flverify
// генерируем случайные числа в диапазоне от 0 до 1
float my_random()
{
  return (float) rand() /  RAND_MAX;
}

// Измерение времени выполнения функций
void banchmark(int sample_n)
{
	int i;
	float *sample = (float*) malloc(sizeof(*sample) * sample_n);
	float *result = (float*) malloc(sizeof(*result) * sample_n);
	fixp *sample_fixp = (fixp*) malloc(sizeof(*sample_fixp) * sample_n);
	fixp *fix_res = (fixp*) malloc(sizeof(*fix_res) * sample_n);



	// заполняем массив случайными данными
	for (i = 0; i < sample_n; ++i) {
		sample[i] = my_random();
		sample_fixp[i] = fixp_rconst(sample[i]);
	}

	LARGE_INTEGER frequency;        // ticks per second
	LARGE_INTEGER t1, t2;           // ticks


	// Получаем количество тиков в секунлу
	QueryPerformanceFrequency(&frequency);
	// start timer

	printf("Starting %d samples banchmark\n", sample_n);

	QueryPerformanceCounter(&t1);
	for (i = 0; i < sample_n; ++i)
		result[i] = FlNoCyclNoGorner(sample[i]);
	QueryPerformanceCounter(&t2);
	printf("Func: FlNoCyclNoGorner %f ns\n", (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart);

	QueryPerformanceCounter(&t1);
	for (i = 0; i < sample_n; ++i)
		result[i] = FlCyclNoGorner(sample[i]);
	QueryPerformanceCounter(&t2);
	printf("Func: FlCyclNoGorner   %f ns\n", (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart);

	QueryPerformanceCounter(&t1);
	for (i = 0; i < sample_n; ++i)
		result[i] = FlNoCyclGorner(sample[i]);
	QueryPerformanceCounter(&t2);
	printf("Func: FlNoCyclGorner   %f ns\n", (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart);

	QueryPerformanceCounter(&t1);
	for (i = 0; i < sample_n; ++i)
		result[i] = FlCyclGorner(sample[i]);
	QueryPerformanceCounter(&t2);
	printf("Func: FlCyclGorner     %f ns\n", (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart);

	QueryPerformanceCounter(&t1);
	for (i = 0; i < sample_n; ++i)
		result[i] = FlMath(sample[i]);
	QueryPerformanceCounter(&t2);
	printf("Func: FlMath           %f ns\n", (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart);


	QueryPerformanceCounter(&t1);
	for (i = 0; i < sample_n; ++i)
		fix_res[i] = FixNoCyclGorner(sample_fixp[i]);
	QueryPerformanceCounter(&t2);
	printf("Func: FixNoCyclGorner  %f ns\n", (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart);

	QueryPerformanceCounter(&t1);
	for (i = 0; i < sample_n; ++i)
		fix_res[i] = FixCyclGorner(sample_fixp[i]);
	QueryPerformanceCounter(&t2);
	printf("Func: FixCyclGorner    %f ns\n", (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart);

	QueryPerformanceCounter(&t1);
	for (i = 0; i < sample_n; ++i)
		fix_res[i] = FixTableGorner(sample_fixp[i]);
	QueryPerformanceCounter(&t2);
	printf("Func: FixTableGorner   %f ns\n", (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart);

	free(sample);
	free(result);
	free(sample_fixp);
	free(fix_res);
}


unsigned char test_table(float fl)
{
	char k;
	for (k = 1; k < 20; ++k) {
		gen_table(k);
		if (!fixverify(fl, FixTableGorner))
		{
			free_table();
			return k;
		}
		else
		{
			free_table();
		}
	}
	return 0;
}

int main(int argc, char const *argv[])
{
	int sample_n = 1000000;

	float fl= 1.0/4194304; // 1/2^22
	const char* flverify_status[] = {"OK", "ERROR"};
	srand(0);



	printf("Starting precision tests:\nPrecision: %.18f\n", fl);
	printf("Func: FlMath           flverify test:  %s\n",  flverify_status[flverify(fl, FlMath)] );
	printf("Func: FlCyclNoGorner   flverify test:  %s\n",  flverify_status[flverify(fl, FlCyclNoGorner)]);
	printf("Func: FlNoCyclNoGorner flverify test:  %s\n",  flverify_status[flverify(fl, FlNoCyclNoGorner)]);
	printf("Func: FlNoCyclGorner   flverify test:  %s\n",  flverify_status[flverify(fl, FlNoCyclGorner)]);
	printf("Func: FlCyclGorner     flverify test:  %s\n",  flverify_status[flverify(fl, FlCyclGorner)]);

        test_fixp();

	printf("Func: FixCyclGorner    fixverify test: %s\n",  flverify_status[fixverify(fl, FixCyclGorner)]);
	printf("Func: FixNoCyclGorner  fixverify test: %s\n",  flverify_status[fixverify(fl, FixNoCyclGorner)]);


	char k = test_table(fl);
	gen_table(k);
	printf("Func: FixTableGorner   fixverify test: %s, k = %d\n",  flverify_status[fixverify(fl, FixTableGorner)], (int)k);
	banchmark(sample_n);
	free_table();

	return 0;

}
