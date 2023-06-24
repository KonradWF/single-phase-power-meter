import matplotlib.pyplot as plt
import datetime as dt
import sys
import re
import serial as serial

meas_period = 600


class Measurement:
    def __init__(self, flag):
        self.saving_data_flag = flag
        self.voltage_meas = 0.0
        self.current_meas = 0.0
        self.power_meas = 0.0
        self.reactive_power_meas = 0.0
        self.apparent_power_meas = 0.0
        self.power_factor_meas = 0.0
        self.current_file_name = ""
        self.serial = None
        self.serial_baudrate = 115200
        self.serial_name = ""


def open_port(self):
    if self.serial == None:
        self.serial = serial.Serial(port=self.serial_name, baudrate=self.serial_baudrate, timeout=2)
    else:
        # Serial already opened - close it
        self.serial.close()
        self.serial = serial.Serial(port=self.serial_name, baudrate=self.serial_baudrate, timeout=2)
    # self.serial.open()


def select_port(measurement, portchoosen):
    measurement.serial_name = portchoosen.get()
    open_port(measurement)

def update_ports(self):
    port_list = serial.tools.list_ports.comports()
    ports = [port.device for port in port_list]
    self.com_ports_list['values'] = ports


def start_saving(measurement, label):
    measurement.saving_data_flag = 1
    label.config(text="Saving ACTIVE", fg="green")
    file_name = "Measured_Data_" + dt.datetime.now().strftime('%H_%M_%S_%d_%m_%Y') + ".txt"
    measurement.current_file_name = file_name
    with open(file_name, 'w') as file2:
        file2.write(
            "Current_date" + "\t" + "Voltage [V]" + "\t" + "Current [A]" + "\t" + "Power_Factor [%]" + "\t" + "Power [W]" + "\t" + "Reactive_Power [W]" + "\t" + "Apparent_Power [W]" + "\t" + "\n")
        file2.close()


def stop_saving(measurement, label):
    measurement.saving_data_flag = 0
    label.config(text="Saving INACTIVE", fg="red")


def update_label_power(label, measurement):
    def update():
        label.config(text=str(measurement.power_meas) + " W")
        label.after(meas_period, update)

    update()


def update_label_reactive(label, measurement):
    def update():
        label.config(text=str(measurement.reactive_power_meas) + " W")
        label.after(meas_period, update)

    update()


def update_label_apparent(label, measurement):
    def update():
        label.config(text=str(measurement.apparent_power_meas) + " W")
        label.after(meas_period, update)

    update()


def update_label_power_factor(label, measurement):
    def update():
        label.config(text=str(measurement.power_factor_meas) + " %")
        label.after(meas_period, update)

    update()


def update_label_voltage(label, measurement):
    def update():
        label.config(text=str(measurement.voltage_meas) + " V")
        label.after(meas_period, update)

    update()


def update_label_current(label, measurement):
    def update():
        label.config(text=str(measurement.current_meas) + " A")
        label.after(meas_period, update)

    update()


def safe_data_to_file(measurement):
    if (measurement.saving_data_flag == 1):
        with open(measurement.current_file_name, 'a') as file:
            # file.write("Current_date" + "\t" + "Voltage [V]"+"\t" + "Current [A]"+"\t"+ "Power_Factor [%]"+"\t"+ "Power [W]"+"\t"+ "Reactive_Power [W]"+"\t"+ "Apparent_Power [W]"+"\t" + "\n")
            file.write(
                dt.datetime.now().strftime('%H:%M:%S,%d.%m.%Y') + "\t" + str(measurement.voltage_meas) + "\t" + str(
                    measurement.current_meas) + "\t" + str(measurement.power_factor_meas) + "\t" + str(
                    measurement.power_meas) + "\t" + str(measurement.reactive_power_meas) + "\t" + str(
                    measurement.apparent_power_meas) + "\t\n")
            file.close()


def exit(measurement):
    measurement.serial.close()
    print('Closed')
    sys.exit()


def animate(i, xs, ys, ax, measurement):
    # Read and correct received data
    data = ""
    if measurement.serial is not None:
        data += str(measurement.serial.readline())
    # data.decode()
    data = data.replace("b'", "")
    data = data.replace("'", "")
    pattern = "U=(?P<voltage>[0-9,.]+);I=(?P<current>[0-9,.]+);PF=(?P<power_factor>[0-9,.]+);P=(?P<power>[0-9,.]+);Q=(?P<reactive_power>[0-9,.]+);S=(?P<apparent_power>[0-9,.]+);"
    result = re.match(pattern, data)
    if result is not None:
        measurement.power_meas = result["power"]
        measurement.apparent_power_meas = result["apparent_power"]
        measurement.reactive_power_meas = result["reactive_power"]
        measurement.power_factor_meas = result["power_factor"]
        measurement.voltage_meas = result["voltage"]
        measurement.current_meas = result["current"]
        ys.append(float(measurement.power_meas))
    else:
        measurement.power_meas = "b/d"
        measurement.apparent_power_meas = "b/d"
        measurement.reactive_power_meas = "b/d"
        measurement.power_factor_meas = "b/d"
        measurement.voltage_meas = "b/d"
        measurement.current_meas = "b/d"
        ys.append(0)

    safe_data_to_file(measurement)
    xs.append(dt.datetime.now().strftime('%H:%M:%S, %d.%m.%Y'))
    # Set x-axis and y-axis to 20 values
    xs = xs[-20:]
    ys = ys[-20:]
    ymax = max(ys)
    ymin = min(ys)
    plt.axis([xs[0], xs[len(xs) - 1], ymin, ymax])
    # Plot settings
    ax.clear()
    ax.plot(xs, ys, 'b')
    plt.xticks(rotation=45, ha='right')
    plt.subplots_adjust(bottom=0.30)
    plt.ylim(bottom=0)
    plt.title('Power over time')
    plt.ylabel('Power [W]')