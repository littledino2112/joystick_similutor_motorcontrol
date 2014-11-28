from Tkinter import Label, Tk, Frame, StringVar, Button, LEFT, RIGHT, BOTTOM, Entry, LabelFrame, Checkbutton, IntVar
import pygame
import serial, glob, sys
import ttk
import tkMessageBox
import time

DEBUG = 1

class GUI(Frame):
      """docstring for GUI"""
      def __init__(self, parent):
            Frame.__init__(self,parent)
            self.parent = parent    # parent is Tk() object

            # Initialize pygame
            pygame.init()
            self.joystickFound = 0
            self.serialStarted = False
            self.calibratedAxes = False
            # Check if there is any joystick connected
            if (pygame.joystick.get_count()):
                  self.joystickFound = 1
                  self.total_Joystick = pygame.joystick.get_count()
                  if (DEBUG):
                          print "Joystick found: {}".format(self.total_Joystick)

                  # Get useful info from the joystick
                  for i in range(self.total_Joystick):
                          self.joystick = pygame.joystick.Joystick(i)
                          self.joystick.init()

                          # Get Joystick name
                          self.name_Joystick = self.joystick.get_name()
                          # Get Joystick number of axes
                          self.total_Axes = self.joystick.get_numaxes()
                          # Get Joystick number of buttons
                          self.total_Buttons = self.joystick.get_numbuttons()

                          if (DEBUG):
                                  print "Joystick {}".format(i)
                                  print "Joystick's name: {}".format(self.name_Joystick)
                                  print "Joystick's total axes: {}".format(self.total_Axes)
                                  print "Joystick's total buttons: {}".format(self.total_Buttons) 
            self.initGUI()

      def initGUI(self):
            self.parent.title("Joystick GUI v2.1")
            self.pack(fill='both')

            
            if (self.joystickFound):
                  self.lblJoystickNum = Label(self, text="Number of joystick: {}".format(self.total_Joystick))
                  self.lblJoystickNum.pack()
                  self.lblJoystickName = Label(self, text="Joystick name: {}".format(self.name_Joystick))
                  self.lblJoystickName.pack()
                  self.lblAxisNumbers = Label(self, text="Number of axes: {}".format(self.total_Axes))
                  self.lblAxisNumbers.pack()
                  self.lblButtonNumbers = Label(self, text="Number of buttons: {}".format(self.total_Buttons))
                  self.lblButtonNumbers.pack()

                  # Show axis value
                  self.list_Axes = list()  # create a list to contain axes' value
                  # self.list_lblAxes = list()
                  self.list_chkbtnAxes = list()
                  self.list_chkbtnSelect = list()
                  self.list_calibratedMiddlePoint = list()
                  for i in range(self.joystick.get_numaxes()):
                        self.list_Axes.append(0)      # Initialize value for axis is 0
                        self.list_chkbtnSelect.append(IntVar())
                        self.list_chkbtnAxes.append(Checkbutton(self, text="Axis {0:d}: {1:.2f}".format(i, self.list_Axes[i]),variable=self.list_chkbtnSelect[i]))
                        self.list_chkbtnAxes[i].pack()

                  # Add 3 entry texts which are used to config motors' speed.
                  label_frame_motor_config = LabelFrame(self, text="Motors' Speed configuration")
                  label_frame_motor_config.pack()
                  frame_motor1 = Frame(label_frame_motor_config)
                  frame_motor1.pack(fill='both')
                  lblMotorSpeed1 = Label(frame_motor1, text="Motor 1's speed ")
                  lblMotorSpeed1.pack(side=LEFT)
                  self.motorspeed1 = StringVar()
                  self.entryMotorSpeed1 = Entry(frame_motor1, textvariable=self.motorspeed1)
                  self.motorspeed1.set("255")
                  self.entryMotorSpeed1.pack(side=LEFT)
                  frame_motor2 = Frame(label_frame_motor_config)
                  frame_motor2.pack(fill='both')
                  lblMotorSpeed2 = Label(frame_motor2, text="Motor 2's speed ")
                  lblMotorSpeed2.pack(side=LEFT)
                  self.motorspeed2 = StringVar()
                  self.entryMotorSpeed2 = Entry(frame_motor2, textvariable=self.motorspeed2)
                  self.motorspeed2.set("255")
                  self.entryMotorSpeed2.pack(side=LEFT)
                  frame_motor3 = Frame(label_frame_motor_config)
                  frame_motor3.pack(fill='both')
                  lblMotorSpeed3 = Label(frame_motor3, text="Motor 3's speed ")
                  lblMotorSpeed3.pack(side=LEFT)
                  self.motorspeed3 = StringVar()
                  self.entryMotorSpeed3 = Entry(frame_motor3, textvariable=self.motorspeed3)
                  self.motorspeed3.set("255")
                  self.entryMotorSpeed3.pack(side=LEFT)
                  self.btnMotorSpeed = Button(label_frame_motor_config, text="Change", command=self.configMotorSpeed)
                  self.btnMotorSpeed.pack()

                  # Add one Frame used to contain serial port selection (Combobox)
                  serialPortResults = self.setUpSerialPorts()
                  frame_serialport = Frame(self)
                  frame_serialport.pack(fill='both')
                  Label(frame_serialport, text="Serial Port selection").pack(side=LEFT)
                  self.cbboxSerialPort = ttk.Combobox(frame_serialport, values=serialPortResults)     # Need to access this parameter for serial port initialization
                  self.cbboxSerialPort.pack(fill='both',side=LEFT)
                  btnStart = Button(frame_serialport, text="Start", command=self.startSerialPort)
                  btnStart.pack(side=LEFT)      

                  # Call periodic func to update axis value if Joystick is connected
                  self.after(100, self.periodicCall)
            else:
                  self.lblNoJoystickFound = Label(self, text="No Joystick Found!")
                  self.lblNoJoystickFound.pack()

            
            btnQuit = Button(self, text="Quit", command=self.quit)
            btnQuit.pack()

      def periodicCall(self): 
            done = False
            command = ""
            for event in pygame.event.get():
                  if (event.type==pygame.QUIT):
                          print event.type
                          done = True
            if (done==False):
                  if (self.calibratedAxes==False):
                        for i in range(self.joystick.get_numaxes()):
                              temp = self.joystick.get_axis(i)
                              self.list_calibratedMiddlePoint.append(float("{0:.2f}".format(temp)))
                              if DEBUG:
                                    print "Stored calibrated axis {}: {}".format(i, self.list_calibratedMiddlePoint[i])
                                    print "Calibrated axis {}: {}".format(i, temp)
                        self.calibratedAxes=True
                        self.after(100, self.periodicCall)
                  else:
                        for i in range(self.joystick.get_numaxes()):
                              self.list_Axes[i]=self.joystick.get_axis(i)            
                              self.list_chkbtnAxes[i].config(text="Axis {0:d}: {1:.2f}".format(i, self.list_Axes[i]))
                              if DEBUG:
                                    print "Axis {} value: {}".format(i, self.list_Axes[i])
                              if (self.serialStarted):
                                    if self.list_chkbtnSelect[i].get()==1:
                                          if DEBUG:
                                                print "Checkbutton {} select: {}".format(i, self.list_chkbtnSelect[i].get())
                                          if self.list_Axes[i]>self.list_calibratedMiddlePoint[i]+0.1:
                                                command = command + 'f'      
                                          elif self.list_Axes[i]<self.list_calibratedMiddlePoint[i]-0.1:
                                                command = command + 'b'
                                          else:
                                                command = command + 's'
                                          command = command + ','
                        command = command + '\0\n'
                        if (self.serialStarted):
                              self.selectedSerialPort.write(command) 
                              print command
                        self.update_idletasks()
                        self.after(100, self.periodicCall)
            else: 
                  print "Quit program"

      def setUpSerialPorts(self):
            if (sys.platform.startswith('win')):
                  ports = ['COM' + str(i+1) for i in range(256)]
            elif (sys.platform.startswith('linux')):
                  ports = glob.glob('/dev/tty[A-Za-z]*')
            elif (sys.platform.startswith('darwin')):
                  ports = glob.glob('/dev/tty.*')
            else:
                  raise EnvironmentError('Unsupported platform')

            results = []
            for port in ports:
                  try:
                        temp = serial.Serial(port)
                        temp.close()
                        results.append(port)
                  except (OSError, serial.SerialException):
                        pass
            return results

      def startSerialPort(self):
            selected = self.cbboxSerialPort.get()
            try:
                  self.selectedSerialPort = serial.Serial(selected, 9600, timeout=2)
                  self.serialStarted = True
            except (OSError, serial.SerialTimeoutException):
                  tkMessageBox.showinfo("Timout error connection to serial port")
            # self.selectedSerialPort.write('1234')

      def configMotorSpeed(self):
            command = "{},{},{},".format(self.motorspeed1.get(),self.motorspeed2.get(),self.motorspeed3.get()) + '\0' + '\n'
            if (self.serialStarted):
                  self.selectedSerialPort.write(command)            
            

def main():
        window = Tk()
        window.geometry("400x500+200+200")
        mainGUI = GUI(window)
        window.mainloop()


if __name__ == "__main__":
        main()
