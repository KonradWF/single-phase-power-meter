from tkinter import *
from tkinter.ttk import Combobox
import matplotlib.animation as animation
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from GUI_Functions import *
import serial.tools.list_ports


xs = []
ys = []


measurement = Measurement(0)
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)


okno = Tk()
okno.configure(bg="white")
okno.title("Power Grid Measure")
canvas = FigureCanvasTkAgg(fig, master=okno)
canvas.get_tk_widget().grid(column=0,row=2, columnspan=25, rowspan=35, sticky=N)


label_1 = Label(okno, text='Save data to file', padx = 5, pady = 5, bg='brown', width=17, fg="white")
label_2 = Label(okno, text='P:', padx = 5, pady = 5, bg='brown', width=17, fg="white")
label_3 = Label(okno,  padx = 5, pady = 5,  width=17)
label_4 = Label(okno, text='Q:', padx = 5, pady = 5, bg='brown', width=17, fg="white")
label_5 = Label(okno,  padx = 5, pady = 5,  width=17)
label_6 = Label(okno, text='S:', padx = 5, pady = 5, bg='brown', width=17, fg="white")
label_7 = Label(okno,  padx = 5, pady = 5,  width=17)
label_8 = Label(okno, text='PF:', padx = 5, pady = 5, bg='brown', width=17, fg="white")
label_9 = Label(okno,  padx = 5, pady = 5,  width=17)
label_10 = Label(okno, text='U_RMS:', padx = 5, pady = 5, bg='brown', width=17, fg="white")
label_11 = Label(okno,  padx = 5, pady = 5,  width=17)
label_12 = Label(okno, text='I_RMS:', padx = 5, pady = 5, bg='brown', width=17, fg="white")
label_13 = Label(okno,  padx = 5, pady = 5,  width=17)
label_save_data = Label(okno, text='Saving INACTIVE', padx=5, pady=5, bg="white", fg="red")
label_14 = Label(okno, width=17, bg="white")

ports = serial.tools.list_ports.comports()
portchoosen = Combobox(okno, width = 17, textvariable = "Select port", postcommand=update_ports)
for port, desc, hwid in sorted(ports):
    temp_list = list(portchoosen['values'])
    temp_list.append(port)
    portchoosen['values'] = tuple(temp_list)
portchoosen.current(0)


buttonB = Button(okno, text='Start data acquisition', command=lambda:start_saving(measurement, label_save_data), padx=5, pady=5, height=1)
buttonC = Button(okno, text='Stop data acquisition', command=lambda:stop_saving(measurement, label_save_data), padx=5, pady=5, height=1)
buttonD = Button(okno, text='Exit', command=lambda:exit(measurement), padx=5, pady=5, bg="red", width=16, fg="white")
buttonSelect = Button(okno, text='Select port', command=lambda:select_port(measurement, portchoosen), padx=5, pady=5, bg="blue", width=16, fg="white")


label_1.grid(column=26, row=20, sticky=S)
label_2.grid(column=26, row=2, sticky=S)
label_3.grid(column=26, row=3,sticky=N)
label_4.grid(column=26, row=5,sticky=S)
label_5.grid(column=26, row=6,sticky=N)
label_6.grid(column=26, row=8,sticky=S)
label_7.grid(column=26, row=9,sticky=N)
label_8.grid(column=26, row=11,sticky=S)
label_9.grid(column=26, row=12,sticky=N)
label_10.grid(column=26, row=14,sticky=S)
label_11.grid(column=26, row=15,sticky=N)
label_12.grid(column=26, row=17,sticky=S)
label_13.grid(column=26, row=18,sticky=N)
buttonD.grid(column=26, row = 26, sticky=S)
portchoosen.grid(column=15, row = 0)
buttonSelect.grid(column=16, row = 0)
ani = animation.FuncAnimation(fig, animate, fargs=(xs, ys, ax, measurement), interval=meas_period)
update_label_power(label_3, measurement)
update_label_reactive(label_5, measurement)
update_label_apparent(label_7, measurement)
update_label_power_factor(label_9, measurement)
update_label_voltage(label_11, measurement)
update_label_current(label_13, measurement)
buttonB.grid(column = 26, row =24, sticky=S)
buttonC.grid(column = 26, row =25, sticky=N)
label_save_data.grid(column=26, row=21, sticky=S)
okno.mainloop()
