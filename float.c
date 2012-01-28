#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <malloc.h>
#include <windows.h>

typedef	float (*PFLOAT_FUNC)(float);

//tg(x)

//start FlFunc

static const float bernully[] = {
1.0/6, 1.0/30, 1.0/42, 1.0/30, 5.0/66, 691.0/2730, 7.0/6, 3617.0/510, 43867.0/798, 174611.0/330, 854513.0/138
};


// Таблица факториалов
static const int factorial_2n[] =
{
1,
2,
24,
720,
40320,
3628800,
479001600,
87178291200,
20922789888000,
6402373705728000,
2432902008176640000,
1124000727777607680000
};


//таблица коэффициентов для метода Горнера
//static const float coff[] = {
	
//};

// Релизация с использованием функций из библиотеки math
float FlMath(float x)
{
	return tanf(x);
}

// Прямая реализация с использованием цикла
float FlCyclNoGorner(float x)
{
}

// Безцикловая реализация по схеме Горнера
float FlNoCyclGorner(float x)
{
}



// Реализация по схеме Горнера с циклом
float FlCyclGorner(float x)
{
}


// Прямая реализация без цикла
float FlNoCyclNoGorner(float x)
{
	
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
0.100334672085450549161578237545,
0.202710035508672503645755114121,
0.309336249609623248346679247334,
0.422793218738161780123618882499,
0.546302489843790484158603248943,
0.684136808341692326251859412878,
0.842288380463079411342164348753,
1.02963855705036411158914688713,
1.26015821755033918805111170514};
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

	float fl= 1.0/4194304; // 1/2^22
	const char* flverify_status[] = {"OK", "ERROR"};
	srand(0);



	printf("Starting precision tests:\nPrecision: %.18f\n", fl);
	printf("Func: FlMath           flverify test:  %s\n",  flverify_status[flverify(fl, FlMath)] );
	printf("Func: FlCyclNoGorner   flverify test:  %s\n",  flverify_status[flverify(fl, FlCyclNoGorner)]);
	printf("Func: FlNoCyclNoGorner flverify test:  %s\n",  flverify_status[flverify(fl, FlNoCyclNoGorner)]);
	printf("Func: FlNoCyclGorner   flverify test:  %s\n",  flverify_status[flverify(fl, FlNoCyclGorner)]);
	printf("Func: FlCyclGorner     flverify test:  %s\n",  flverify_status[flverify(fl, FlCyclGorner)]);

	return 0;

}
