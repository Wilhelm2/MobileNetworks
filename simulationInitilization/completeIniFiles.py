import random
import math
import sys
import subprocess
from shutil import copyfile
from hypercube import pavage

if len(sys.argv) != 7:
	print sys.argv[0] + " seed SendInterval nbHosts nbStations fautes dimension" 
	exit(1)

random.seed(sys.argv[1])
#tab = [ (158.33,175), (375,175), (591.66,175), (158.33,425), (375,425), (591.66,425) ]
tab = [ (300,300), (508,300), (404,480), (92,300), (196,480), (196,120),(404,120) ]
nbHosts = int(sys.argv[3])
nbStations = int(sys.argv[4])
fautes = int(sys.argv[5])
dimension = int(sys.argv[6])


ligne = 0 
colonne = 0

def placementStations(tailleCarre):
	positionStations=[]
	for i in range(tailleCarre):
		for j in range(tailleCarre):
			if(i%2==0):
				positionStations.append( (300+j*208,300+ i*180) )
			else:
				positionStations.append( (404+j*208,300+ i*180) )
	for i in range(0,nbStations-tailleCarre*tailleCarre):
		if(tailleCarre%2==0):
			positionStations.append( (300+i*208,300+ tailleCarre*180) )
		else:
			positionStations.append( (404+i*208,300+ tailleCarre*180) )
	return positionStations

def placementHostsString(nbHosts,nbStations, positionStations):
	host=0
	iniFileString = ""
	for i in range(nbStations):
		for j in range(int(nbHosts/nbStations)):
			distance = 150
			while distance > 115: # pour laisser une petite marge (au lieu de mettre 120)
				x = random.uniform(positionStations[i][0]-120,positionStations[i][0]+120)
				y = random.uniform(positionStations[i][1]-120,positionStations[i][1]+120)
				distance = math.sqrt(pow(x-positionStations[i][0],2) + pow(y-positionStations[i][1],2))
			iniFileString+=	"*.Mobiles["+str(host)+"].mobility.initialX = "+str(x) +"m\n"
			iniFileString+= "*.Mobiles["+str(host)+"].mobility.initialY = "+str(y) +"m\n"
			host+=1

	for i in range(host,nbHosts):
		cell = random.randint(0,len(positionStations)-1)
		distance = 150
		while distance > 115: # pour laisser une petite marge (au lieu de mettre 120)
			x = random.uniform(positionStations[cell][0]-120,positionStations[cell][0]+120)
			y = random.uniform(positionStations[cell][1]-120,positionStations[cell][1]+120)
	#		x = max(min(random.uniform(tab[cell][0]-120,tab[cell][0]+120),510),90)
	#		y = max(min(random.uniform(tab[cell][1]-120,tab[cell][1]+120),540),60)
			distance = math.sqrt(pow(x-positionStations[cell][0],2) + pow(y-positionStations[cell][1],2))
			#~ print distance 
		iniFileString+=	"*.Mobiles["+str(i)+"].mobility.initialX = "+str(x) +"m\n"
		iniFileString+= "*.Mobiles["+str(i)+"].mobility.initialY = "+str(y) +"m\n"
	return iniFileString

def writeInitFile(nbHosts,nbStations,fautes):
	positionStations = placementStations(int(math.sqrt(nbStations)) )
	iniFileString =  "Arbre.simParams.AppMFrequency = " + sys.argv[2] + "\n"
	iniFileString+= "Arbre.simParams.nbMobileHosts = " +str(nbHosts) + "\n"
	iniFileString+= "Arbre.simParams.nbMobileSupportStations = " + str(nbStations) + "\n"
	for i in range(0,len(positionStations)):
		iniFileString+=	"*.Stations["+str(i)+"].mobility.initialX = "+str(positionStations[i][0]) +"m\n"
		iniFileString+= "*.Stations["+str(i)+"].mobility.initialY = "+str(positionStations[i][1]) +"m\n"
	iniFileString += placementHostsString(nbHosts,nbStations,positionStations)

	if fautes == 1:
		copyfile("modelarbreFautes.ini","arbre.ini")
	else:
		copyfile("modelarbre.ini","arbre.ini")
	f=open("arbre.ini","a") 
	f.write(iniFileString)
	f.close()

	cpCommand = "cp arbre.ini ../simulations/arbre.ini"
	subprocess.check_call(["bash","-c",cpCommand])

def arbreLargeurTrois(nbHosts, nbStations):
	currlevel=[0]
	nextlevel=[]
	pos=0
	s2=""
	while(pos < nbStations):
		for n in currlevel: 
			for i in range(1,4): # arbre de largeur 3
				if(pos+i<nbStations):
					s2+= "\t\t\tStations["+str(n)+"].ethg++ <--> chan <--> Stations["+str(pos+i)+"].ethg++;\n"
					nextlevel.append(pos+i)
				else:
					break
			pos+=3
		for i in range(0, len(currlevel) -1):
			s2+= "\t\t\tStations["+str(currlevel[i])+"].ethg++ <--> chan <--> Stations["+str(currlevel[i+1])+"].ethg++;\n"		
		currlevel=nextlevel
		nextlevel=[]
	for i in range(0, len(currlevel) -1):
		s2+= "\t\t\tStations["+str(currlevel[i])+"].ethg++ <--> chan <--> Stations["+str(currlevel[i+1])+"].ethg++;\n"		
		
	s2+="}"
	return s2

def writeNedFile(nbHosts,nbStations,dimension):
	print("writenedfile")
	nedFileString="\t\tStations[" + str(nbStations)+"]:S\n\t\t{\n\t\t\t@display(\"i=device/antennatower\");\n\t\t}\n"
	nedFileString+="\t\tMobiles[" + str(nbHosts)+"]:M"
	#nedFileStringPosMobiles = arbreLargeurTrois(nbHosts,nbStations)
	arretesPavage = pavage(nbStations,dimension)
	
	nedFileStringPosMobiles=""
	for elt in arretesPavage:
#        print (str(elt[0]) +":"+str(elt[1]))
		nedFileStringPosMobiles +=  "Stations["+ str(elt[0]) +"].ethg++ <--> chan <--> Stations["+str(elt[1])+"].ethg++;\n"
	nedFileStringPosMobiles+="}"

	print (nedFileStringPosMobiles)
	copyfile("modelarbre.ned","arbre.ned")
	f=open("arbre.ned","a") 
	f.write(nedFileString)
	f2=open("modelarbrefin.ned","r")
	f.write(f2.read())
	f.write(nedFileStringPosMobiles)
	f.close()

	cpCommand = "cp arbre.ned ../simulations/arbre.ned"
	subprocess.check_call(["bash","-c",cpCommand])

def writeConnectionToEstablishFile(nbStations,dimension):
	arretesPavage = pavage(nbStations,dimension)
	channels = [[] for x in range(0,nbStations)]
	for elt in arretesPavage:
		channels[elt[0]].append(elt[1])
	print(channels)

	f=open("connectionToEstablish.txt","w")
	for elt in channels:
		f.write(''.join(str(x)+ " " for x in elt))
		f.write("\n")
	f.close()
	return channels



writeInitFile(nbHosts,nbStations,fautes)
writeNedFile(nbHosts,nbStations,dimension)
writeConnectionToEstablishFile(nbStations,dimension)



