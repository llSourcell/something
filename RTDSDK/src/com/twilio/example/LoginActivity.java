package com.twilio.example;

import android.app.Activity;
import android.app.ProgressDialog;
import android.os.AsyncTask;
import android.os.Bundle;
import android.provider.Settings.Secure;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class LoginActivity extends Activity {
	
	private Button login;
	private Button logout;
	private ProgressDialog progressDialog;
	private String capabilityToken = null;
	private EditText clientNameTextBox;
	private static final String DEFAULT_CLIENT_NAME = "kumkum";
	private TestRTDJNI testJNI;
	private String endpoint_id = "";
	
	private static final String AUTH_PHP_SCRIPT = "https://twilio-ip-messaging-token.herokuapp.com/token?identity=";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.login);
		
		clientNameTextBox = (EditText)findViewById(R.id.client_name);
        clientNameTextBox.setText(DEFAULT_CLIENT_NAME);
        this.endpoint_id = Secure.getString(this.getApplicationContext().getContentResolver(), Secure.ANDROID_ID);
		
		this.login = (Button)findViewById(R.id.register);
		this.login.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				StringBuilder url = new StringBuilder();
		    	url.append(AUTH_PHP_SCRIPT);
				url.append(clientNameTextBox.getText().toString());
				url.append("&endpoint_id="+LoginActivity.this.endpoint_id);
				
				new GetCapabilityTokenAsyncTask().execute(url.toString());
			}
		});
		
		this.logout = (Button)findViewById(R.id.logout);
		this.logout.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				testJNI.shutdown();
			}
		});
		
		
		testJNI = new TestRTDJNI();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.login, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
	
	
	private class LoginAsyncTask extends AsyncTask<String, Void, String> {

		@Override
	    protected void onPreExecute() {
	        super.onPreExecute();
	        LoginActivity.this.progressDialog = ProgressDialog.show(LoginActivity.this, "",
	                "Logging in. Please wait...", true);
		}

		@Override
		protected String doInBackground(String... params) {
			LoginActivity.this.testJNI.doTest("");//testRTD();
			return null;
		}

	}
	
	
	private class GetCapabilityTokenAsyncTask extends AsyncTask<String, Void, String> {

		@Override
		protected void onPostExecute(String result) {
			super.onPostExecute(result);
			LoginActivity.this.testJNI.doTest(capabilityToken);
		}
		
		@Override
	    protected void onPreExecute() {
	        super.onPreExecute();
	        LoginActivity.this.progressDialog = ProgressDialog.show(LoginActivity.this, "",
	                "Logging in. Please wait...", true);
		}

		@Override
		protected String doInBackground(String... params) {
			
			try {
				capabilityToken = HttpHelper.httpGet(params[0]);
				testJNI.setCapabilityToken(capabilityToken);
			} catch (Exception e) {
				e.printStackTrace();
			}
			return capabilityToken;
		}
    }
	
}
