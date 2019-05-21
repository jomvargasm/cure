import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


if __name__ == '__main__':
    df = pd.read_csv('guardar.csv')
    for k in range(3):
        plt.figure()
        for i in range(5 * k, 5 * (k + 1)):
            mask = df['cluster'].values == i
            x_vals = df['x'].values[mask]
            y_vals = df['y'].values[mask]
            plt.plot(x_vals, y_vals, 'o')
        plt.grid()
        plt.title(str(k))
    plt.figure()
    for i in range(0, 15):
        mask = df['cluster'].values == i
        x_vals = df['x'].values[mask]
        y_vals = df['y'].values[mask]
        plt.plot(x_vals, y_vals, 'o')
    plt.grid()
    plt.title("Todos")
    plt.show()


    
    
