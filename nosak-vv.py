# -*- encoding: utf-8 -*-
import decimal
import math

print "помощь для FnNoCyclNoGorner"
for x in xrange(10):
	print "powf(x, {0}) / {2} {1}".format(2*x+1, "+" if x % 2 != 0 else "-", math.factorial(2*x+1) )



print "эталонные значения"
for x in ("0.1","0.2","0.3","0.4","0.5","0.6","0.7","0.8","0.9"):
	f = float(x)
	print "{0:.30},".format(math.cos(f))

print "помощь для FnNoCyclGorner"
print "("*49*2
for x in xrange(49):
	d = 48 - x
	print "(1.0/{0})) * x) * x + ".format(2*d+1)



print  1000 / 7447.341076    
print  1000 / 8212.029491    
print  1000 / 228.931987 
print  1000 / 435.947005 
print  1000 / 41.731977 
