# Author: James Pavek
# Reads last measurements from data files and prints to screen

import os
import sys
import struct

def sizeof_fmt(num, suffix='B'):
    for unit in ['','Ki','Mi','Gi','Ti','Pi','Ei','Zi']:
        if abs(num) < 1024.0:
            return "%3.1f%s%s" % (num, unit, suffix)
        num /= 1024.0
    return "%.1f%s%s" % (num, 'Yi', suffix)

drive_list = ['/mnt/slcdrive','/mnt/mlcdrive1','/mnt/mlcdrive2']
# Offset from end of file, this is the total number of bytes of each measurement
offset = 8240
# Data readout format
data_format = """
Drive: {0}
Timestamp: {1}
ENV Hum: {2} %; ENV Temp: {3} C;
HK Temp SLC: {4} C;  HK Temp Bat1: {5} C;  HK Temp Bat2:{6} C
Spec Temp: {7} C; RPi Temp: {8} C; Spec: {9}
ADS1: {10} A; ADS2: {11} A; ADS3: {12} A; ADS4: {13} A;
"""

print('------')
for drive in drive_list:
    if 'mlc' in drive:
        type = 'mlc'
    elif 'slc' in drive:
        type = 'slc'
    else:
        print('Can\'t find drive type')

    # Reads in datafile and unpacks data from binary format
    with open(drive + '/datafile','rb') as f:
        f.seek(-offset,os.SEEK_END)
        timestamp = struct.unpack('I',f.read(4))
        spectrum = struct.unpack('2048f',f.read(2048*4))
        spec_temp = struct.unpack('f',f.read(4))
        rpi_temp = struct.unpack('f',f.read(4))
        hk_temp_bat1 = struct.unpack('f',f.read(4))
        hk_temp_bat2 = struct.unpack('f',f.read(4))
        hk_temp_slc = struct.unpack('f',f.read(4))
        env_temp = struct.unpack('f',f.read(4))
        env_hum = struct.unpack('f',f.read(4))
        ads1 = struct.unpack('f',f.read(4))
        ads2 = struct.unpack('f',f.read(4))
        ads3 = struct.unpack('f',f.read(4))
        ads4 = struct.unpack('f',f.read(4))


    print(data_format.format(drive,
                            timestamp,
                            env_hum,
                            env_temp,
                            hk_temp_slc,
                            hk_temp_bat1,
                            hk_temp_bat2,
                            spec_temp,
                            rpi_temp,
                            spectrum[0],
                            ads1,
                            ads2,
                            ads3,
                            ads4))


    # Print image file size if necessary
    if type=='mlc':
	if (os.path.isdir(drive+'/images')):
                print('Number of images: {num}'.format(num=len(os.listdir(drive+'/images'))))
	else:
		print('Could not find image directory')

print('------')
