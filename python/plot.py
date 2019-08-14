import serial
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import threading

plt.style.use('ggplot')

data = 100*[0]

def data_gen():
    with serial.Serial('/dev/ttyACM0') as ser:
        while True:
            items = ser.readline().decode('utf-8').rstrip().split(',')
            global data
            data = data[1:]
            data.append(float(items[0]))
            print(items[0])
            yield data

def main():
    fig, ax = plt.subplots()
    line, = ax.plot([0,]*len(data))
    ax.set_ylim(-1.5, 1.5)

    def update(data):
        line.set_ydata(data)
        return line,

    ani = animation.FuncAnimation(fig, update, data_gen, interval=0, blit=True)
    plt.show()
    # while True:
    #     pass


if __name__ == '__main__':
    main()
