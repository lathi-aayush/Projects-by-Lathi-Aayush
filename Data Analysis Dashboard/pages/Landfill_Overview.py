import streamlit as st
import pandas as pd
import geopandas as gpd
import matplotlib.pyplot as plt
import plotly.express as px
from shapely.geometry import Point
import os
import urllib.request
import zipfile

# Set page configuration
st.set_page_config(page_title="India Waste Management Dashboard", layout="wide")

# Title
st.markdown("Dashboard Created by Ayush Shirode")
st.title("India Waste Management Dashboard")

# Sidebar - Filters
st.sidebar.header("🔍 Filters")

# Load data
@st.cache_data
def load_data():
    df = pd.read_excel("data.xlsx")
    df.columns = df.columns.str.strip()
    df = df.dropna(subset=['City/District', 'Year', 'Waste Type', 'Waste Generated (Tons/Day)', 'Population Density (People/kmÂ²)', 'Landfill Capacity (Tons)'])
    df['Year'] = df['Year'].astype(int)
    df['Waste Generated (Tons/Day)'] = pd.to_numeric(df['Waste Generated (Tons/Day)'], errors='coerce')
    df['Population Density (People/kmÂ²)'] = pd.to_numeric(df['Population Density (People/kmÂ²)'], errors='coerce')
    df['Landfill Capacity (Tons)'] = pd.to_numeric(df['Landfill Capacity (Tons)'], errors='coerce')
    return df

data = load_data()

# Extract unique values for filters
years = sorted(data['Year'].unique())
cities = sorted(data['City/District'].unique())
waste_types = sorted(data['Waste Type'].unique())

# Sidebar filters
selected_year = st.sidebar.selectbox("Select Year", years)
selected_cities = st.sidebar.multiselect("Select Cities (for comparison)", cities, default=["Mumbai"])
selected_waste_type = st.sidebar.selectbox("Select Waste Type", ["All"] + waste_types)
show_raw_data = st.sidebar.checkbox("Show Raw Data")

# Filter data based on selections
filtered_data = data.copy()
if selected_cities:
    filtered_data = filtered_data[filtered_data['City/District'].isin(selected_cities)]
if selected_waste_type != "All":
    filtered_data = filtered_data[filtered_data['Waste Type'] == selected_waste_type]
filtered_data = filtered_data[filtered_data['Year'] == selected_year]

# Load map data
@st.cache_data
def load_map_data():
    if not os.path.exists('naturalearth_lowres'):
        url = "https://naciscdn.org/naturalearth/110m/cultural/ne_110m_admin_0_countries.zip"
        zip_path = "countries.zip"
        urllib.request.urlretrieve(url, zip_path)
        with zipfile.ZipFile(zip_path, 'r') as zip_ref:
            zip_ref.extractall("naturalearth_lowres")
        os.remove(zip_path)
    world = gpd.read_file("naturalearth_lowres/ne_110m_admin_0_countries.shp")
    india = world[world['ADMIN'] == 'India']
    return india

india_map = load_map_data()

# City coordinates (for mapping)
city_coords = {
    'Mumbai': (19.0760, 72.8777),
    'Delhi': (28.7041, 77.1025),
    'Bengaluru': (12.9716, 77.5946),
    'Chennai': (13.0827, 80.2707),
    'Kolkata': (22.5726, 88.3639),
    'Hyderabad': (17.3850, 78.4867),
    'Pune': (18.5204, 73.8567),
    'Ahmedabad': (23.0225, 72.5714),
    'Jaipur': (26.9124, 75.7873),
    'Lucknow': (26.8467, 80.9462),
    'Surat': (21.1702, 72.8311),
    'Kanpur': (26.4499, 80.3319),
    'Nagpur': (21.1458, 79.0882),
    'Patna': (25.5941, 85.1376),
    'Bhopal': (23.2599, 77.4126),
    'Thiruvananthapuram': (8.5241, 76.9366),
    'Indore': (22.7196, 75.8577),
    'Vadodara': (22.3072, 73.1812),
    'Guwahati': (26.1445, 91.7362),
    'Coimbatore': (11.0168, 76.9558),
    'Ranchi': (23.3441, 85.3096),
    'Amritsar': (31.6340, 74.8723),
    'Jodhpur': (26.2389, 73.0243),
    'Varanasi': (25.3176, 82.9739),
    'Ludhiana': (30.9010, 75.8573),
    'Agra': (27.1767, 78.0081),
    'Meerut': (28.9845, 77.7064),
    'Nashik': (20.0059, 73.7910),
    'Rajkot': (22.3039, 70.8022),
    'Madurai': (9.9252, 78.1198),
    'Jabalpur': (23.1815, 79.9864),
    'Allahabad': (25.4358, 81.8463),
    'Visakhapatnam': (17.6868, 83.2185),
    'Gwalior': (26.2183, 78.1828)
}

