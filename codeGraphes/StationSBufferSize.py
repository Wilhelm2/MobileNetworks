import matplotlib.pyplot as plt
import csv
import sys
import ast
import subprocess
from statistics import mean 
from statistics import pstdev 


intervals = ast.literal_eval(sys.argv[1]) 
hosts = ast.literal_eval(sys.argv[2]) 
dimension = int(sys.argv[3])

for j in intervals:
	for nbHosts in hosts:
		plt.figure()	
		xtime = []
		yMin = []
		yMax = []
		yMoy = []
		yEcartType = []
		
		with open("data/"+str(dimension) +"/interval"+ str(j) +"Hosts" +str(nbHosts) + "/SBufferSizeMSS.dat",'r') as csvfile:
			plots = csv.reader(csvfile, delimiter=' ')
			for row in plots:
				row = [int(x) for x in row[:-1]]
				xtime.append(float(row[0]))
				yMin.append(float(min(row[1:])))
				yMax.append(float(max(row[1:])))
				yMoy.append(mean(row[1:]))
				yEcartType.append(pstdev(row[1:]))


		plt.plot(xtime,yMin, label='Min')
		plt.plot(xtime,yMax, label='Max')
		plt.plot(xtime,yMoy, label='Moy')
		plt.plot(xtime,yEcartType, label='EcartType')
		
		
		# ~ plt.yscale('log')
		plt.xlabel('Time(s)', fontsize=18)
		plt.ylabel('Average SBuffer size (messages)', fontsize=18)
		# ~ plt.title('Average size of bufferSend of MSS ('+ str(j)+')')
		
		plt.tick_params(labelsize=18)
		plt.legend( labels = ['Min','Max','Moy','Deviation'],loc='upper center', bbox_to_anchor=(0.45, -0.15),fancybox=False, shadow=False, ncol=4,fontsize=16)
		# ~ plt.legend(loc='center left', bbox_to_anchor=(1, 0.5),fontsize=16)
		plt.tight_layout()
		subprocess.check_call(["bash","-c","mkdir -p Graphes/"+str(dimension) +"/interval"+ str(j) +"Hosts" + str(nbHosts)  ])
		plt.savefig("Graphes/"+str(dimension) +"/interval"+ str(j) +"Hosts" +str(nbHosts) + "/SBufferSize.eps", format='eps', bbox_inches="tight")

		plt.show()
		plt.close("all")


