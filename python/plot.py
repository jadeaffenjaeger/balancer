import serial
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import threading

plt.style.use('ggplot')

data = 100*[7*[0]]

def data_gen():
    with serial.Serial('/dev/ttyACM0') as ser:
        while True:
            # Get data from serial port
            items = ser.readline().decode('utf-8').rstrip().split(',')

            # Cast to float explicitly
            items = [float(item) for item in items]
            global data
            data = data[1:]
            data.append(items)
            print(items)
            yield data

def main():
    fig = plt.figure(figsize=(10,8))
    plt.tight_layout()

    ax = plt.subplot(311)
    line1, = ax.plot([0,]*len(data))
    ax.set_ylim(-1.5, 1.5)

    ax = plt.subplot(312)
    line2, = ax.plot([0,]*len(data))
    ax.set_ylim(-1.5, 1.5)

    ax = plt.subplot(313)
    line3, = ax.plot([0,]*len(data))
    ax.set_ylim(-1.5, 1.5)

    def update(data):

        line1.set_ydata([d[0] for d in data])
        line2.set_ydata([d[1] for d in data])
        line3.set_ydata([d[2] for d in data])
        return [line1, line2, line3]

    ani = animation.FuncAnimation(fig, update, data_gen, interval=0, blit=True)
    plt.show()


if __name__ == '__main__':
    main()
