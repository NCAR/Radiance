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

def getFiles(directory):
    #files = "C:\\HAO-IG\\DIMS\\Software\\Data\\sun_angled\\"
    #files = "C:\\HAO-IG\\DIMS\\Software\\Data\\datafile_7-12-2017_1000ms"
    #directory = "C:\\HAO-IG\\DIMS\\Software\\Data\\omega_thermal_vac\\"
    
    allfiles=os.listdir(directory)
    files = [onefile for onefile in allfiles if onefile.endswith(".dat")]
    files = [directory + s for s in files]    
    return(files)    

def getData(files):
    
    #Not sure yet if we'll be looping over many files?    
    file = files[0]

    #length of data from 1 cycle. 
    #Need to figure out how to make this automatic    
    #last 4 is for the exposure time
    cycleSize = 8240+2048*4+2+4
 
    #Total file size
    fileSize=0
    for file in files: 
        fileSize += os.path.getsize(file)

    #number of data cycles
    numCycles = int(fileSize/cycleSize) #* len(files)      
        
    print(cycleSize,fileSize,numCycles)

	#Initilize everything
    timestamp= np.zeros(numCycles)
    times=np.zeros(numCycles)
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
    exposure=np.zeros(numCycles)
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
        print(file)
        with open(file,'rb') as f:
        
            cycleStartByte = 0 #used to start at offset, not sure why we'd skip the 1st cycle?
    

            
    
            
            while cycleStartByte < (fileSize):#-cycleSize):
                f.seek(cycleStartByte, 0)
                timestamp[cycleCount], = (struct.unpack('I',f.read(4)))
                times[cycleCount] = timestamp[cycleCount]-timestamp[0]
                #print(timestamp[cycleCount])
                
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
                exposure[cycleCount], = (struct.unpack('f',f.read(4)))
    
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
    dataStructure = {'timestamp':timestamp,'times':times,'spec_temp':spec_temp,'computer_temp':rpi_temp,'battery1_temp':bat1_temp,'battery2_temp':bat2_temp,'slc_temp':slc_temp,'env_temp':env_temp,'env_hum':env_hum,'exposure':exposure,'ads1':ads1,'ads2':ads2,'ads3':ads3,'ads4':ads4,'spec_heater':spectrometer_heater_status,'batt_heater':battery_heater_status}
    data = pd.DataFrame(dataStructure)
    return(data,wavelength,spectrum)
        
def extractData():
    pass

def plotTemperatures(data,ax):
    
    heater_on = 1   
    heater_off = 3    
    times = data['times']
    ax.plot(data['times'],data['spec_temp'],label='Spectrometer Temperature')
    ax.plot(data['times'],data['computer_temp'],label='Computer Temperature')
    ax.plot(data['times'],data['battery1_temp'],label='Battery 1 Temperature')
    ax.plot(data['times'],data['battery2_temp'],label='Battery 2 Temperature')
    #ax1.plot(data['slc_temp'],label='SLC Temperature')
    ax.plot(data['times'],data['spec_heater']*10,label='Spectrometer Heater',linestyle='-',color='b')
    ax.plot(data['times'],data['batt_heater']*5,label='Battery Heater',linestyle='-',color='c')
    ax.plot([0,np.amax(times)],[heater_on,heater_on],'--g',label='Heater Set Point On')
    ax.plot([0,np.amax(times)],[heater_off,heater_off],'--r',label='Heater Set Point Off')
    #ax1.plot(data['env_temp'],label="Environmental Temperature",marker='v')
    #ax1.plot(data['env_hum'],label="Environmental Humidity",marker='^')
    #ax.set_xlim([np.amin(timelimits),np.amax(timelimits)])
    ax.set_xlabel("Time [s]")
    ax.set_ylabel("Temperature [C]")
    ax.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3, ncol=3, mode="expand", borderaxespad=0.)
    ax.set_xlim(left=0)
    ax.grid(True) 
   

    
def plotADS(data,ax):

    ax.plot(data['times'],data['ads1'],label='ADS 1')    
    ax.plot(data['times'],data['ads2'],label='ADS 2')    
    ax.plot(data['times'],data['ads3'],label='ADS 3')    
    ax.plot(data['times'],data['ads4'],label='ADS 4')  
    ax.set_xlabel("Time [s]")
    ax.set_ylabel("ADS Value")
    ax.set_xlim(left=0)
    ax.grid(True)
    ax.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3, ncol=4, mode="expand", borderaxespad=0.)
    
    
