
<p align="center" style="display: flex; align-items: center; justify-content: center;">
 <img alt="HackForEarth" src="https://encrypted-tbn1.gstatic.com/images?q=tbn:ANd9GcTwro2K2PD-hUpyWczT-5eZCwmQBsGoUR0OmVavmn73XREGbq3n" width="128" height="128" style="margin-left: 10px; margin-right: 10px;">
</p>
<h1 align="center">Smart Home System For Wastewater Treatment And Management </h1>

This project develops a smart home system for wastewater treatment and management, offering an innovative solution to water scarcity. It efficiently treats wastewater and assesses potability for safe household reuse. 

<br> <!-- This adds space between the title and the table of contents -->

## Table of contents 
1. [Problem Statement](#🚩-problem-statement)
2. [Solution](#🎯-solution)
3. [Water Treatment and Management Process](#⏩-water-treatment-and-management-process)
4. [Dataset](#📊-dataset)
5. ## Problem Statement
Household water demand has increased due to population growth, urbanization, and more water-intensive lifestyles. As cities grow, water supply systems are under pressure to meet daily needs like cooking, cleaning, and gardening. Climate change, with changing rainfall patterns and droughts, makes this problem worse by putting more strain on water resources. This highlights the need for better water management solutions, including reliable methods to assess and ensure household water is safe to use.
## Solution
We propose a smart home system for wastewater treatment and management, powered by a photovoltaic system. This eco-friendly solution will use advanced sensors to monitor, treat, and recycle wastewater efficiently, ensuring it is safe for potable uses. Through real-time data analysis, the system will give households insights into water quality, promoting sustainable practices. This approach addresses wastewater challenges, supports water conservation, reduces energy use, and contributes to public health.

## Water Treatment and Management Process

This section outlines the water treatment system, which purifies wastewater and ensures its safe reuse. Below is a step-by-step explanation of the process.

### 1. Wastewater Collection
- Wastewater from sinks, showers, and washing machines is collected in a storage tank.
- A water level sensor monitors the tank. When the water reaches a specific level, a pump activates to move the wastewater to the next stage.

### 2. Preliminary Filtration in the Screen Chamber
- The wastewater is filtered in the screen chamber, where larger debris (e.g., hair, particles) is removed to prepare the water for advanced treatment.

### 3. Biological Treatment in the Membrane Bioreactor
- The water enters the membrane bioreactor, which consists of two sections: the bioreactor and the submerged membrane tank.
- In the bioreactor, an anoxic tank facilitates the de-nitrification process, removing nitrogen compounds by converting nitrates into nitrogen gas.

### 4. Submerged Membrane Tank and Backwashing System
- After de-nitrification, water is pumped into the submerged membrane tank, where fine membranes filter out bacteria and other impurities.
- A backwashing system reverses water flow periodically to clean the membranes and prevent clogging.

### 5. Chemical Cleaning
- Regular chemical cleaning dissolves and removes stubborn particles to maintain membrane performance and prevent fouling.

### 6. UV Light Treatment
- After filtration, water is treated with ultraviolet (UV) light to eliminate pathogens by disrupting their DNA, ensuring safety from harmful microorganisms.

### 7. Reverse Osmosis for Chemical Contamination
- In the final treatment stage, reverse osmosis removes chemical contaminants such as heavy metals and dissolved salts, producing clean, safe water for reuse.

### 8. Sensor Data Collection
- Multiple sensors measure water quality parameters such as pH, turbidity, TDS, conductivity, sulfate, and chloramine levels.
- Data from these sensors is collected via Arduino and sent to a Raspberry Pi for further analysis.

### 9. Raspberry Pi and AI Integration
- The Raspberry Pi processes sensor data and uses an AI model trained on the 'Water Potability' dataset to predict water quality in real-time.
- The AI model evaluates sensor data to determine whether the water is potable or non-potable.

### 10. AI-Based Decision-Making
- Based on the AI model's prediction, the system either allows the water to be used for household purposes or redirects it to the wastewater treatment facility for further processing.

### 11. Cloud Data Transmission and Remote Monitoring
- All sensor data and water quality determinations are transmitted to a cloud platform for real-time monitoring and historical analysis.
- Users can monitor water quality through a cloud-based dashboard and receive alerts via email or SMS if there are issues (e.g., non-potable water or changes in quality).

## Dataset
The [dataset](https://www.kaggle.com/adityakadiwal/water-potability) contains water quality metrics for 3276 different water bodies.
1. pH value:
pH is crucial for assessing the acid-base balance of water, indicating whether it's acidic or alkaline. The World Health Organization (WHO) recommends a pH range of 6.5 to 8.5 for safe water. In the current analysis, pH values ranged from 6.52 to 6.83, staying within WHO guidelines.
2. Hardness:
Water hardness is mainly due to dissolved calcium and magnesium salts, which come from geologic formations the water travels through. The amount of time water contacts these materials determines its hardness. Hard water is known for its reduced ability to lather soap, caused by calcium and magnesium.
3. Solids (Total dissolved solids - TDS):
Water can dissolve various inorganic and organic minerals like potassium, calcium, sodium, and magnesium. High TDS levels can affect water taste and appearance. WHO recommends a desirable TDS limit of 500 mg/l and a maximum of 1,000 mg/l for drinking water.
4. Chloramines:
Chloramines, formed when ammonia is added to chlorine, are used to disinfect public water systems. According to health standards, chlorine levels up to 4 mg/L (or 4 ppm) are safe for drinking water.
5. Sulfate:
Sulfates are naturally found in soil, rocks, and water sources. While sulfate levels in freshwater typically range from 3 to 30 mg/L, concentrations can reach up to 1,000 mg/L in certain regions. Seawater contains approximately 2,700 mg/L of sulfates.
6. Conductivity:
Pure water doesn’t conduct electricity well, but increasing ion concentrations improve its conductivity. Electrical conductivity (EC) is determined by the amount of dissolved solids in water. WHO standards suggest EC should not exceed 400 μS/cm for safe water.
7. Organic_carbon:
TOC refers to the total carbon content in organic compounds, coming from decaying natural organic matter or synthetic sources. The US EPA recommends a TOC level below 2 mg/L for treated drinking water and below 4 mg/L for source water used in treatment.
8. Trihalomethanes:
THMs are by-products of chlorine disinfection. Their concentration in water depends on the amount of organic matter, chlorine usage, and water temperature. Levels of up to 80 ppm are considered safe in drinking water.
9. Turbidity:
Turbidity measures the presence of suspended particles in water, indicating its clarity. It also reflects waste discharge quality concerning colloidal matter. The turbidity value at Wondo Genet Campus (0.98 NTU) is well below the WHO recommended limit of 5.00 NTU.
10. Potability:
Potability indicates whether water is safe for human consumption, with a value of 1 meaning potable and 0 indicating non-potable.
