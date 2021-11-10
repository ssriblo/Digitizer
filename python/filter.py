# https://stackoverflow.com/questions/50247517/python-specific-frequency-removenotch-filter/63038706#63038706

# https://stackoverflow.com/questions/25191620/creating-lowpass-filter-in-scipy-understanding-methods-and-units

from scipy import signal
import matplotlib.pyplot as plt
import numpy as np
from scipy.signal import butter, lfilter, freqz
from scipy.fftpack import fft, ifft, fftshift, rfft, fftfreq
import scipy

def butter_lowpass(cutoff, fs, order=5):
    nyq = 0.5 * fs
    normal_cutoff = cutoff / nyq
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    return b, a

def butter_lowpass_filter(data, cutoff, fs, order=5):
    b, a = butter_lowpass(cutoff, fs, order=order)
    y = lfilter(b, a, data)
    return y

def lp(data, sample_us, size, PRINTSOURCE):
    samp_freq = 1000_000/sample_us  # Sample frequency (Hz)
#    t = np.linspace(0.0, 1, size)
    T = (sample_us * size)/1000_000         # seconds
    print(f'Time aquire window={T}')
    print(f'size={size}')
    print(f'sample_us={sample_us}')
    t = np.linspace(0, T, size, endpoint=False)

    
#    data = np.sin(1500.0*2*np.pi*t) + 1.5*np.cos(4000.0*2*np.pi*t) + 0.5*np.sin(5000.0*2*np.pi*t)

    # Filter requirements.
    order = 5
    fs = samp_freq       # sample rate, Hz
    cutoff = 1000.0  # desired cutoff frequency of the filter, Hz
    print(f'fs={fs}')
    # Get the filter coefficients so we can check its frequency response.
    b, a = butter_lowpass(cutoff, fs, order)
    
    # Plot the frequency response.
    w, h = freqz(b, a, worN=8000)
#    plt.subplot(2, 1, 1)
#    plt.plot(0.5*fs*w/np.pi, np.abs(h), 'b')
#    plt.plot(cutoff, 0.5*np.sqrt(2), 'ko')
#    plt.axvline(cutoff, color='k')
#    plt.xlim(0, 0.1*fs)
#    plt.title("Lowpass Filter Frequency Response")
#    plt.xlabel('Frequency [Hz]')
#    plt.grid()

    data_detrended_const = signal.detrend(data, type='constant')
    data_detrended_linear = signal.detrend(data, type='linear')
    data_pure = data_detrended_const
#    data_pure = data_detrended_linear

    fourier = np.fft.fft(data_pure)
    n = size
    freq = np.fft.fftfreq(n, d=sample_us/1000)

    plt.subplot(2, 1, 1)
    plt.plot(freq, np.abs(fourier))
    plt.xlim(0, 2)
    plt.xlabel('Frequency [KHz]')
    plt.grid()


    data_filtered = butter_lowpass_filter(data_pure, cutoff, fs, order)

    plt.subplot(2, 1, 2)
    if(PRINTSOURCE):
        plt.plot(t, data_pure, 'b-', label='data')
    plt.plot(t, data_filtered, 'g-', linewidth=1, label='filtered data')
    plt.xlabel('Time [sec]')
    plt.grid()
    plt.legend(loc='upper right')

    plt.subplots_adjust(hspace=0.35)

