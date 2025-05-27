import streamlit as st
import pandas as pd
import matplotlib.pyplot as plt
import geopandas as gpd
import plotly.express as px
from shapely.geometry import Point
import os
import numpy as np


st.set_page_config(layout="wide")

@st.cache_data
def get_naturalearth_data():
    if not os.path.exists('naturalearth_lowres'):
        import urllib.request
        import zipfile
        
        url = "https://naciscdn.org/naturalearth/110m/cultural/ne_110m_admin_0_countries.zip"
        zip_path = "countries.zip"
        urllib.request.urlretrieve(url, zip_path)
        
        with zipfile.ZipFile(zip_path, 'r') as zip_ref:
            zip_ref.extractall("naturalearth_lowres")
        os.remove(zip_path)
    
    return gpd.read_file("naturalearth_lowres/ne_110m_admin_0_countries.shp")

@st.cache_data
def load_data():
    data = pd.read_excel('final_DAWaste_Management_and_Recycling_India.xlsx')
    
    city_info = {
        'Mumbai': (19.0760, 72.8777, 20.4),
        'Delhi': (28.7041, 77.1025, 18.7),
        'Bengaluru': (12.9716, 77.5946, 12.3),
        'Chennai': (13.0827, 80.2707, 10.9),
        'Kolkata': (22.5726, 88.3639, 14.9),
        'Hyderabad': (17.3850, 78.4867, 9.7),
        'Pune': (18.5204, 73.8567, 7.2),
        'Ahmedabad': (23.0225, 72.5714, 7.6),
        'Jaipur': (26.9124, 75.7873, 3.9),
        'Lucknow': (26.8467, 80.9462, 3.6),
        'Surat': (21.1702, 72.8311, 6.8),
        'Kanpur': (26.4499, 80.3319, 3.2),
        'Nagpur': (21.1458, 79.0882, 2.8),
        'Patna': (25.5941, 85.1376, 2.2),
        'Bhopal': (23.2599, 77.4126, 2.3),
        'Thiruvananthapuram': (8.5241, 76.9366, 1.7),
        'Indore': (22.7196, 75.8577, 2.5),
        'Vadodara': (22.3072, 73.1812, 2.1),
        'Guwahati': (26.1445, 91.7362, 1.1),
        'Coimbatore': (11.0168, 76.9558, 2.1),
        'Ranchi': (23.3441, 85.3096, 1.4),
        'Amritsar': (31.6340, 74.8723, 1.3),
        'Jodhpur': (26.2389, 73.0243, 1.5),
        'Varanasi': (25.3176, 82.9739, 1.5),
        'Ludhiana': (30.9010, 75.8573, 1.8),
        'Agra': (27.1767, 78.0081, 1.8),
        'Meerut': (28.9845, 77.7064, 1.5),
        'Nashik': (20.0059, 73.7910, 1.8),
        'Rajkot': (22.3039, 70.8022, 1.6),
        'Madurai': (9.9252, 78.1198, 1.6),
        'Jabalpur': (23.1815, 79.9864, 1.3),
        'Allahabad': (25.4358, 81.8463, 1.2),
        'Visakhapatnam': (17.6868, 83.2185, 2.0),
        'Gwalior': (26.2183, 78.1828, 1.2)
    }
    
    data['Latitude'] = data['city'].map(lambda x: city_info.get(x, (0, 0, 0))[0])
    data['Longitude'] = data['city'].map(lambda x: city_info.get(x, (0, 0, 0))[1])
    data['Population (Millions)'] = data['city'].map(lambda x: city_info.get(x, (0, 0, 0))[2])
    
    return data

data = load_data()

@st.cache_data
def load_india_map():
    world = get_naturalearth_data()
    india_map = world[world['ADMIN'] == 'India']
    return india_map

india_map = load_india_map()

cities = sorted(data['city'].unique())

st.markdown("Dashboard Created by Ayush Shirode")
st.title('India Waste Generation Analysis Dashboard')

st.sidebar.header('Filters')
selected_city = st.sidebar.selectbox('Select a City', cities)
show_raw_data = st.sidebar.checkbox('Show raw data')
show_national_view = st.sidebar.checkbox('Show national overview')

