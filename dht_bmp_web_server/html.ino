String webpage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>DHT22 & BMP Monitor</title>
  <script src="http://cdn.jsdelivr.net/npm/chart.js@3.9.1/dist/chart.min.js"></script>
  <style>
    html, body {
      height: 100%;
      margin: 0;
      padding: 0;
      font-family: Arial;
      display: flex;
      flex-direction: column;
    }

    h2 {
      margin: 10px 0;
      text-align: center;
    }

    /* Full vertical stack layout */
    .chart-grid {
      flex: 1;
      display: flex;
      flex-direction: column;
      height: calc(100% - 40px); /* subtract header height */
    }

    /* Each chart takes equal height of available space */
    .chart-container {
      flex: 1; /* equal share of vertical space */
      display: flex;
      flex-direction: column;
      background: #f9f9f9;
      border-radius: 10px;
      margin: 5px 10px;
      padding: 5px;
      box-shadow: 0 2px 6px rgba(0,0,0,0.1);
      overflow: hidden;
    }

    .chart-container h3 {
      margin: 0;
      font-size: 1rem;
    }

    canvas {
      flex: 1;
      width: 100%;
      height: 100%;
    }

    @media (max-width: 600px) {
      .chart-container h3 {
        font-size: 0.9rem;
      }
    }
  </style>
</head>
<body>
  <h2>DHT22 & BMP Sensor Dashboard</h2>

  <div class="chart-grid">
    <!-- DHT Temperature -->
    <div class="chart-container">
      <h3>DHT Temp (&deg;C)</h3>
      <canvas id="dhtTempChart"></canvas>
    </div>

    <!-- DHT Humidity -->
    <div class="chart-container">
      <h3>DHT Humidity (%)</h3>
      <canvas id="dhtHumChart"></canvas>
    </div>

    <!-- BMP Temperature -->
    <div class="chart-container">
      <h3>BMP Temp (&deg;C)</h3>
      <canvas id="bmpTempChart"></canvas>
    </div>

    <!-- BMP Pressure -->
    <div class="chart-container">
      <h3>BMP Pressure (hPa)</h3>
      <canvas id="bmpPressureChart"></canvas>
    </div>

    <!-- BMP Altitude -->
    <div class="chart-container">
      <h3>BMP Altitude (m)</h3>
      <canvas id="bmpAltitudeChart"></canvas>
    </div>
  </div>

  <script>
    // Chart initialization function
    function createChart(ctx, label, color, yText) {
      return new Chart(ctx, {
        type: 'line',
        data: {
          labels: [],
          datasets: [{
            label: label,
            borderColor: color,
            data: [],
            fill: false,
            tension: 0.1
          }]
        },
        options: {
          responsive: true,
          maintainAspectRatio: false,
          scales: {
            x: { title: { display: true, text: 'Time' }},
            y: { title: { display: true, text: yText }}
          }
        }
      });
    }

    // Initialize charts
    const dhtTempChart = createChart(document.getElementById('dhtTempChart').getContext('2d'), 'DHT Temp (°C)', 'red', 'Temperature (°C)');
    const dhtHumChart = createChart(document.getElementById('dhtHumChart').getContext('2d'), 'DHT Humidity (%)', 'blue', 'Humidity (%)');
    const bmpTempChart = createChart(document.getElementById('bmpTempChart').getContext('2d'), 'BMP Temp (°C)', 'orange', 'Temperature (°C)');
    const bmpPressureChart = createChart(document.getElementById('bmpPressureChart').getContext('2d'), 'BMP Pressure (hPa)', 'green', 'Pressure (hPa)');
    const bmpAltitudeChart = createChart(document.getElementById('bmpAltitudeChart').getContext('2d'), 'BMP Altitude (m)', 'purple', 'Altitude (m)');

    // Helper to update charts
    function updateChart(chart, value, time) {
      chart.data.labels.push(time);
      chart.data.datasets[0].data.push(value);
      if (chart.data.labels.length > 20) {
        chart.data.labels.shift();
        chart.data.datasets[0].data.shift();
      }
      chart.update();
    }

    // Update every x seconds
    setInterval(async () => {
      const res = await fetch('/data');
      const json = await res.json();

      const now = new Date().toLocaleTimeString();

      // Update DHT Charts
      updateChart(dhtTempChart, json.dht_temperature, now);
      updateChart(dhtHumChart, json.dht_humidity, now);

      // Update BMP Charts
      updateChart(bmpTempChart, json.bmp_temperature, now);
      updateChart(bmpPressureChart, json.bmp_pressure_hPa, now);
      updateChart(bmpAltitudeChart, json.bmp_altitude, now);

    }, 2000);
  </script>
</body>
</html>
)rawliteral";