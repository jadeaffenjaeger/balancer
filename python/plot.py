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
    limit = 200

    fig = plt.figure(figsize=(10,8))
    plt.tight_layout()

    ax = plt.subplot(311)
    line1a, = ax.plot([0,]*len(data))
    # line1b, = ax.plot([0,]*len(data))
    ax.set_ylim(-limit, limit)

    ax = plt.subplot(312)
    line2a, = ax.plot([0,]*len(data))
    # line2b, = ax.plot([0,]*len(data))
    ax.set_ylim(-limit, limit)

    ax = plt.subplot(313)
    line3a, = ax.plot([0,]*len(data))
    # line3b, = ax.plot([0,]*len(data))
    ax.set_ylim(-limit, limit)

    def update(data):

        line1a.set_ydata([d[0] for d in data])
        # line1b.set_ydata([d[3] for d in data])
        line2a.set_ydata([d[1] for d in data])
        # line2b.set_ydata([d[4] for d in data])
        line3a.set_ydata([d[2] for d in data])
        # line3b.set_ydata([d[5] for d in data])
        # return [line1a, line1b, line2a, line2b, line3a, line3b]
        return [line1a, line2a, line3a]

    ani = animation.FuncAnimation(fig, update, data_gen, interval=0, blit=True)
    plt.show()


if __name__ == '__main__':
    main()
