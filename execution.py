
import subprocess
from shutil import copyfile

subprocess.check_call(["bash","-c","make MODE=debug"])				
subprocess.check_call(["bash","-c","make "])				
subprocess.check_call(["bash","-c","mkdir -p data "])				


repetitions = 1
intervals = [2]  
nbHosts = [320] 
nbStations = [16]
seed = 3
fautes = 1
dimension = 4

for j in intervals:
	print ("INTERVAL " + str(j))
	for k in nbHosts:
		# Completes initialization files (.ini and .ned)
		subprocess.check_call(["bash","-c","python completeIniFiles.py " + str(seed) + " " + str(j) + " " + str(k) + " " + str(nbStations[0]) + " " + str(fautes)+ " " + str(dimension)], cwd='simulationInitilization')
			
		# Execution 
		execCommandA = "./MNwithStationFailures	   -f ../../../simulations/arbre.ini    -u Cmdenv > res.txt" #res.txt "# /dev/null"
		subprocess.check_call(["bash","-c",execCommandA], cwd='out/gcc-release/src/')				

#		execCommandA = "valgrind  ./MNwithStationFailures	   -f ../../../simulations/arbre.ini    -u Cmdenv > res.txt" #res.txt "# /dev/null"
#		execCommandA = "./MNwithStationFailures_dbg	   -f ../../../simulations/arbre.ini    -u Cmdenv > res.txt" #res.txt "# /dev/null"
#		execCommandA = "gdb --args ./MNwithStationFailures_dbg  -f ../../../simulations/arbre.ini    -u Cmdenv " # /dev/null"
#		execCommandA = "valgrind  --tool=massif ./ArbreOptimise_dbg	   -f arbre.ini    -u Cmdenv " # /dev/null"
#		subprocess.check_call(["bash","-c",execCommandA], cwd='out/gcc-debug/src/')
				
		# Copy files
		subprocess.check_call(["bash","-c","mkdir -p codeGraphes/data/"+str(dimension)+"/interval"+str(j)+ "Hosts"+ str(k) ])
		subprocess.check_call(["bash","-c","cp -r data/* codeGraphes/data/"+str(dimension)+"/interval"+str(j)+ "Hosts"+ str(k) ])
		
subprocess.check_call(["bash","-c","python3 SentAmoutData.py " + str(intervals)+" " + str(nbHosts) + " "+str(dimension) ], cwd='codeGraphes/')
subprocess.check_call(["bash","-c","python3 StationSBufferSize.py " + str(intervals)+" " + str(nbHosts) + " "+str(dimension) ], cwd='codeGraphes/')
subprocess.check_call(["bash","-c","python3 nbSentMsg.py " + str(intervals)+" " + str(nbHosts) + " "+str(dimension) ], cwd='codeGraphes/')
subprocess.check_call(["bash","-c","python3 theoricalSentMsg.py " + str(intervals)+" " + str(nbHosts) + " "+str(dimension) ], cwd='codeGraphes/')
