# -*- coding: utf-8 -*-
"""
Created on Sun Jul 30 14:15:48 2017

@author: oakley
"""

import os
#import sys
import struct
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

def getFiles():
    #files = "C:\\HAO-IG\\DIMS\\Software\\Data\\sun_angled\\"
    #files = "C:\\HAO-IG\\DIMS\\Software\\Data\\datafile_7-12-2017_1000ms"
    #directory = "C:\\HAO-IG\\DIMS\\Software\\Data\\omega_thermal_vac\\"
    directory = "C:\\HAO-IG\\DIMS\\Software\\Data\\3-1-2018\\"
    files=os.listdir(directory)
    files = [directory + s for s in files]    
    return(files)    

def getData(files):
    
    #Not sure yet if we'll be looping over many files?    
    file = files[0]

    #length of data from 1 cycle. 
    #Need to figure out how to make this automatic    
    cycleSize = 8240+2048*4+2
 
    #Total file size
    fileSize=0
    for file in files: 
        fileSize += os.path.getsize(file)

    #number of data cycles
    numCycles = fileSize/cycleSize #* len(files)      
        
    print(cycleSize,fileSize,numCycles)

	#Initilize everything
    timestamp= np.zeros(numCycles)
    wavelength=np.zeros([numCycles,2048])
    spectrum= np.zeros([numCycles,2048])
    #wavelength = []
    #spectrum = []    
    
    spec_temp=np.zeros(numCycles)
    rpi_temp=np.zeros(numCycles)
    bat1_temp=np.zeros(numCycles)
    bat2_temp=np.zeros(numCycles)
    slc_temp=np.zeros(numCycles)
    env_temp=np.zeros(numCycles)
    env_hum=np.zeros(numCycles)
    ads1=np.zeros(numCycles)
    ads2=np.zeros(numCycles)
    ads3=np.zeros(numCycles)
    ads4=np.zeros(numCycles)
    
    spectrometer_heater_status = np.zeros(numCycles)
    battery_heater_status = np.zeros(numCycles)
    
    #not used yet, but might be useful        
    cycleCount = 0
    #Open file to read
    for file in files:
        #Total file size
        fileSize = os.path.getsize(file)
        with open(file,'rb') as f:
        
            cycleStartByte = 0 #used to start at offset, not sure why we'd skip the 1st cycle?
    

            
    
            
            while cycleStartByte < (fileSize-cycleSize):
                f.seek(cycleStartByte, 0)
                timestamp[cycleCount], = (struct.unpack('I',f.read(4)))
                
                #wavelength.append(struct.unpack('2048f',f.read(2048*4)))
                wavelength[cycleCount,:] = np.asarray(struct.unpack('2048f',f.read(2048*4)))
                spectrum[cycleCount,:] = np.asarray(struct.unpack('2048f',f.read(2048*4)))
                #spectrum.append(struct.unpack('2048f',f.read(2048*4)))
    
                spec_temp[cycleCount], = struct.unpack('f',f.read(4))
                rpi_temp[cycleCount], = (struct.unpack('f',f.read(4)))
                bat1_temp[cycleCount], = (struct.unpack('f',f.read(4)))
                bat2_temp[cycleCount], = (struct.unpack('f',f.read(4)))
                slc_temp[cycleCount], = (struct.unpack('f',f.read(4)))
                env_temp[cycleCount], = (struct.unpack('f',f.read(4)))
                env_hum[cycleCount], = (struct.unpack('f',f.read(4)))
    
                ads1[cycleCount], = (struct.unpack('f',f.read(4)))
                ads2[cycleCount], = (struct.unpack('f',f.read(4)))
                ads3[cycleCount], = (struct.unpack('f',f.read(4)))
                ads4[cycleCount], = (struct.unpack('f',f.read(4)))
    
                spectrometer_heater_status[cycleCount], = (struct.unpack('?',f.read(1)))
                battery_heater_status[cycleCount], = (struct.unpack('?',f.read(1)))
    
    
                cycleStartByte = cycleStartByte + cycleSize
                cycleCount += 1
    
    
#            print(bat1_temp)     
#            print(ads3)     
#            print(ads4)     
    
            #wavelength = np.asarray(wavelength)        
            #spec_temp = spec_temp[1:]
            #spec_temp = np.array(spec_temp)
    dataStructure = {'times':timestamp,'spec_temp':spec_temp,'computer_temp':rpi_temp,'battery1_temp':bat1_temp,'battery2_temp':bat2_temp,'slc_temp':slc_temp,'env_temp':env_temp,'env_hum':env_hum,'ads1':ads1,'ads2':ads2,'ads3':ads3,'ads4':ads4,'spec_heater':spectrometer_heater_status,'batt_heater':battery_heater_status}
    data = pd.DataFrame(dataStructure)
    return(data,wavelength,spectrum)
        
def extractData():
    pass

