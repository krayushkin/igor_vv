#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <malloc.h>
#include <windows.h>

typedef	float (*PFLOAT_FUNC)(float);

static const float a[] = 
{
	1.0,
	0.333333333333333314829616256247,
	0.133333333333333331482961625625,
	0.0539682539682539708092434693754,
	0.0218694885361552029956477838368,
	0.00886323552990219733216381570173,
	0.00359212803657248114230782221057,
	0.00145583438705131833039441158206,
	0.000590027440945585946591167569864,
	0.000239129114243552477921117560555,
	9.69153795692945094946216255671e-05 
};


#define LOOP_COUNT 11



// Релизация с использованием функций из библиотеки math
float FlMath(float x)
{
	return tanf(x);
}

// Прямая реализация с использованием цикла
float FlCyclNoGorner(float x)
{
	float power_x = x;
	float s = 0;
	int i;
	float x2 = x*x;

	for (i=0;i<LOOP_COUNT;++i)
	{
		s += a[i] * power_x;
		power_x = power_x * x2;
	}
	return s;
}

// Безцикловая реализация по схеме Горнера
float FlNoCyclGorner(float x)
{
	float x2 = x*x;
	return (((((((((((a[10])*x2+a[9])*x2+a[8])*x2+a[7])*x2+a[6])*x2+a[5])*x2+a[4])*x2+a[3])*x2+a[2])*x2+a[1])*x2+a[0])*x;

}



// Реализация по схеме Горнера с циклом
float FlCyclGorner(float x)
{
	float s = 0;
	float x2 = x * x;
	int i;
	for (i = LOOP_COUNT-1; i >= 0; i--) {
		s = s * x2 + a[i];
	}
	return s * x;
}


// Прямая реализация без цикла
float FlNoCyclNoGorner(float x)
{
	return a[0]*powf(x, 1)+a[1]*powf(x, 3)+a[2]*powf(x, 5)+a[3]*powf(x, 7)+a[4]*powf(x, 9)+a[5]*powf(x, 11)+a[6]*powf(x, 13)+a[7]*powf(x, 15)+a[8]*powf(x, 17)+a[9]*powf(x, 19)+a[10]*powf(x, 21);
}


//start flverify
// функция для проверки точности вычисления функции p. Возвращает 0 в случае успеха проверки и 1 в случае неудачи.
int flverify(float  fl, PFLOAT_FUNC p)
{
	// Набор входных значений
	static float s[] = {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8};
	// Эталонные значения функции для набора входных значений
	static float f[] = {
		0.100334672085450549161578237545,
		0.202710035508672503645755114121,
		0.309336249609623248346679247334,
		0.422793218738161780123618882499,
		0.546302489843790484158603248943,
		0.684136808341692326251859412878,
		0.842288380463079411342164348753,
		1.02963855705036411158914688713
	};

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

	// заполняем массив случайными данными
	for (i = 0; i < sample_n; ++i) {
		sample[i] = my_random();
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

	free(sample);
	free(result);
}



int main(int argc, char const *argv[])
{
	int sample_n = 1000000;

	float fl= 1.0/1048576; // 1/2^20
	const char* flverify_status[] = {"OK", "ERROR"};
	srand(0);


	printf("Starting precision tests:\nPrecision: %.18f\n", fl);
	printf("Func: FlMath           flverify test:  %s\n",  flverify_status[flverify(fl, FlMath)] );
	printf("Func: FlCyclNoGorner   flverify test:  %s\n",  flverify_status[flverify(fl, FlCyclNoGorner)]);
	printf("Func: FlNoCyclNoGorner flverify test:  %s\n",  flverify_status[flverify(fl, FlNoCyclNoGorner)]);
	printf("Func: FlNoCyclGorner   flverify test:  %s\n",  flverify_status[flverify(fl, FlNoCyclGorner)]);
	printf("Func: FlCyclGorner     flverify test:  %s\n",  flverify_status[flverify(fl, FlCyclGorner)]);

	banchmark(sample_n);
	return 0;

}
