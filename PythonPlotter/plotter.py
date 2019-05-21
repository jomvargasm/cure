import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


if __name__ == '__main__':
    df = pd.read_csv('guardar.csv')
    n_clusters = 15
    n_sub_clusters = 3
    for k in range(n_sub_clusters):
        plt.figure()
        for i in range((n_clusters // n_sub_clusters) * k, (n_clusters // n_sub_clusters) * (k + 1)):
            mask = df['cluster'].values == i
            x_vals = df['x'].values[mask]
            y_vals = df['y'].values[mask]
            plt.plot(x_vals, y_vals, 'o')
        plt.grid()
        plt.title(str(k))
    plt.figure()
    for i in range(0, n_clusters // 2):
        mask = df['cluster'].values == i
        x_vals = df['x'].values[mask]
        y_vals = df['y'].values[mask]
        plt.plot(x_vals, y_vals, 'o')
    for i in range(n_clusters // 2, n_clusters):
        mask = df['cluster'].values == i
        x_vals = df['x'].values[mask]
        y_vals = df['y'].values[mask]
        plt.plot(x_vals, y_vals, '*')
    plt.grid()
    plt.title("Todos")
    plt.show()


    
    