def plotTemperatures(data):
    
    heater_on = 19#1   
    heater_off = 21#3    
    fig,(ax1)=plt.subplots()    
    times = data['times']
    times=np.array(times - times[0]) 
    nonzero=np.where(times>0)
    timelimits = times[nonzero]
    ax1.plot(times,data['spec_temp'],label='Spectrometer Temperature')
    ax1.plot(times,data['computer_temp'],label='Computer Temperature')
    ax1.plot(times,data['battery1_temp'],label='Battery 1 Temperature')
    ax1.plot(times,data['battery2_temp'],label='Battery 2 Temperature')
    #ax1.plot(data['slc_temp'],label='SLC Temperature')
    ax1.plot(times,data['spec_heater']*10,label='Spectrometer Heater',linestyle='-',color='b')
    ax1.plot(times,data['batt_heater']*5,label='Battery Heater',linestyle='-',color='c')
    ax1.plot([0,np.amax(times)],[heater_on,heater_on],'--g',label='Heater Set Point On')
    ax1.plot([0,np.amax(times)],[heater_off,heater_off],'--r',label='Heater Set Point Off')
    #ax1.plot(data['env_temp'],label="Environmental Temperature",marker='v')
    #ax1.plot(data['env_hum'],label="Environmental Humidity",marker='^')
    ax1.set_xlim([np.amin(timelimits),np.amax(timelimits)])
    ax1.legend()
    
  
    
    #dataplot = data.Series(data['spec_temp'])
    #xx=np.array(data['spec_temp'])
    #plt.plot(xx)
    #data.plot(x='spec_temp',y='spec_temp')    
    

    
def plotADS(data):
    fig,(ax1)=plt.subplots()    

    ax1.plot(data['ads1'],label='ADS 1')    
    ax1.plot(data['ads2'],label='ADS 2')    
    ax1.plot(data['ads3'],label='ADS 3')    
    ax1.plot(data['ads4'],label='ADS 4')  
    ax1.legend()
    
    
def plotSpectrum(data,wavelength,spectrum):
    #fig,(ax1,ax2,ax3)=plt.subplots(3,1,sharex=True)    
    fig,(ax1)=plt.subplots(1,1)    

    #Measured Noise
    rmsNoise = np.std(spectrum[1:10,:],axis=0)
    aveSpectrum = np.mean(spectrum[1:10,:],axis=0)    

    #Expected Noise
    poissonNoise = np.sqrt(aveSpectrum)

    #Measured SNR
    measuredSNR = np.divide(aveSpectrum,rmsNoise)
    
    #Expected SNR
    poissonSNR = np.divide(aveSpectrum,poissonNoise)
    
    whichspectrum = 1
    
    ax1.plot(wavelength[whichspectrum,:],spectrum[whichspectrum,:],'b')
    ax1.set_xlim([350,1000])
    plt.grid()
    #ax2.plot(wavelength[whichspectrum,:],rmsNoise,'b',label="Measured")
    #ax2.plot(wavelength[whichspectrum,:],poissonNoise,'r',label="Expected")
    #ax2.legend()
    #ax3.plot(wavelength[whichspectrum,:],measuredSNR,'b',label="Measured")
    #ax3.plot(wavelength[whichspectrum,:],poissonSNR,'r',label="Expected")
    #ax3.legend()

    fig,(ax1)=plt.subplots(1,1)
    times = data['times']
    times=np.array(times - times[0]) 
    nonzero=np.where(times>0)
    timelimits = times[nonzero]
    ax1.plot(times,np.amax(spectrum,axis=1))    
    ax1.set_xlim([np.amin(timelimits),np.amax(timelimits)])

def calibrateIrradiance(wavelength, spectrum):
    
    calibration_file = "C:\\HAO-IG\\DIMS\\Software\\calibration_minimal.csv"
    calibration = np.genfromtxt(calibration_file,dtype='float',delimiter=',',skip_header=1)
    calwave = calibration[:,0]    
    dark = calibration[:,1]
    caldark = np.interp(wavelength[0,:], calwave, dark)
    cal = calibration[:,2]    
    calint = np.interp(wavelength[0,:],calwave,cal)
    irradiance = np.zeros(spectrum.shape)
    cal2=calibration[:,3]
    for spec in range(spectrum.shape[0]):
        irradiance[spec,:] = np.subtract(spectrum[spec,:],spectrum[10,:])
        irradiance[spec,:] = np.divide(irradiance[spec,:],cal2)  
    fig,(ax1)=plt.subplots(1,1)    
    print(wavelength[0,330])
    print(irradiance.shape)
    ax1.plot(wavelength[20,:],irradiance[20,:])
    ax1.set_xlim([350,1100])
    ax1.set_ylim([0,100000])

    return(irradiance)
    
def saveData():
    pass


def main():
    files = getFiles()
    data,wavelength,spectrum = getData(files)
    extractData()
    plotTemperatures(data)
    plotADS(data)
    irradiance = calibrateIrradiance(wavelength,spectrum)
    #irradiance = spectrum    
    plotSpectrum(data,wavelength,spectrum)
    saveData()


if __name__ == "__main__":
    main()