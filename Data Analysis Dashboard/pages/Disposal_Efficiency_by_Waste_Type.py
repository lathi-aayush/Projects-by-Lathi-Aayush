import streamlit as st
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

sns.set_theme(style="whitegrid")
st.set_page_config(page_title="Waste Disposal Efficiency Dashboard", layout="wide")

st.markdown("Dashboard Created by Harsh Gupta")
st.title("♻️ Comparative Efficiency of Disposal Methods by Waste Type")
st.markdown("Analyze and rank disposal methods based on average MES to optimize waste management strategies.")

df = pd.read_excel("final_DAWaste_Management_and_Recycling_India.xlsx")
df = df[["WasteType", "MES", "disposal_method"]].dropna()

st.sidebar.header("Filters")
waste_types = st.sidebar.multiselect("Waste Types", df["WasteType"].unique(), default=df["WasteType"].unique())
methods = df["disposal_method"].unique().tolist()
selected_methods = st.sidebar.multiselect("Disposal Methods", methods, default=methods)

filtered_df = df[df["WasteType"].isin(waste_types) & df["disposal_method"].isin(selected_methods)]

if filtered_df.empty:
    st.warning("Please select at least one waste type and one disposal method.")
    st.stop()

agg_df = filtered_df.groupby(["WasteType", "disposal_method"])["MES"].mean().reset_index()

fig, ax = plt.subplots(figsize=(10, 6))
sns.barplot(data=agg_df, x="WasteType", y="MES", hue="disposal_method", palette="Set2", ax=ax)
ax.set_title("Average MES by Waste Type and Disposal Method", fontsize=16)
ax.set_ylabel("Average MES")
ax.set_xlabel("Waste Type")
ax.set_ylim(0, 10)
ax.legend(title="Disposal Method", loc='upper right')
ax.grid(axis='y')
st.pyplot(fig)

with st.expander("Show Data Table"):
    st.dataframe(agg_df.pivot(index="WasteType", columns="disposal_method", values="MES").round(2))

st.sidebar.markdown("""
### About This Dashboard
This dashboard compares the effectiveness of disposal methods across different types of waste to identify optimal strategies.

**Goal:**
Rank which disposal method works best for each waste type (e.g., Composting for Organic, Recycling for Plastic).

**Features:**
- Performance benchmarking for each method
- Waste-type specific disposal outcomes
- Insights into method suitability and efficiency

""")
