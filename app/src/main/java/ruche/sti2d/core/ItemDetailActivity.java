package ruche.sti2d.core;


import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.WindowManager;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.AxisBase;
import com.github.mikephil.charting.components.Legend;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.formatter.IAxisValueFormatter;
import com.github.mikephil.charting.highlight.Highlight;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;
import com.github.mikephil.charting.listener.OnChartValueSelectedListener;
import com.github.mikephil.charting.utils.ColorTemplate;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Locale;
import java.util.Random;
import java.util.concurrent.TimeUnit;

import ruchemanager.core.R;

/**
 * An activity representing a single Item detail screen. This
 * activity is only used narrow width devices. On tablet-size devices,
 * item details are presented side-by-side with a list of items
 * in a {@link ItemListActivity}.
 */
public class ItemDetailActivity extends FragmentActivity implements OnChartValueSelectedListener {


    private LineChart mChart;

    protected float getRandom(float range, float startsfrom) {
        return (float) (Math.random() * range) + startsfrom;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

        setContentView(R.layout.chart);

        mChart = (LineChart) findViewById(R.id.chart1);
        mChart.setOnChartValueSelectedListener(this);

        mChart.setDrawGridBackground(false);
        mChart.getDescription().setEnabled(false);
        mChart.setDrawBorders(false);

        mChart.getAxisLeft().setEnabled(false);
        mChart.getAxisRight().setDrawAxisLine(false);
        mChart.getAxisRight().setDrawGridLines(false);
        mChart.getXAxis().setDrawAxisLine(false);
        mChart.getXAxis().setDrawGridLines(false);

        mChart.getXAxis().setValueFormatter(new IAxisValueFormatter() {

            private SimpleDateFormat mFormat = new SimpleDateFormat("dd MMM HH:mm", Locale.FRANCE);

            @Override
            public String getFormattedValue(float value, AxisBase axis) {
                long millis = TimeUnit.HOURS.toMillis((long) value);
                return mFormat.format(new Date(millis));
            }
        });

        // enable touch gestures
        mChart.setTouchEnabled(true);

        // enable scaling and dragging
        mChart.setDragEnabled(true);
        mChart.setScaleEnabled(true);

        // if disabled, scaling can be done on x- and y-axis separately
        mChart.setPinchZoom(false);


        Legend l = mChart.getLegend();
        l.setVerticalAlignment(Legend.LegendVerticalAlignment.BOTTOM);
        l.setHorizontalAlignment(Legend.LegendHorizontalAlignment.CENTER);
        l.setOrientation(Legend.LegendOrientation.HORIZONTAL);
        l.setDrawInside(false);

        ArrayList<ILineDataSet> dataSets = new ArrayList<>();
        dataSets.add(generateValue("Masse", mColors[0] ,1));
        dataSets.add(generateValue("Humidité", mColors[1], 2));
        dataSets.add(generateValue("Temperature", mColors[2], 3));

        LineData data = new LineData(dataSets);
        mChart.setData(data);
        mChart.invalidate();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        //getMenuInflater().inflate(R.menu.line, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {


        return true;
    }

    private int[] mColors = new int[]{
            ColorTemplate.VORDIPLOM_COLORS[0],
            ColorTemplate.VORDIPLOM_COLORS[1],
            ColorTemplate.VORDIPLOM_COLORS[2]
    };

    private ILineDataSet generateValue(String name, int color, int id) {
        ArrayList<Entry> values = new ArrayList<>();
        switch (id) {
            case 1 : {
                values.add(new Entry(1, 60));
                values.add(new Entry(2, 70));
                values.add(new Entry(3, 80));
                values.add(new Entry(4, 80));
                values.add(new Entry(5, 75));
                values.add(new Entry(6, 90));
                values.add(new Entry(7, 120));
                break;
            }

            case 2: {
                values.add(new Entry(1, 60));
                values.add(new Entry(2, 73));
                values.add(new Entry(3, 81));
                values.add(new Entry(4, 83));
                values.add(new Entry(5, 76));
                values.add(new Entry(6, 85));
                values.add(new Entry(7, 91));
                break;
            }

            case 3: {
                values.add(new Entry(1, 37));
                values.add(new Entry(2, 38));
                values.add(new Entry(3, 37));
                values.add(new Entry(4, 37));
                values.add(new Entry(5, 40));
                values.add(new Entry(6, 41));
                values.add(new Entry(7, 43));
                break;
            }
        }


        LineDataSet d = new LineDataSet(values, name);
        d.setLineWidth(2.5f);
        d.setCircleRadius(4f);

        d.setColor(color);
        d.setCircleColor(color);

        return d;
    }

    @Override
    public void onValueSelected(Entry e, Highlight h) {

    }

    @Override
    public void onNothingSelected() {

    }
}
