
import sys
import math

#if(len(sys.argv)!=3):
#    print("usage: hypercube.py <dimension> <nbNodes>")
#    exit()

#if(not sys.argv[1].isdigit() or not sys.argv[2].isdigit()):
#    print("dimension must be an int")
#    exit()

#dimension = int(sys.argv[1])
#nbNodes = int(sys.argv[2])
#res = ""
#node = "Node"

#if(dimension == 0):
 #   exit() 


def calculArretesHypercube(dimension):
    tab = []
    if (dimension == 1):
        tab.append((0,1))
    elif (dimension == 2):
        for i in range(0,4):
            tab.append((i,(i+1)%4))
    else:

        for i in range(0,4):
            tab.append((i,(i+1)%4))
        for d in range(3, dimension+1):
            nbNodes = pow(2, d-1)
            tab2 =[]
            for i in range(len(tab)):
                tab2.append((tab[i][0]+nbNodes, tab[i][1]+nbNodes))
            for i in range(nbNodes):
                tab2.append((i, nbNodes+i))
            tab = tab + tab2
    return tab

def hypercubePave(indexCopie, arretesHypercube, dimension):
    arretesHypercubeCopie = []
    tailleHypercube = pow(2,dimension)
    for elt in arretesHypercube:
        arretesHypercubeCopie.append((elt[0]+tailleHypercube*indexCopie,elt[1]+tailleHypercube*indexCopie))
    return arretesHypercubeCopie

def hypercubeLinks(indexCopie, dimension):
    tailleHypercube = pow(2,dimension)
    arretesLink = []
    for i in range(0,tailleHypercube/2):
        arretesLink.append((tailleHypercube/2+i+tailleHypercube*indexCopie,i+tailleHypercube*(indexCopie+1)))
    return arretesLink

def pavage(nbNodes, dimension):
    H = calculArretesHypercube(dimension)
    tailleHypercube = pow(2,dimension)
    nbPaves = int(math.ceil(1.*nbNodes/tailleHypercube))
    print("A besoin de " + str(nbPaves) + " paves")
    arretesPavage = []
    for i in range(0,nbPaves): # fait nbPaves paves 
        arretesPavage = arretesPavage + hypercubePave(i,H,dimension)
    for i in range(0,nbPaves-1): # fait nbPaves paves 
        arretesPavage = arretesPavage + hypercubeLinks(i,dimension)
    print("Nombre de liens " + str(len(arretesPavage)))
    return arretesPavage


#arretesHypercube = calculArretesHypercube(dimension)
#print (arretesHypercube)
#print ("Pavage ")
#print(pavage(nbNodes,dimension))

#for elt in pavage(nbNodes,dimension):
#    print (str(elt[0]) +":"+str(elt[1]))
#    res += node +"["+ str(elt[0]) +"].ethg++ <--> chan <--> " + node + "["+str(elt[1])+"].ethg++;\n"

#print(res)
#print("Size of Hypercube " + str(pow(2,dimension))+ " Pavage number of links " + str(len(pavage(nbNodes,dimension))))
# le nombre d'arretes est de a(x) = 2*a(x-1) + 2^(x-1) a partir de x=2
