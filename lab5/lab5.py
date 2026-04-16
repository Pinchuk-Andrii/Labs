import streamlit as st
import pandas as pd
import matplotlib.pyplot as plt
import os
import io

st.set_page_config(layout="wide")
st.title("Лабораторна робота №5: Аналіз індексів VCI, TCI, VHI")

# Завантаження даних
@st.cache_data
def load_all_data():
    path = "lab5/data/" 
    if not os.path.exists(path): return pd.DataFrame()
    files = [f for f in os.listdir(path) if f.endswith('.csv')]
    all_df = []
    for f in files:
        try:
            with open(os.path.join(path, f), 'r', encoding='utf-8', errors='ignore') as file:
                lines = file.readlines()
            # Чистимо заголовок
            header = lines[1].replace('<br>', '').strip()
            # Чистимо дані
            body = [line.replace('<tt><pre>', '').strip() for line in lines[2:]]
            
            csv_text = header + '\n' + '\n'.join(body)
            # index_col=False допомагає, якщо в кінці рядків є зайві коми
            df = pd.read_csv(io.StringIO(csv_text), index_col=False)
            
            df.columns = [c.strip() for c in df.columns]
            df = df.rename(columns={'year': 'Year', 'week': 'Week'})
            df['Province'] = int(f.split('_')[2])
            
            # Перетворюємо на числа тільки ті колонки, що нам потрібні
            cols = ['Year', 'Week', 'VCI', 'TCI', 'VHI', 'Province']
            df = df[cols].apply(pd.to_numeric, errors='coerce')
            all_df.append(df.dropna(subset=['Year']))
        except: continue
    return pd.concat(all_df, ignore_index=True) if all_df else pd.DataFrame()

data = load_all_data()

# Інтерфейс
col_side, col_main = st.columns([1, 3])

with col_side:
    st.header("Фільтри")
    if data.empty:
        st.error("Дані не знайдено!")
    else:
        series = st.selectbox("Показник:", ["VCI", "TCI", "VHI"])
        # Створюємо список областей як цілі числа
        area = st.selectbox("Область №:", sorted(data.Province.unique().astype(int)))
        weeks = st.slider("Тижні:", 1, 52, (1, 52))
        
        # Робимо початковий інтервал ширшим, щоб таблиця не була порожньою
        min_y, max_y = int(data.Year.min()), int(data.Year.max())
        years = st.slider("Роки:", min_y, max_y, (min_y, max_y))
        
        up = st.checkbox("За зростанням")
        down = st.checkbox("За спаданням")
        if st.button("Скинути фільтри"):
            st.cache_data.clear()
            st.rerun()

# Відображення
if not data.empty:
    # Фільтрація
    df_filtered = data[
        (data.Province == area) & 
        (data.Year.between(years[0], years[1])) & 
        (data.Week.between(weeks[0], weeks[1]))
    ]
    
    if up and not down:
        df_filtered = df_filtered.sort_values(by=series)
    elif down and not up:
        df_filtered = df_filtered.sort_values(by=series, ascending=False)

    with col_main:
        t1, t2, t3 = st.tabs(["Таблиця", "Графік", "Порівняння"])
        
        with t1:
            st.subheader(f"Результати для області №{area}")
            # Перевірка на порожнечу
            if not df_filtered.empty:
                st.dataframe(df_filtered, use_container_width=True)
            else:
                st.warning("За вашим запитом даних не знайдено. Спробуйте змінити роки.")
            
        with t2:
            if not df_filtered.empty:
                fig, ax = plt.subplots(figsize=(10, 4))
                ax.plot(df_filtered.Year + df_filtered.Week/52, df_filtered[series])
                ax.set_ylabel(series)
                st.pyplot(fig)
            
        with t3:
            fig2, ax2 = plt.subplots(figsize=(10, 4))
            for p in data.Province.unique():
                sub = data[(data.Province == p) & (data.Year.between(years[0], years[1]))]
                # Малюємо обрану область червоним, інші - сірим
                color = 'red' if p == area else 'lightgrey'
                alpha = 1.0 if p == area else 0.2
                ax2.plot(sub.Year + sub.Week/52, sub[series], color=color, alpha=alpha)
            st.pyplot(fig2)