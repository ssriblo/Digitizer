# https://stackoverflow.com/questions/50247517/python-specific-frequency-removenotch-filter/63038706#63038706

# https://stackoverflow.com/questions/25191620/creating-lowpass-filter-in-scipy-understanding-methods-and-units

from scipy import signal
import matplotlib.pyplot as plt
import numpy as np
from scipy.signal import butter, lfilter, freqz

def butter_lowpass(cutoff, fs, order=5):
    nyq = 0.5 * fs
    normal_cutoff = cutoff / nyq
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    return b, a

def butter_lowpass_filter(data, cutoff, fs, order=5):
    b, a = butter_lowpass(cutoff, fs, order=order)
    y = lfilter(b, a, data)
    return y

def lp(data, sample_us, size):
    samp_freq = 1000_000/sample_us  # Sample frequency (Hz)
#    t = np.linspace(0.0, 1, size)
    T = (sample_us * size)/1000_000         # seconds
    t = np.linspace(0, T, size, endpoint=False)

    
#    data = np.sin(1500.0*2*np.pi*t) + 1.5*np.cos(4000.0*2*np.pi*t) + 0.5*np.sin(5000.0*2*np.pi*t)

    # Filter requirements.
    order = 2
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

    data_filtered = butter_lowpass_filter(data, cutoff, fs, order)

    plt.subplot(2, 1, 2)
    plt.plot(t, data, 'b-', label='data')
    plt.plot(t, data_filtered, 'g-', linewidth=2, label='filtered data')
    plt.ylim(ymin=0.003)  # this line
    plt.xlabel('Time [sec]')
    plt.grid()
    plt.legend()

    plt.subplots_adjust(hspace=0.35)


#    plt.figure('result')    plt.subplot(211)
#    plt.plot(t, data, color = 'r')
#    plt.subplot(212)
#    plt.plot(t, data_filtered, color = 'r')

def notch(data, sample_us, size):
    # Create/view notch filter
#    samp_freq = 1000  # Sample frequency (Hz)
    samp_freq = 1000_000/sample_us  # Sample frequency (Hz)
#    notch_freq = 60.0  # Frequency to be removed from signal (Hz)
    notch_freq = 3950.0  # Frequency to be removed from signal (Hz)
    quality_factor = 15.0  # Quality factor
    b_notch, a_notch = signal.iirnotch(notch_freq, quality_factor, samp_freq)
    freq, h = signal.freqz(b_notch, a_notch, fs = samp_freq)
#    plt.figure('filter')
#    plt.plot( freq, 20*np.log10(abs(h)))

    # Create/view signal that is a mixture of two frequencies
    f1 = 50
    f2 = 3700
#    t = np.linspace(0.0, 1, 1_000)
    t = np.linspace(0.0, 1, size)
#    y_pure = np.sin(f1 * 2.0*np.pi*t) + np.sin(f2 * 2.0*np.pi*t) 
    y_pure =  np.sin(f2 * 2.0*np.pi*t) 
    data_pure = data
    plt.figure('result')
    plt.subplot(211)
#    plt.plot(t, y_pure, color = 'r')
    plt.plot(t, data_pure, color = 'r')

    # apply notch filter to signal
#    y_notched = signal.filtfilt(b_notch, a_notch, y_pure)
    data_notched = signal.filtfilt(b_notch, a_notch, data_pure)

    # plot notch-filtered version of signal
    plt.subplot(212)
#    plt.plot(t, y_notched, color = 'r')
    plt.plot(t, data_notched, color = 'r')
    #plt.show()