def plotSpectrum(data,wavelength,spectrum,timeToPlot,ax,axx):
    #fig,(ax1,ax2,ax3)=plt.subplots(3,1,sharex=True)    
    #fig,(ax1)=plt.subplots(1,1)    

    #Measured Noise
    rmsNoise = np.std(spectrum[1:10,:],axis=0)
    aveSpectrum = np.mean(spectrum[1:10,:],axis=0)    

    #Expected Noise
    poissonNoise = np.sqrt(aveSpectrum)

    #Measured SNR
   # measuredSNR = np.divide(aveSpectrum,rmsNoise)
    
    #Expected SNR
    #poissonSNR = np.divide(aveSpectrum,poissonNoise)
    #times = data['times']
    #times=np.array(times - times[0]) 
    #nonzero=np.where(times>0)
    #timelimits = times[nonzero]
    
    #whichspectrum = 70
    #whichspectrum = np.argmax(data['times']>timeToPlot)
    whichspectrum = np.where(data['times']>timeToPlot)[0][0]
    plt.subplots_adjust(hspace=0.5)
    axx.grid(True)    
    axx.plot(wavelength[whichspectrum,:],spectrum[whichspectrum,:],'b')
    axx.set_title("Selected Spectrum (time = "+str(timeToPlot) + "s)")
    axx.set_xlabel("Wavelength [nm]")
    axx.set_ylabel("Counts")
    axx.set_xlim([200,1000])
    #axx.set_ylim([0,np.amax(spectrum[whichspectrum,:])*1.05])
    axx.set_ylim([0,np.amax(spectrum[whichspectrum,:])*1.05])
    #ax2.plot(wavelength[whichspectrum,:],rmsNoise,'b',label="Measured")
    #ax2.plot(wavelength[whichspectrum,:],poissonNoise,'r',label="Expected")
    #ax2.legend()
    #ax3.plot(wavelength[whichspectrum,:],measuredSNR,'b',label="Measured")
    #ax3.plot(wavelength[whichspectrum,:],poissonSNR,'r',label="Expected")
    #ax3.legend()

    #fig,(ax1)=plt.subplots(1,1)

    ax.plot(data['times'],np.amax(spectrum,axis=1),'r',label="Peak Counts per Spectrum")    

    
    ax.plot([data['times'][whichspectrum],data['times'][whichspectrum]],[0,65000],'b',label="Selected Spectrum")
    #ax3.set_xlim([np.amin(timelimits),np.amax(timelimits)])
    ax.set_xlabel("Time [s]")
    ax.set_ylabel("Peak Counts")
    ax.set_xlim(left=0)
    ax.set_ylim([0,np.amax(spectrum)*1.05])
    ax.yaxis.label.set_color('red')
    ax.tick_params(axis='y',colors='red')
    axy2 = ax.twinx()
    axy2.plot(data['times'],data['exposure'],'--g',label="Exposure Time")
    axy2.set_ylabel("Exposure Times [ms]")
    axy2.yaxis.label.set_color('green')
    axy2.tick_params(axis='y',colors='green')
    axy2.set_ylim([0,np.amax(data['exposure'])*1.05])
    ax.grid(True)
   
def calibrateIrradiance(wavelength, spectrum,ax1):
    
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
    #fig,(ax1)=plt.subplots(1,1)    
    print(wavelength[0,330])
    print(irradiance.shape)
    ax1.plot(wavelength[20,:],irradiance[20,:])
    ax1.set_xlim([350,1100])
    ax1.set_ylim([0,100000])

    return(irradiance)
    
def saveData():
    pass


def main():
    
    #====================================INPUTS====================================
    directory = "C:\\HAO-IG\\DIMS\\Software\\Data\\4-12-2018\\"
    timeToPlot = 500
    #====================================INPUTS====================================


    files = getFiles(directory)
    data,wavelength,spectrum = getData(files)
    extractData()

    fig = plt.subplots()
    ax1=plt.subplot(321)
    ax2=plt.subplot(323)
    ax3=plt.subplot(325)
    ax4=plt.subplot(122)
    
    plotTemperatures(data,ax1)
    plotADS(data,ax2)
    #fig,(ax1,ax2,ax3)=plt.subplots(3,1)
    #fig,(ax2,ax3)=plt.subplots(2,1)
    #irradiance = calibrateIrradiance(wavelength,spectrum,ax1)
    #irradiance = spectrum    
    plotSpectrum(data,wavelength,spectrum,timeToPlot,ax3,ax4)
    saveData()

if __name__ == "__main__":
    main()