# Add coordinates to data
def add_coordinates(df):
    df['Latitude'] = df['City/District'].map(lambda x: city_coords.get(x, (None, None))[0])
    df['Longitude'] = df['City/District'].map(lambda x: city_coords.get(x, (None, None))[1])
    return df

filtered_data = add_coordinates(filtered_data)

# National Overview (when no city is selected or show aggregate)
st.header("🌐 National Waste Generation Overview")

# Aggregate data for map
map_data = filtered_data.groupby('City/District').agg({
    'Waste Generated (Tons/Day)': 'sum',
    'Population Density (People/kmÂ²)': 'mean',
    'Landfill Capacity (Tons)': 'mean',
    'Latitude': 'first',
    'Longitude': 'first'
}).reset_index()

# Create GeoDataFrame
geometry = [Point(xy) for xy in zip(map_data['Longitude'], map_data['Latitude'])]
geo_df = gpd.GeoDataFrame(map_data, geometry=geometry)

# Plot map
fig, ax = plt.subplots(figsize=(12, 8))
india_map.plot(ax=ax, color='lightgrey', edgecolor='black')
scatter = ax.scatter(
    geo_df.geometry.x,
    geo_df.geometry.y,
    s=geo_df['Population Density (People/kmÂ²)'] / 10,
    c=geo_df['Waste Generated (Tons/Day)'],
    cmap='Reds',
    alpha=0.6,
    edgecolors='black'
)
for idx, row in geo_df.iterrows():
    ax.text(row['Longitude'], row['Latitude'], row['City/District'], fontsize=8)
cbar = plt.colorbar(scatter, ax=ax)
cbar.set_label('Waste Generated (Tons/Day)')
ax.set_title(f"Municipal Solid Waste Generation in Indian Cities - {selected_year}")
ax.axis('off')
st.pyplot(fig)

# City Comparison Analysis
if selected_cities:
    st.header(f"🏙 City Comparison - {', '.join(selected_cities)} ({selected_year})")

    # Total Waste per City Bar Chart
    waste_by_city = filtered_data.groupby('City/District')['Waste Generated (Tons/Day)'].sum().reset_index()
    fig_city_bar = px.bar(
        waste_by_city,
        x='City/District',
        y='Waste Generated (Tons/Day)',
        title="Total Waste Generated by City",
        text_auto='.2s',
        color='City/District',
        labels={'Waste Generated (Tons/Day)': 'Waste (Tons/Day)', 'City/District': 'City'}
    )
    st.plotly_chart(fig_city_bar, use_container_width=True)

    # City Metrics Table
    city_metrics = filtered_data.groupby('City/District').agg({
        'Waste Generated (Tons/Day)': 'sum',
        'Population Density (People/kmÂ²)': 'mean',
        'Landfill Capacity (Tons)': 'mean'
    }).reset_index().round(1)
    city_metrics = city_metrics.rename(columns={
        'Waste Generated (Tons/Day)': 'Total Waste (TPD)',
        'Population Density (People/kmÂ²)': 'Population Density (People/km²)',
        'Landfill Capacity (Tons)': 'Landfill Capacity (Tons)'
    })
    st.dataframe(city_metrics)

    # Waste Composition Pie Chart (all selected cities combined)
    waste_composition = filtered_data.groupby('Waste Type')['Waste Generated (Tons/Day)'].sum().reset_index()
    fig_pie = px.pie(
        waste_composition,
        names='Waste Type',
        values='Waste Generated (Tons/Day)',
        title=f"Waste Composition - {', '.join(selected_cities)} ({selected_year})"
    )
    st.plotly_chart(fig_pie, use_container_width=True)

# Waste Type Analysis (national or filtered)
st.header("♻ Waste Type Analysis")
waste_type_data = filtered_data.groupby('Waste Type')['Waste Generated (Tons/Day)'].sum().reset_index()
fig_waste_type = px.bar(
    waste_type_data,
    x='Waste Type',
    y='Waste Generated (Tons/Day)',
    title="Waste Generated by Type",
    labels={'Waste Generated (Tons/Day)': 'Waste (Tons/Day)', 'Waste Type': 'Type of Waste'},
    text_auto='.2s'
)
st.plotly_chart(fig_waste_type, use_container_width=True)

# Show raw data if selected
if show_raw_data:
    st.header("📊 Raw Data")
    st.dataframe(filtered_data)

st.sidebar.markdown("""
### About This Dashboard
This interactive dashboard displays waste management data for cities across India.

**Features:**
- National overview with geographic visualization
- City-specific waste generation analysis
- Multiple chart types for different perspectives
- Capacity utilization indicators
- Population density visualization

**Map Legend:**
- Circle size represents population
- Circle color represents waste volume
- Only major cities (>2M population) are labeled

**Data Shown:**
- Waste generation by type (tons/day)
- Landfill capacity information
- Geographic distribution
- Population data
""")