import streamlit as st
import pandas as pd
import matplotlib.pyplot as plt

st.set_page_config(page_title="Cost vs Recycling Rate", layout="wide")

st.markdown("Dashboard Created by Ayush Dewangan")
st.title("Comparing Costs")
st.markdown("Explore how different city compare in terms of average waste management cost per city")

df_raw = pd.read_excel("final_DAWaste_Management_and_Recycling_India.xlsx")

all_waste_types = sorted(df_raw["WasteType"].dropna().unique())
all_cities = sorted(df_raw["city"].dropna().unique())

st.sidebar.header("🔍 Filters")
selected_waste_types = st.sidebar.multiselect("Select Waste Types", options=all_waste_types, default=all_waste_types)
selected_cities = st.sidebar.multiselect("Select Cities", options=all_cities, default=all_cities)

df_filtered = df_raw[
    df_raw["WasteType"].isin(selected_waste_types) &
    df_raw["city"].isin(selected_cities)
]

df_city_cost = df_filtered.groupby("city").agg({
    "CostofWasteManagement": "mean"
}).reset_index().rename(columns={"city": "City", "CostofWasteManagement": "Average Cost"})

st.subheader("Average Cost of Waste Management per City")

fig1, ax1 = plt.subplots(figsize=(12, 6))
bars1 = ax1.bar(df_city_cost["City"], df_city_cost["Average Cost"], color='lightgreen')
ax1.set_xlabel("City")
ax1.set_ylabel("Average Cost (in Lakhs ₹)")
ax1.set_title("Waste Management Cost per City")
plt.xticks([])  

for bar, city in zip(bars1, df_city_cost["City"]):
    height = bar.get_height()
    ax1.text(
        bar.get_x() + bar.get_width() / 2,
        0.05 * height,
        city,
        ha='center',
        va='bottom',
        fontsize=8,
        color='black',
        rotation=90,
        weight='bold'
    )

for bar in bars1:
    height = bar.get_height()
    ax1.text(
        bar.get_x() + bar.get_width() / 2,
        height + 0.5,
        f"{height:.1f}",
        ha='center',
        va='bottom',
        fontsize=9,
        color='black'
    )

st.pyplot(fig1)

df_grouped = df_filtered.groupby("WasteType").agg({
    "WasteGenerated": "sum",
    "CostofWasteManagement": "mean",
    "RecyclingRate": "mean"
}).reset_index()

st.subheader("Recycling Rate by Waste Type across multiple cities")

fig2, ax2 = plt.subplots(figsize=(12, 6))
bars2 = ax2.bar(df_grouped["WasteType"], df_grouped["RecyclingRate"], color='green')
ax2.set_xlabel("Waste Type")
ax2.set_ylabel("Recycling Rate (%)")
ax2.set_title("Recycling Rate Across Waste Types")
plt.xticks(rotation=45)

for bar, rate in zip(bars2, df_grouped["RecyclingRate"]):
    height = bar.get_height()
    ax2.text(bar.get_x() + bar.get_width()/2, height + 0.5, f"{rate:.1f}%", ha='center', va='bottom', fontsize=9)

st.pyplot(fig2)
