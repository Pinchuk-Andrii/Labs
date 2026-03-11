import matplotlib
matplotlib.use('Qt5Agg') # використовую стабільний движок Qt5
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button, CheckButtons
from scipy.signal import butter, lfilter

# Завдання 1. Функція згідно з вимогами (6 параметрів)
def harmonic_with_noise(t, amplitude, frequency, phase, noise_mean, noise_covariance, show_noise):
    harmonic = amplitude * np.sin(2 * np.pi * frequency * t + phase)
    if not show_noise:
        return harmonic, harmonic
    # Шум генерується окремо, тут просто опис логіки
    noise = np.random.normal(noise_mean, np.sqrt(noise_covariance), len(t))
    return harmonic + noise, harmonic

# Завдання 7. Фільтрація сигналу
def apply_filter(data, cutoff, fs=100):
    nyq = 0.5 * fs
    b, a = butter(4, cutoff / nyq, btype='low')
    return lfilter(b, a, data)

# Завдання 3. Початкові параметри
t = np.linspace(0, 10, 1000)
init_amp, init_freq, init_phase = 1.0, 0.5, 0.0
init_mean, init_cov, init_cut = 0.0, 0.1, 2.0

# Завдання 6. Збереження шуму в пам'яті
current_noise = np.random.normal(init_mean, np.sqrt(init_cov), 1000)
last_noise_params = [init_mean, init_cov]

# Завдання 2. Створення інтерфейсу
fig, ax = plt.subplots(figsize=(10, 7))
plt.subplots_adjust(bottom=0.35)

y_clean = init_amp * np.sin(2 * np.pi * init_freq * t + init_phase)
ln_noisy, = ax.plot(t, y_clean + current_noise, color='orange', alpha=0.5, label='Noisy')
ln_filt, = ax.plot(t, apply_filter(y_clean + current_noise, init_cut), 'r', lw=2, label='Filtered')
ln_clean, = ax.plot(t, y_clean, 'b--', alpha=0.7, label='Clean')
ax.legend()
ax.set_ylim(-5, 5)

# Слайдери для параметрів гармоніки та шуму
s_a = Slider(plt.axes([0.15, 0.22, 0.25, 0.03]), 'Amplitude', 0.1, 3.0, valinit=init_amp)
s_f = Slider(plt.axes([0.15, 0.18, 0.25, 0.03]), 'Frequency', 0.1, 5.0, valinit=init_freq)
s_p = Slider(plt.axes([0.15, 0.14, 0.25, 0.03]), 'Phase', 0, 2*np.pi, valinit=init_phase)
s_m = Slider(plt.axes([0.6, 0.22, 0.25, 0.03]), 'Noise Mean', -1, 1, valinit=init_mean)
s_c = Slider(plt.axes([0.6, 0.18, 0.25, 0.03]), 'Noise Cov', 0, 1, valinit=init_cov)
s_cut = Slider(plt.axes([0.6, 0.14, 0.25, 0.03]), 'Cutoff', 0.1, 10, valinit=init_cut)

# Елементи керування (завдання 2,4,6)
check = CheckButtons(plt.axes([0.15, 0.05, 0.1, 0.05]), ['Show Noise'], [True])
btn_reset = Button(plt.axes([0.8, 0.05, 0.1, 0.04]), 'Reset')

# Завдання 5. Оновлення графіка
def update(val):
    global current_noise, last_noise_params
    
    # Завдання 6. Якщо змінено параметри шуму генеруємо заново, інакше лишаємо
    if [s_m.val, s_c.val] != last_noise_params:
        current_noise = np.random.normal(s_m.val, np.sqrt(s_c.val), 1000)
        last_noise_params = [s_m.val, s_c.val]
    
    # Розрахунок чистої гармоніки
    h = s_a.val * np.sin(2 * np.pi * s_f.val * t + s_p.val)
    show = check.get_status()[0]
    
    # Завдання 4 та 9. Відображення результатів
    ln_noisy.set_ydata(h + current_noise)
    ln_noisy.set_visible(show)
    ln_filt.set_ydata(apply_filter(h + current_noise, s_cut.val))
    ln_filt.set_visible(show)
    ln_clean.set_ydata(h)
    fig.canvas.draw_idle()

# Завдання 6. Логіка Reset
def reset(event):
    s_a.reset(); s_f.reset(); s_p.reset()
    s_m.reset(); s_c.reset(); s_cut.reset()

for s in [s_a, s_f, s_p, s_m, s_c, s_cut]: s.on_changed(update)
check.on_clicked(update)
btn_reset.on_clicked(reset)

# Завдання 10. Інструкції
print("\nІНСТРУКЦІЯ КОРИСТУВАЧА:")
print("1.Використовуйте повзунки зліва для налаштування хвилі.")
print("2.Повзунки справа керують інтенсивністю шуму та його очищенням.")
print("3.Кнопка Reset миттєво повертає графік до вихідного стану.")
print("4.Чекбокс дозволяє порівняти чистий сигнал із зашумленим.\n")

plt.show()