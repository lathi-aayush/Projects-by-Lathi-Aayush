import streamlit as st
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

st.set_page_config(page_title="Waste Disposal Trends vs. Population Density", layout="wide")

st.markdown("Dashboard Created by Purvika Agrawal")
st.sidebar.header("🔍 Filters")
show_all_cities = st.sidebar.checkbox("Show All Cities", value=True)

df_raw = pd.read_excel("final_DAWaste_Management_and_Recycling_India.xlsx")

df_grouped = df_raw.groupby('city').agg({
    'PopulationDensity': 'mean',
    'LandfillCapacity': 'mean'
}).reset_index().rename(columns={"city": "City"})

if not show_all_cities:
    selected_cities = st.sidebar.multiselect("Select Cities", options=sorted(df_grouped["City"].unique()))
    df = df_grouped[df_grouped["City"].isin(selected_cities)]
else:
    df = df_grouped

st.title("Waste Disposal Trends vs. Population Density")
st.markdown("Analyze how urban population density influences landfill dependency.")

df_sorted = df.sort_values(by="PopulationDensity", ascending=False)

# Scatter plot: Landfill Capacity vs Population Density (shown first)
st.subheader("Landfill Capacity vs Population Density")
fig2, ax2 = plt.subplots(figsize=(10,6))
sns.scatterplot(data=df, x="PopulationDensity", y="LandfillCapacity", hue="City", s=100, ax=ax2)
ax2.set_xlabel("Population Density (people per sq km)")
ax2.set_ylabel("Landfill Capacity (in tons)")
ax2.set_title("Scatter Plot of Landfill Capacity vs Population Density")
plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
st.pyplot(fig2)

# Bar chart: City-wise Landfill Usage sorted by Population Density (shown second)
st.subheader("City-wise Landfill Usage Sorted by Population Density")
fig, ax = plt.subplots(figsize=(14, 7))
bars = ax.bar(df_sorted["City"], df_sorted["LandfillCapacity"], color='orange')
ax.set_xlabel("City")
ax.set_ylabel("Landfill Capacity (in tons)")
plt.xticks(rotation=90)
ax.set_title("City-wise Landfill Usage Sorted by Population Density")

for bar, pop_den in zip(bars, df_sorted["PopulationDensity"]):
    height = bar.get_height()
    ax.text(bar.get_x() + bar.get_width()/2, height + 1, f"{pop_den:.0f}", ha='center', va='bottom', fontsize=8, rotation=90)

st.pyplot(fig)

