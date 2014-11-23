from Tkinter import Label, Tk, Frame, StringVar, Button, LEFT, RIGHT, BOTTOM, Entry, LabelFrame
from threading import Timer
import pygame

DEBUG = 1

class GUI(Frame):
        """docstring for GUI"""
        def __init__(self, parent):
                Frame.__init__(self,parent)
                self.parent = parent    # parent is Tk() object

                # Initialize pygame
                pygame.init()
                self.joystickFound = 0
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
                self.parent.title("Joystick GUI")
                self.pack(fill='both')

                btnQuit = Button(self, text="Quit", command=self.quit)
                btnQuit.pack()
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
                        self.axis1=0
                        self.axis2=0
                        self.axis3=0
                        self.lblAxis1 = Label(self, text="Axis 1: {}".format(self.axis1))
                        self.lblAxis1.pack()
                        self.lblAxis2 = Label(self, text="Axis 2: {}".format(self.axis2))
                        self.lblAxis2.pack()
                        self.lblAxis3 = Label(self, text="Axis 3: {}".format(self.axis3))
                        self.lblAxis3.pack()

                        # Add 3 entry texts which are used to config motors' speed.
                        label_frame_motor_config = LabelFrame(self, text="Motors' Speed configuration")
                        label_frame_motor_config.pack()
                        frame_motor1 = Frame(label_frame_motor_config)
                        frame_motor1.pack(fill='both')
                        lblMotorSpeed1 = Label(frame_motor1, text="Motor 1's speed ")
                        lblMotorSpeed1.pack(side=LEFT)
                        self.entryMotorSpeed1 = Entry(frame_motor1)
                        self.entryMotorSpeed1.pack(side=LEFT)
                        frame_motor2 = Frame(label_frame_motor_config)
                        frame_motor2.pack(fill='both')
                        lblMotorSpeed2 = Label(frame_motor2, text="Motor 2's speed ")
                        lblMotorSpeed2.pack(side=LEFT)
                        self.entryMotorSpeed2 = Entry(frame_motor2)
                        self.entryMotorSpeed2.pack(side=LEFT)
                        frame_motor3 = Frame(label_frame_motor_config)
                        frame_motor3.pack(fill='both')
                        lblMotorSpeed3 = Label(frame_motor3, text="Motor 3's speed ")
                        lblMotorSpeed3.pack(side=LEFT)
                        self.entryMotorSpeed3 = Entry(frame_motor3)
                        self.entryMotorSpeed3.pack(side=LEFT)
                        self.btnMotorSpeed = Button(label_frame_motor_config, text="Submit")
                        self.btnMotorSpeed.pack()

                        # Call periodic func to update axis value if Joystick is connected
                        self.after(100, self.periodicCall)
                else:
                        self.lblNoJoystickFound = Label(self, text="No Joystick Found!")
                        self.lblNoJoystickFound.pack()
                # Timer
                #Timer(5, self.periodicCall, ()).start()

        def periodicCall(self): 
                done = False
                for event in pygame.event.get():
                        if (event.type==pygame.QUIT):
                                print event.type
                                done = True
                if (~done):
                        self.axis1=self.joystick.get_axis(0)            
                        self.axis2=self.joystick.get_axis(1)
                        self.axis3=self.joystick.get_axis(2)
                        self.lblAxis1.config(text="Axis 1: {}".format(self.axis1))
                        self.lblAxis2.config(text="Axis 2: {}".format(self.axis2))
                        self.lblAxis3.config(text="Axis 3: {}".format(self.axis3))
                        self.update_idletasks()
                        self.after(100, self.periodicCall)
                else: 
                        print "Quit program"

def main():
        window = Tk()
        window.geometry("400x400+200+200")
        mainGUI = GUI(window)
        window.mainloop()


if __name__ == "__main__":
        main()
