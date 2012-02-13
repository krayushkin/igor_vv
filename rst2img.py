# -*- coding: utf-8 -*-
import docutils.core as doc
import xml.sax as sax
import sys
from urllib import urlencode, quote_plus
import urllib2

class BarChart(object):
	"""bar chart"""
	def __init__(self, **keywords):
		self.title = keywords["title"] if "title" in keywords else None
		self.labels = keywords["labels"] if "labels" in keywords else []
		self.digits = keywords["digits"] if "digits" in keywords else []
		self.suffix = keywords["suffix"] if "suffix" in keywords else None
		

class DocutilsXmlHandler(sax.ContentHandler):
	"""docstring for DocutilsXmlHandler"""
	def __init__(self):
		self.data = []
		self.tbody = False
		self.read = False
	
	def startElement(self, name, attrs):
		"""docstring for fname"""
		if name == "tbody":
			self.tbody = True
		elif self.tbody and name == "paragraph":
			self.read = True

	def characters(self, content):
		if self.read:
			self.data.append( content )
			self.read = False
			
	def endElement(self, name):
		"""docstring for endElement(self, """
		if name == "tbody":
			self.tbody = False

	def get_data(self):
		"""docstring for get_data"""
		data = BarChart(labels=self.data[0::2], digits=self.data[1::2])
		return data


def get_image(out, data):
	palette = ["85FF6D", "7791FF", "C5C8D9", "FF7499", "FBAC71",
		   "E6FB78", "E58D9D", "D6CE3D", "004D67", "ECE1CD"]
	labels = data.labels
	digits = data.digits
	title = data.title
	colors = [palette[i % len(palette)] for i in xrange(len(labels))]
	height = (27 * len(labels) +      # 27 пикселей на линию * кол-во линий
	          25 +                    # 25 на нижнюю часть (подписи к оси 0х)
		  (50 if title else 0))   # Если есть заголовок, то на него 50 пикс.
	url_base = "https://chart.googleapis.com/chart?"
	args = {"chs" : "500x{0}".format(height), # chart_size
		"cht" : "bhg",                    # chart_type
		"chd" : "t:" + "|".join(digits),  # chart_data
		"chds": "a", 
		"chdl": u"|".join(labels).encode("utf-8"),
		"chxt": "x",
		"chco": ",".join(colors),
		"chm" : "|".join( ["N,000000,{0},-1,11,,c".format(i) for i in xrange(len(labels))] ) }
	if title:
		args["chtt"] = title.encode("utf-8") if isinstance(title, unicode) else title

	url = url_base + urlencode(args)

	open(out, "wb").write( urllib2.urlopen( url ).read() )



xml_str = doc.publish_string(source=open(sys.argv[1]).read(),
		             writer_name="xml",
			     settings_overrides={'input_encoding': 'utf-8',
				                 "output_encoding": "utf-8"})
handler = DocutilsXmlHandler()

sax.parseString(xml_str, handler)

data =  handler.get_data()
get_image(sys.argv[2], data)

