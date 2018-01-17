#!/usr/bin/python

#Data decoding script
#Alec Fiala

import os
import sys
import struct
import matplotlib.pyplot as plt


#Function to plot data from a single datafile
def plotterFun(dataFile):


	offset = 8240


	# Data readout format
	data_format = """
	Timestamp: {0}
	ENV Hum: {1} %; ENV Temp: {2} C;
	HK Temp SLC: {3} C;  HK Temp Bat1: {4} C;  HK Temp Bat2:{5} C
	Spec Temp: {6} C; RPi Temp: {7} C; Spec: {8}
	ADS1: {9} A; ADS2: {10} A; ADS3: {11} A; ADS4: {12} A;
	"""

	#Initilize everything
	timestamp= []
	spectrum= []
	spec_temp=[]
	rpi_temp=[]
	hk_temp_bat1=[]
	hk_temp_bat2=[]
	hk_temp_slc=[]
	env_temp=[]
	env_hum=[]
	ads1=[]
	ads2=[]
	ads3=[]
	ads4= []




	#Open file to read
	with open(dataFile,'rb') as f:
		byte = offset
		count = 1
		size = os.path.getsize(dataFile)
		while byte < size:
			f.seek(byte, 0)
			timestamp.append(struct.unpack('I',f.read(4)))
			spectrum.append(struct.unpack('2048f',f.read(2048*4)))
			spec_temp.append(struct.unpack('f',f.read(4)))
			rpi_temp.append(struct.unpack('f',f.read(4)))
			hk_temp_bat1.append(struct.unpack('f',f.read(4)))
			hk_temp_bat2.append(struct.unpack('f',f.read(4)))
			hk_temp_slc.append(struct.unpack('f',f.read(4)))
			env_temp.append(struct.unpack('f',f.read(4)))
			env_hum.append(struct.unpack('f',f.read(4)))
			ads1.append(struct.unpack('f',f.read(4)))
			ads2.append(struct.unpack('f',f.read(4)))
			ads3.append(struct.unpack('f',f.read(4)))
			ads4.append(struct.unpack('f',f.read(4)))
			byte = byte + offset
			count += 1


			#print(data_format.format(timestamp,env_hum,env_temp,hk_temp_slc,hk_temp_bat1,hk_temp_bat2,spec_temp,rpi_temp,spectrum[0],ads1,ads2,ads3,ads4))


	# Convert timestamp from tuple to number and start at zero instead of epoch seconds
	initTime = timestamp[0][0]
	timeDiff = []
	for i in range(0, len(timestamp)):
		timestamp[i] = timestamp[i][0] - initTime
		#Find the time difference between measurements
		if i >1:
			timeDiff.append(timestamp[i] - timestamp[i-1])
			#Remove backwards time travel
			if timeDiff[-1] < 0:
				timeDiff[-1] = 0

		#Remove arbitrary negative timestamps
		if (timestamp[i] < 0) and (i>1):
			timestamp[i] = timestamp[i-1]





	#convert ADS data from tuple to not that (Assumes all ads have the same number of data points)

	for i in range(0, len(ads1)):
		ads1[i] = ads1[i][0]
		ads2[i] = ads2[i][0]
		ads3[i] = ads3[i][0]
		ads4[i] = ads4[i][0]


	#Package variables for plotter function:
	time = [timestamp, timeDiff]
	ads = [ads1, ads2, ads3, ads4]
	temp = [spec_temp, rpi_temp, hk_temp_bat1, hk_temp_bat2, hk_temp_slc, env_temp]
	

	#Check for discontinuities in the data, split plots accordingly
	for i in range(0, len(timeDiff)):
		#If time between two measurements is greater than 2hr, split the list
		if timeDiff[i] > 5000:
			dataSplit(spectrum, time, ads, temp, env_hum, dataFile, i)
			modTime = timestamp
			modTime[:] = [t - timestamp[i] for t in timestamp]
			timeT = [modTime[i+2:-1], timeDiff[i+2:-1]]
			adsT = [ads1[i+2:-1], ads2[i+2:-1], ads3[i+2:-1], ads4[i+2:-1]]
			tempT = [spec_temp[i+2:-1], rpi_temp[i+2:-1], hk_temp_bat1[i+2:-1], hk_temp_bat2[i+2:-1], hk_temp_slc[i+2:-1], env_temp[i+2:-1]]
			plotter(spectrum, tempT, adsT, timeT, env_hum, dataFile, i)
			break

		elif i == len(timeDiff):

			plotter(spectrum, temp, ads, time, env_hum, dataFile, 0)

	


def dataSplit(spec, time, ads, temp, env_hum, dataFile, i):
	timeTemp = [time[0][0:i], time[1][0:i]]
	adsTemp = [ads[0][0:i], ads[1][0:i], ads[2][0:i],ads[3][0:i]]
	tempTemp = [temp[0][0:i], temp[1][0:i], temp[2][0:i], temp[3][0:i], temp[4][0:i], temp[5][0:i]]
	plotter(spec, tempTemp, adsTemp, timeTemp, env_hum, dataFile, i)

			





def plotter(spec, temp, ads,time, hum, dataFile, i):

	#Unpack all data:
	timestamp = time[0]
	timeDiff = time[1]
	ads1 = ads[0]
	ads2 = ads[1]
	ads3 = ads[2]
	ads4 = ads[3]
	spec_temp = temp[0]
	rpi_temp = temp[1]
	hk_temp_bat1 = temp[2]
	hk_temp_bat2 = temp[3]
	hk_temp_slc = temp[4]
	env_temp = temp[5]

	#Create nameStr for files based on current data file name:
	nameStr = ''
	for letter in dataFile:
		if letter == '/':
			break
		else:
			nameStr = nameStr + letter + str(i)



	#Plot Temperature data
	plt.figure()
	#Commented for TVAC
	#plt.plot(timestamp, spec_temp, '.',label="Spectometer")
	plt.plot(timestamp, rpi_temp,  '.', label="Raspberry Pi")
	plt.plot(timestamp, hk_temp_bat1, '.', label="HK Battery 1")
	plt.plot(timestamp, hk_temp_bat2, '.', label="HK Battery 2")
	#plt.plot(timestamp, hk_temp_slc, '.', label="HK SLC")
	plt.plot(timestamp, env_temp, '.',label="Env. Temperature")

	plt.title("Temperature Data")
	plt.xlabel("Time (s)")
	plt.ylabel("Temperature (Deg C)")
	plt.legend(loc='upper left')

	#Save fig to file
	plt.savefig('temp'+ nameStr +'.png')



	#Plot ADS information

	# plt.figure()
	# plt.plot(timestamp, ads1, label="ADS 1")
	# plt.plot(timestamp, ads2, label="ADS 2")
	# plt.plot(timestamp, ads3, label="ADS 3")
	# plt.plot(timestamp, ads4, label="ADS 4")

	# plt.title("ADS Data")
	# plt.xlabel("Time (s)")
	# plt.ylabel("? (?)")#FIX THIS WITH REAL VALUES-------------------------------------
	# plt.legend(loc='upper left')
	# plt.savefig('ADS'+nameStr+'.png')




	#Plot timing values:

	plt.figure()
	plt.plot(timeDiff, label="Time Difference")

	plt.title("Time Difference")
	plt.xlabel("Measurement Number")
	plt.ylabel("Time Difference (s)")
	plt.savefig('timeDiff'+ nameStr + '.png')
	










#Call function to plot data

#Specify folder structure
files = ['MLCDot/datafile']

#loop through the files names for each type of drive
for name in files:
	plotterFun(name)

#Enable if you want to see plots and not just save them:
plt.show()

