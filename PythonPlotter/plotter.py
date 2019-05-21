import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


if __name__ == '__main__':
    df = pd.read_csv('guardar.csv')
    for k in range(2):
        plt.figure()
        for i in range(4 * k, 4 * (k + 1)):
            mask = df['cluster'].values == i
            x_vals = df['x'].values[mask]
            y_vals = df['y'].values[mask]
            plt.plot(x_vals, y_vals, 'o')
        plt.grid()
        plt.title(str(k))
    plt.figure()
    for i in range(0, 4):
        mask = df['cluster'].values == i
        x_vals = df['x'].values[mask]
        y_vals = df['y'].values[mask]
        plt.plot(x_vals, y_vals, 'o')
    for i in range(4, 8):
        mask = df['cluster'].values == i
        x_vals = df['x'].values[mask]
        y_vals = df['y'].values[mask]
        plt.plot(x_vals, y_vals, '*')
    plt.grid()
    plt.title("Todos")
    plt.show()


    
    
