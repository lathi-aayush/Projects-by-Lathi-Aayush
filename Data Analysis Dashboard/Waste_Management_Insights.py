import streamlit as st
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np

st.set_page_config(layout="wide")

st.markdown("Dashboard Created by CS-D Group 13")
st.title("♻️ Dynamic Dashboard: Compare Any Columns")

df = pd.read_excel("final_DAWaste_Management_and_Recycling_India.xlsx")

required_columns = ['AwarenessCampaignsCount', 'RecyclingRate', 'city']
if not all(col in df.columns for col in required_columns):
    st.error(f"Excel file must contain columns: {required_columns}")
    st.stop()

df = df.dropna()

st.sidebar.header("🔍 Filters")

x_options = df.columns.tolist()
y_options = [col for col in df.columns if pd.api.types.is_numeric_dtype(df[col])]

x_axis = st.sidebar.selectbox("Select X-axis (categorical or continuous)", options=x_options, index=x_options.index('city'))
y_axis = st.sidebar.selectbox("Select Y-axis (numeric)", options=y_options, index=y_options.index('RecyclingRate'))

filtered_df = df[[x_axis, y_axis]].dropna()

if pd.api.types.is_numeric_dtype(filtered_df[x_axis]):
    grouped = filtered_df.groupby(x_axis)[y_axis].mean().reset_index()
    x_vals = grouped[x_axis]
    y_vals = grouped[y_axis]
else:
    grouped = filtered_df.groupby(x_axis)[y_axis].mean().reset_index()
    x_vals = grouped[x_axis].astype(str)
    y_vals = grouped[y_axis]

fig, ax = plt.subplots(figsize=(12, 6))
sns.barplot(x=x_vals, y=y_vals, palette='viridis', ax=ax)

ax.set_xlabel(x_axis)
ax.set_ylabel(f"Average {y_axis}")
ax.set_title(f"Average {y_axis} by {x_axis}")

if len(x_vals) > 5:
    ax.tick_params(axis='x', rotation=90)

for container in ax.containers:
    ax.bar_label(container, fmt="%.2f", padding=3, label_type='edge')

st.pyplot(fig)

with st.expander("Show Data Used for Plot"):
    st.dataframe(grouped)
