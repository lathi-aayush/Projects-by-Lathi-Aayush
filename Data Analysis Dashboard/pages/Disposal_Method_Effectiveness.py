import streamlit as st
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np

# App configuration
st.set_page_config(page_title="Disposal Method vs MES", layout="wide")
st.markdown("Dashboard Created by Lathi Aayush")
st.title("🚮 Disposal Method Effectiveness vs. Municipal Efficiency")
st.markdown("Identify which disposal methods (e.g., Composting, Recycling, Landfill) are most effectively used in cities with high municipal efficiency scores.")

# Style
sns.set_style("darkgrid")

# Load data
df = pd.read_excel("final_DAWaste_Management_and_Recycling_India.xlsx")

# Preprocess
avg_mes = df.groupby('city')['MES'].mean().reset_index()
method_counts = df.groupby(['city', 'disposal_method']).size().reset_index(name='count')
total_counts = df.groupby('city').size().reset_index(name='total')
method_stats = pd.merge(method_counts, total_counts, on='city')
method_stats['proportion'] = method_stats['count'] / method_stats['total']
final_df = pd.merge(method_stats, avg_mes, on='city')

# Sidebar filters
st.sidebar.header("🔍 Filters")
bin_width = st.sidebar.slider("MES Bin Width", min_value=0.1, max_value=2.0, value=0.5, step=0.1)

min_mes = avg_mes['MES'].min()
max_mes = avg_mes['MES'].max()
bins = np.arange(np.floor(min_mes), np.ceil(max_mes) + bin_width, bin_width)
labels = [f"{round(b,2)}-{round(b+bin_width,2)}" for b in bins[:-1]]

avg_mes['MES_bin'] = pd.cut(avg_mes['MES'], bins=bins, labels=labels, include_lowest=True)
final_df['MES_bin'] = pd.cut(final_df['MES'], bins=bins, labels=labels, include_lowest=True)

selected_bins = st.sidebar.multiselect("MES Bin Groups", options=labels, default=labels)
selected_cities = st.sidebar.multiselect("Cities", options=sorted(df['city'].unique()), default=sorted(df['city'].unique()))
method_selected = st.sidebar.selectbox("Disposal Method", final_df['disposal_method'].unique())

# Filtered data
filtered_avg_mes = avg_mes[avg_mes['MES_bin'].isin(selected_bins) & avg_mes['city'].isin(selected_cities)]
filtered_final_df = final_df[final_df['MES_bin'].isin(selected_bins) & final_df['city'].isin(selected_cities)]
filtered = filtered_final_df[filtered_final_df['disposal_method'] == method_selected]

# Layout columns
col1, col2 = st.columns(2)

# Modified Bar Plot with unique colors per city
with col1:
    fig2, ax2 = plt.subplots(figsize=(8, 6))
    fig2.patch.set_facecolor('#121212')
    ax2.set_facecolor('#121212')

    # Assign a unique color for each city
    unique_cities = filtered_avg_mes['city'].unique()
    city_palette = dict(zip(unique_cities, sns.color_palette("husl", len(unique_cities))))

    barplot = sns.barplot(data=filtered_avg_mes, x='city', y='MES', palette=city_palette, ax=ax2)

    for i, bar in enumerate(barplot.patches):
        height = bar.get_height()
        x = bar.get_x() + bar.get_width() / 2
        ax2.text(x, height * 0.05, filtered_avg_mes['city'].iloc[i], ha='center', va='bottom', color='black', fontsize=9, rotation=90)
        ax2.text(x, height + 0.1, f"{height:.2f}", ha='center', va='bottom', color='white', fontsize=10)

    ax2.set_title("Average MES per City", color="white", fontsize=14)
    ax2.set_xlabel("City", color="white")
    ax2.set_ylabel("Average MES", color="white")
    plt.xticks([], color="white")
    plt.yticks(color="white")
    ax2.grid(True, color='#1a1a1a')
    ax2.spines['top'].set_visible(False)
    ax2.spines['right'].set_visible(False)
    ax2.spines['left'].set_color('white')
    ax2.spines['bottom'].set_color('white')
    st.pyplot(fig2)

# Scatter Plot
with col2:
    fig1, ax1 = plt.subplots(figsize=(8, 6))
    fig1.patch.set_facecolor('#121212')
    ax1.set_facecolor('#121212')
    neon_green = "#0aff99"

    sns.scatterplot(data=filtered, x='MES', y='proportion', s=150, ax=ax1,
                    color=neon_green, edgecolor='white', linewidth=0.8, alpha=0.9)

    ax1.set_title(f"'{method_selected}' vs MES", color="white", fontsize=14)
    ax1.set_xlabel("MES", color="white")
    ax1.set_ylabel("Proportion Used", color="white")
    plt.xticks(color="white")
    plt.yticks(color="white")
    ax1.grid(True, color='#1a1a1a')
    ax1.spines['top'].set_visible(False)
    ax1.spines['right'].set_visible(False)
    ax1.spines['left'].set_color('white')
    ax1.spines['bottom'].set_color('white')
    st.pyplot(fig1)

# Data Table at the bottom
st.markdown("---")

with st.expander("📊 Show Data Table: City-wise MES by Disposal Method"):
    agg_df = df.groupby(['city', 'disposal_method'])['MES'].mean().reset_index()
    pivot_table = agg_df.pivot(index="city", columns="disposal_method", values="MES").round(2)
    st.dataframe(pivot_table)

# About sidebar
st.sidebar.markdown("""
### About This Dashboard
This interactive dashboard explores how effectively different waste disposal methods are utilized in cities with varying levels of municipal efficiency.

**Goal:**
Identify which disposal methods (e.g., Composting, Recycling, Landfill) are most effectively used in cities with high municipal efficiency scores.

**Features:**
- Correlation analysis between disposal methods and municipal efficiency
- Comparative visuals across cities
- Filtering by disposal method or efficiency tier
- Highlights best practices in urban waste management
""")