if show_national_view:
    st.header('National Waste Generation Overview')
    
    city_summary = data.groupby('city').agg({
        'WasteGenerated': 'sum',
        'Population (Millions)': 'first',
        'Latitude': 'first',
        'Longitude': 'first'
    }).reset_index()
    
    geometry = [Point(xy) for xy in zip(city_summary['Longitude'], city_summary['Latitude'])]
    geo_df = gpd.GeoDataFrame(city_summary, geometry=geometry)
    
    fig, ax = plt.subplots(figsize=(12, 8))
    india_map.plot(ax=ax, color='lightgrey', edgecolor='black')
    
    min_size = 20  
    max_size = 200 
    sizes = geo_df['Population (Millions)'] / geo_df['Population (Millions)'].max() * (max_size - min_size) + min_size
    
    scatter = ax.scatter(
        geo_df.geometry.x,
        geo_df.geometry.y,
        s=sizes,
        c=geo_df['WasteGenerated'],
        cmap='Reds',
        alpha=0.7,
        edgecolors='black',
        linewidths=0.5
    )
    
    cbar = plt.colorbar(scatter, ax=ax)
    cbar.set_label('WasteGenerated')
    
    for x, y, label, pop in zip(geo_df.geometry.x, geo_df.geometry.y, 
                                geo_df['city'], geo_df['Population (Millions)']):
        if pop > 2: 
            ax.text(x, y, label, fontsize=8, ha='right', va='bottom')
    
    ax.set_title('Waste Generation and Population Density Across Indian Cities\n'
                '(Circle size = Population, Color = Waste Volume)')
    ax.axis('off')
    st.pyplot(fig)
    
    col1, col2, col3 = st.columns(3)
    total_waste = data['WasteGenerated'].sum()
    avg_waste = data.groupby('city')['WasteGenerated'].sum().mean()
    cities_above_capacity = len([city for city in cities if 
                               data[data['city'] == city]['WasteGenerated'].sum() > 
                               data[data['city'] == city]['LandfillCapacity'].iloc[0]])
        
    st.header(f'🏙️ Waste Generation in {selected_city}')

def prepare_city_data(city):
    city_data = data[data['city'] == city]
    waste_types = city_data['WasteType'].unique()
    waste_amounts = city_data['WasteGenerated'].values
    landfill_capacity = city_data['LandfillCapacity'].iloc[0]
    total_waste = city_data['WasteGenerated'].sum()
    population = city_data['Population (Millions)'].iloc[0]
    
    min_length = min(len(waste_types), len(waste_amounts))
    return (waste_types[:min_length], waste_amounts[:min_length], 
            landfill_capacity, total_waste, population)

waste_types, waste_amounts, landfill_capacity, total_waste, population = prepare_city_data(selected_city)

tab1, tab2 = st.tabs(["Bar Chart", "Pie Chart"])

with tab1:
    if len(waste_types) == len(waste_amounts) and len(waste_types) > 0:
        fig, ax = plt.subplots(figsize=(10, 6))
        bars = ax.bar(waste_types, waste_amounts, label='Waste Generated')
        
        for bar in bars:
            height = bar.get_height()
            ax.text(bar.get_x() + bar.get_width()/2., height,
                    f'{height:.1f}',
                    ha='center', va='bottom')
        
        ax.set_ylabel('Tons per Day')
        ax.set_title(f'Daily Waste Generation in {selected_city}')
        ax.legend()
        st.pyplot(fig)
    else:
        st.warning(f"Data mismatch: Found {len(waste_types)} WasteTypes but {len(waste_amounts)} amounts")

with tab2:
    if len(waste_types) == len(waste_amounts) and len(waste_types) > 0:
        fig = px.pie(names=waste_types, values=waste_amounts, 
                    title=f'Waste Composition in {selected_city}')
        st.plotly_chart(fig, use_container_width=True)
    else:
        st.warning("Cannot display pie chart due to data mismatch")
    

if show_raw_data:
    st.subheader('Raw Data')
    st.write(data[data['city'] == selected_city])

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