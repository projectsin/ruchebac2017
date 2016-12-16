package ruche.sti2d.core;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.os.Handler;
import android.widget.Toast;
import ruche.sti2d.network.Connector;
import ruchemanager.core.R;

public class MainActivity extends Activity {

    private Connector connector;
    private ProgressDialog progressDialog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        this.connector = new Connector(this);

        progressDialog = new ProgressDialog(this, R.style.AppTheme_Dark_Dialog) {{
            setIndeterminate(true);
        }};

    }

    @Override
    protected void onStart() {
        super.onStart();
        connect();
    }

    private void connect() {
        progressDialog.setMessage("Connexion...");
        progressDialog.show();
        connector.connect();
    }

    private void close() {
        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                MainActivity.super.finish();
            }
        },4000);
    }

    public void updateStatus(State state) {
        switch (state) {

            case CONNECTED:
                startActivity(new Intent(getApplicationContext(), ItemListActivity.class).addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP));
                progressDialog.dismiss();
                this.finish();
                break;

            case NOT_CONNECTED:
                progressDialog.dismiss();
                Toast.makeText(this, "Impossible de se connecter au serveur...", Toast.LENGTH_LONG).show();
                close();
                break;
        }
    }

    public enum State {
        CONNECTED,
        NOT_CONNECTED
    }
}
