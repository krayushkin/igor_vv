# -*- coding: utf-8 -*-

import sys
from urllib import urlencode, quote_plus
import urllib2


def get_formula(out, f):
	"""docstring for get_formula"""
	url_base = "https://chart.googleapis.com/chart?cht=tx&chl="
	url = url_base + quote_plus(f)
	open(out, "wb").write( urllib2.urlopen( url ).read() )

get_formula(sys.argv[2], open(sys.argv[1], "rb").read())

