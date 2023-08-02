import matplotlib.pyplot as plt
import csv
import sys
import ast
import subprocess


intervals = ast.literal_eval(sys.argv[1]) 
hosts = ast.literal_eval(sys.argv[2]) 
dimension = int(sys.argv[3])

for j in intervals:
	for nbHosts in hosts:
		plt.figure()	
		xtime = []
		yTotalSentMessages = []
		yWiredSentMessages = []
		yWifiSentMessages  = []
			
		with open("data/"+str(dimension) +"/interval"+ str(j) +"Hosts" +str(nbHosts) + "/nbTheoricalSentMessagesFile.dat",'r') as csvfile:
			plots = csv.reader(csvfile, delimiter=' ')
			for row in plots:
				xtime.append(float(row[0]))
				yTotalSentMessages.append(float(row[1]))
				yWifiSentMessages.append(float(row[2]))
				yWiredSentMessages.append(float(row[3]))


		# ~ plt.plot(xtime,yTotalSentMessages, label='Total CB')
		l1 = plt.plot(xtime,yWiredSentMessages)#, label='Wired CB')
		l2 = plt.plot(xtime,yWifiSentMessages)#, label='Wireless CB')
		
#		plt.yscale('log')
		plt.xlabel('Time(s)', fontsize=18)
		plt.ylabel('Theoretical sent messages', fontsize=18)
		# ~ plt.title('Average size of bufferSend of MSS ('+ str(j)+')')
		plt.ylim(ymin=0)
#		plt.xlim(xmin=0,xmax=300)
		
		plt.tick_params(labelsize=16)
		# ~ lgd = plt.legend(bbox_to_anchor=(1, 0.5),loc='center left',fontsize=16)
		plt.legend( labels = ['Wired ','Wireless '],loc='upper center', bbox_to_anchor=(0.45, -0.15),fancybox=False, shadow=False, ncol=2,fontsize=16)
		
		plt.tight_layout()
		subprocess.check_call(["bash","-c","mkdir -p Graphes/"+str(dimension) +"/interval"+ str(j) +"Hosts" + str(nbHosts)  ])
		plt.savefig("Graphes/"+str(dimension) +"/interval"+ str(j) +"Hosts" +str(nbHosts) + "/nbTheoricalSentMessagesFile.eps", format='eps', bbox_inches="tight")

		plt.show()
		plt.close("all")
