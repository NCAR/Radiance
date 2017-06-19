# RADIANCE-main
Software repository for the RADIANCE senior project.

# Configuration
- Ensure the heater and sensor pin numbers are correct

# Requirements
- Install the pigpio, avaspec, and raspicam packages.

  To install pigpio: http://abyz.co.uk/rpi/pigpio/download.html (after following instructions you can remove master.zip from the Pi)
  
  To install avaspec: anywhere in the directory structure (on the Pi), run: 
  
  ```
  pi@raspberrypi:~ $ wget http://www.jennykampmeier.com/libavs_2.0.1.0_armhf.deb
  pi@raspberrypi:~ $ sudo dpkg -i libavs_2.0.1.0_armhf.deb
  pi@raspberrypi:~ $ rm libavs_2.0.1.0_armhf.deb
  ```
  To install raspicam, download file from Sourceforge: https://downloads.sourceforge.net/project/raspicam/raspicam-0.1.6.zip, then follow instructions here: http://www.uco.es/investiga/grupos/ava/node/40

- Enable access to I2C, SPI, and 1-Wire via `sudo raspi-config`:
  ```
  pi@raspberrypi:~ $ sudo raspi-config
  ```
  Arrow-down and choose `5 Interfacing Options`. Arrow-down and choose `P1 Camera`. Choose `Yes` when prompted. Repeat for `P4 SPI`, `P5 I2C` and `P7 1-Wire`. When it asks you to reboot, say `Yes`.
  
- Edit the crontab and add the following two lines beneath the comments (note if you've done a fresh install the crontab won't exist, but it will make one automatically):
  ```
  pi@raspberrypi:~ $ crontab -e
  ```
  ```
  @reboot sh /home/pi/RADIANCE-main/util/startup.sh
  */1 * * * * /home/pi/RADIANCE-main/util/restart.sh radiance
  ```
  Note: The second line will cause the RPi to restart every minute if the RADIANCE software is not running.
  
- Mount, format, and configure the USB drives as ext4 and data=journal: 
  ```
  pi@raspberrypi:~ $ sudo nano /etc/fstab
  ```
  Then add the following lines at the end of the document:
  ```
  UUID=3f7681a4-bed7-4bbf-b1fd-966d3cc6f79f    /mnt/slcdrive  ext4  data=journal  0  0
  UUID=5c0d5816-9b30-4041-a98d-71844d1dd6b9    /mnt/mlcdrive1 ext4  data=journal  0  0
  UUID=8f7a6e96-8b9a-428c-bb17-ddd595607f23    /mnt/mlcdrive2 ext4  data=journal  0  0
  ```
  Exit and save the `fstab` file by doing a Ctrl+X, type "Y" to save, then press Enter.  Restart the pi and make sure the folders are all mounted properly. Create an "images" folder in each mlc drive:

  ```
  mkdir /mnt/mlcdrive1/images
  mkdir /mnt/mlcdrive2/images
  ```
  
- Enable the hardware watchdog: https://www.domoticz.com/wiki/Setting_up_the_raspberry_pi_watchdog
  ```
  pi@raspberrypi:~ $ sudo modprobe bcm2835-wdt
  pi@raspberrypi:~ $ echo "bcm2835-wdt" | sudo tee -a /etc/modules
  pi@raspberrypi:~ $ sudo apt-get install watchdog
  pi@raspberrypi:~ $ sudo update-rc.d watchdog defaults
  pi@raspberrypi:~ $ sudo nano /etc/watchdog.conf
  ```
  In the text editor, uncomment the line (delete the `#`) that starts with `#max-load-1` and uncomment the line that starts with `#watchdog-device`. Exit and save the file by doing a Ctrl+X, type "Y" to save, then press Enter. Finally, start the watchdog service:
  ```
  pi@raspberrypi:~ $ sudo service watchdog start
  ```
- Create the log pipe:
```
  pi@raspberrypi:~ $ sudo mkfifo radiance_log
  ```
# Sync with Git and Compile
To download/compile:
```
pi@raspberrypi:~ $ git clone https://github.com/JamesPavek/RADIANCE-main.git
pi@raspberrypi:~ $ cd RADIANCE-main
pi@raspberrypi:~ $ sudo make
```

# If needed, update RADIANCE software
- Do the following on the Pi:
```
pi@raspberrypi:~ $ cd ~/RADIANCE-main/
pi@raspberrypi:~ $ git reset --hard
pi@raspberrypi:~ $ git pull
```
This will update the software and grab the header files for the `include` folder.


# Execution

To run:
```
sudo ./radiance
```

# Restarting the Pi
To restart the Pi (if you made configuration changes and want to be sure that they are reflected, for example), type this anywhere in the directory structure on the Pi:
```
pi@raspberrypi:~ $ /home/pi/RADIANCE-main/util/restart.sh
```
