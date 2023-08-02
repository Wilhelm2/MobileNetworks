import matplotlib.pyplot as plt
import csv
import sys
import ast
import subprocess
import matplotlib.ticker as mtick


intervals = ast.literal_eval(sys.argv[1]) #[20,10,7,5,4,3,2.5]
hosts = ast.literal_eval(sys.argv[2])
dimension = int(sys.argv[3])

def sumlist( l, nbelts ):
	res = []
	pos = 0
	case = 0 
	for x in l :
		case+= x
		pos += 1
		if pos == nbelts :
			res.append(case/nbelts)
			case = 0
			pos = 0
	if pos > 0: # derniers elts 
		res.append(case/pos)
	return res
	


for j in intervals:
	for nbHosts in hosts:
		plt.figure()	
		xarbre = []
		yarbre = []
		
		with open("data/"+str(dimension) +"/interval"+ str(j) +"Hosts" +str(nbHosts) + "/SentAmoutDataFile.dat",'r') as csvfile:
			plots = csv.reader(csvfile, delimiter=' ')
			for row in plots:
				xarbre.append(float(row[0]))
				yarbre.append(float(row[1]))

		plt.xlabel('Time(s)', fontsize=18)
		plt.ylabel('Sent data (Kbytes)', fontsize=18)
		
		xarbre = xarbre[0:len(xarbre):3]
		yarbre = [x/1000 for x in yarbre]
		plt.plot(xarbre,sumlist(yarbre,3), label='CB')
		

		# ~ axes = plt.gca()
		# ~ axes.get_yaxis().set_major_formatter(mtick.FormatStrFormatter('%.2e'))

		plt.tick_params(labelsize=18)
		plt.legend(fontsize=16)#loc='center left', bbox_to_anchor=(1, 0.5),fontsize=16)
		plt.tight_layout()
		subprocess.check_call(["bash","-c","mkdir -p Graphes/"+str(dimension) +"/interval"+ str(j) +"Hosts" + str(nbHosts)  ])
		plt.savefig("Graphes/"+str(dimension) +"/interval"+ str(j) +"Hosts" + str(nbHosts) + "/SentAmoutDataFile.eps", format='eps', bbox_inches="tight")

		# ~ plt.show()
		plt.close("all")

