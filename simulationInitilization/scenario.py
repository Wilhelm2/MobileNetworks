from random import randint,seed
with open("../out/gcc-release/src/scenario.xml",'w+') as fp:
	j = 3
	seed(10)
	fp.write("<scenario>\n")
	for i in range(15,300,30):
		ident = randint(0,69)
		fp.write('\t<crash t="'+str(i)+ 's" module="Mobiles['+ str(ident) + ']"/>\n')
		fp.write('\t<startup t="'+str(i+j)+ 's" module="Mobiles['+ str(ident)+']"/>\n')
		j+=1
	fp.write("</scenario>")
