package com.example.mqttfromm5;

import android.graphics.Color;
import android.os.Bundle;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;
import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;
import org.eclipse.paho.client.mqttv3.*;

public class MainActivity extends AppCompatActivity {

    private LineChart lineChart;
    private MqttClient mqttClient;
    private TextView tvStatus;
    private float timeIndex = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        lineChart = findViewById(R.id.lineChart);
        tvStatus = findViewById(R.id.tvStatus);

        setupChart();
        connectMQTT();
    }

    private void setupChart() {
        lineChart.setData(new LineData());
        lineChart.getDescription().setEnabled(false);
        lineChart.getLegend().setEnabled(true);
    }

    private void connectMQTT() {
        // ใส่ IP ของ DigitalOcean ของคุณ
        String serverUri = "tcp:// ip :1883"; //ip droplet
        String clientId = "Android_" + MqttClient.generateClientId();

        try {
            mqttClient = new MqttClient(serverUri, clientId, null);
            MqttConnectOptions options = new MqttConnectOptions();
            options.setUserName("m5user"); // User ที่คุณสร้างใน Mosquitto
            options.setPassword("password".toCharArray()); // รหัสผ่านที่คุณสร้างใน Mosquitto


            mqttClient.setCallback(new MqttCallback() {
                @Override
                public void messageArrived(String topic, MqttMessage message) {
                    String tempStr = message.toString();
                    float temp = Float.parseFloat(message.toString());

                    runOnUiThread(() -> {
                        tvStatus.setText(getString(R.string.temp_display, tempStr));
                        addEntry(temp);
                    });
                }
                @Override public void connectionLost(Throwable cause) {
                    runOnUiThread(() -> tvStatus.setText(R.string.status_failed));
                }
                @Override public void deliveryComplete(IMqttDeliveryToken token) {}


            });

            mqttClient.connect(options);
            mqttClient.subscribe("m5stack/temp"); // Topic เดียวกับที่ M5 ส่ง
            tvStatus.setText(R.string.status_connected);

        } catch (MqttException e) {
            tvStatus.setText(R.string.status_failed);
        }
    }

    private void addEntry(float temp) {
        LineData data = lineChart.getData();
        if (data != null) {
            ILineDataSet set = data.getDataSetByIndex(0);
            if (set == null) {
                set = createSet();
                data.addDataSet(set);
            }
            data.addEntry(new Entry(timeIndex++, temp), 0);
            data.notifyDataChanged();
            lineChart.notifyDataSetChanged();
            lineChart.setVisibleXRangeMaximum(15); // แสดง 15 ข้อมูลล่าสุดบนจอ
            lineChart.moveViewToX(data.getEntryCount());
        }
    }

    private LineDataSet createSet() {
        LineDataSet set = new LineDataSet(null, "Temperature (°C)");
        set.setColor(Color.BLUE);
        set.setCircleColor(Color.RED);
        set.setLineWidth(2f);
        set.setDrawValues(false);
        return set;
    }
}