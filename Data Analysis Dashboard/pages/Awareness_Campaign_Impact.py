import streamlit as st
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np

st.set_page_config(layout="wide")

st.markdown("Dashboard Created by Swaroop Deshmukh")
st.title("♻️ City-wise Recycling & Awareness Campaign Insights")

df = pd.read_excel("final_DAWaste_Management_and_Recycling_India.xlsx")
required_columns = ['AwarenessCampaignsCount', 'RecyclingRate', 'city']
if not all(col in df.columns for col in required_columns):
    st.error(f"Excel file must contain columns: {required_columns}")
    st.stop()

df = df[required_columns].dropna()

st.sidebar.header("🔍 Filters")

rate_bin_width = st.sidebar.slider("Recycling Rate Bin Width", min_value=1.0, max_value=20.0, value=5.0, step=1.0)
campaign_bin_width = st.sidebar.slider("Awareness Campaign Bin Width", min_value=1, max_value=20, value=5, step=1)

rate_min = df['RecyclingRate'].min()
rate_max = df['RecyclingRate'].max()
rate_bins = np.arange(np.floor(rate_min), np.ceil(rate_max) + rate_bin_width, rate_bin_width)
rate_labels = [f"{round(b, 2)}–{round(b + rate_bin_width, 2)}" for b in rate_bins[:-1]]
df['RecyclingRate_bin'] = pd.cut(df['RecyclingRate'], bins=rate_bins, labels=rate_labels, include_lowest=True)

campaign_min = df['AwarenessCampaignsCount'].min()
campaign_max = df['AwarenessCampaignsCount'].max()
campaign_bins = np.arange(np.floor(campaign_min), np.ceil(campaign_max) + campaign_bin_width, campaign_bin_width)
campaign_labels = [f"{int(b)}–{int(b + campaign_bin_width)}" for b in campaign_bins[:-1]]
df['Campaigns_bin'] = pd.cut(df['AwarenessCampaignsCount'], bins=campaign_bins, labels=campaign_labels, include_lowest=True)

selected_rate_bins = st.sidebar.multiselect("Recycling Rate Groups", options=rate_labels, default=rate_labels)
selected_campaign_bins = st.sidebar.multiselect("Awareness Campaign Groups", options=campaign_labels, default=campaign_labels)
selected_cities = st.sidebar.multiselect("Select Cities", options=sorted(df['city'].unique()), default=sorted(df['city'].unique()))

filtered_df = df[
    (df['RecyclingRate_bin'].isin(selected_rate_bins)) &
    (df['Campaigns_bin'].isin(selected_campaign_bins)) &
    (df['city'].isin(selected_cities))
]

if filtered_df.empty:
    st.warning("No data matches the selected filters.")
else:
    grouped = filtered_df.groupby('city').agg({
        'RecyclingRate': 'mean',
        'AwarenessCampaignsCount': 'mean'
    }).reset_index()

    st.subheader("📈 Awareness Count vs Recycling Rate")
    fig_top, ax_top = plt.subplots(figsize=(10, 6))
    barplot_top = sns.barplot(data=grouped, x='AwarenessCampaignsCount', y='RecyclingRate', palette='coolwarm', ax=ax_top)
    ax_top.set_xlabel("Avg Awareness Campaigns Count")
    ax_top.set_ylabel("Avg Recycling Rate")
    ax_top.tick_params(axis='x', rotation=45, labelsize=8)

    for container in ax_top.containers:
        ax_top.bar_label(container, fmt="%.1f")
    for bar, city in zip(ax_top.patches, grouped['city']):
            height = bar.get_height()
            ax_top.text(
                bar.get_x() + bar.get_width() / 2,  
                0.05 * height,                      
                city,
                ha='center',
                va='bottom',
                fontsize=8,
                color='white',
                rotation=90,
                weight='bold'
            )
    st.pyplot(fig_top)

    # Existing Two Charts
    col1, col2 = st.columns(2)

    with col1:
        st.subheader("📊 Average Recycling Rate by City")
        fig1, ax1 = plt.subplots(figsize=(8, 5))
        barplot1 = sns.barplot(data=grouped, x='city', y='RecyclingRate', palette='viridis', ax=ax1)
        ax1.set_xlabel("City")
        ax1.set_ylabel("Avg Recycling Rate")
        ax1.set_xticklabels([])
        for container in ax1.containers:
            ax1.bar_label(container, fmt="%.1f")
        for bar, city in zip(ax1.patches, grouped['city']):
            height = bar.get_height()
            ax1.text(
                bar.get_x() + bar.get_width() / 2, 
                0.05 * height,                      
                city,
                ha='center',
                va='bottom',
                fontsize=8,
                color='white',
                rotation=90,
                weight='bold'
            )
        st.pyplot(fig1)

    with col2:
        st.subheader("📢 Average Awareness Campaigns by City")
        fig2, ax2 = plt.subplots(figsize=(8, 5))
        barplot2 = sns.barplot(data=grouped, x='city', y='AwarenessCampaignsCount', palette='magma', ax=ax2)
        ax2.set_xlabel("City")
        ax2.set_ylabel("Avg Campaigns Count")
        ax2.set_xticklabels([])
        for container in ax2.containers:
            ax2.bar_label(container, fmt="%.1f")
        for bar, city in zip(ax2.patches, grouped['city']):
            height = bar.get_height()
            ax2.text(
                bar.get_x() + bar.get_width() / 2,
                0.05 * height,
                city,
                ha='center',
                va='bottom',
                fontsize=8,
                color='white',
                rotation=90,
                weight='bold'
            )
        st.pyplot(fig2)

st.sidebar.markdown("""
### About This Dashboard
This interactive dashboard evaluates the relationship between public awareness efforts and recycling outcomes in Indian cities.

**Goal:**
Investigate if cities with more awareness campaigns have higher recycling rates and better disposal practices.

**Features:**
- Visualization of campaign count vs. recycling rate
- Cluster analysis of campaign effectiveness
- Comparative insights across regions

""")
