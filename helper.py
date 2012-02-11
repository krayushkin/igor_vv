# -*- coding: utf-8 -*-
import decimal
import math


b = [1.0/6, 1.0/30, 1.0/42, 1.0/30, 5.0/66, 691.0/2730, 7.0/6, 3617.0/510, 43867.0/798, 174611.0/330, 854513.0/138]

# расчет коэффициентов для метода Горнера
def a(n):
	"Возвращает a-энное"
	return math.pow(2, 2*n) * ( math.pow(2, 2*n) - 1 ) * b[n-1] / math.factorial(2*n)

def gen(m):
	"""Прямая реализация без цикла"""
	s = ""
	for i in  xrange(m) :
		s+="a[{0}]*powf(x, {1})+".format(i, 2*i+1)
	return s 

def gen_gorner(m):
	"""Генерировать безцикловую схему горнера для m коэфф"""
	s =  "(" * m + "a[{0}]".format(m-1)
	for i in  reversed( xrange(m-1) ):
		s+=")*x2+a[{0}]".format(i)
	return s + ")*x"


def gen_a(n):
	"""Возвращаем список n коэф."""
	return [a(x) for x in xrange(1, n+1)]

def main():
	n = len(b)
	print gen(n)
	print gen_gorner(n)
	for x in gen_a(n):
		print "{0:.30}, ".format(x),
	print

if __name__ == '__main__':
	main()	

