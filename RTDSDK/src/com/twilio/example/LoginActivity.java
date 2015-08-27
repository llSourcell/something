package com.twilio.example;

import android.app.Activity;
import android.app.ProgressDialog;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;

public class LoginActivity extends Activity {
	
	private Button login;
	private ProgressDialog progressDialog;
	private String capabilityToken = null;
	
	private TestRTDJNI testJNI;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.login);
		
		this.login = (Button)findViewById(R.id.register);
		this.login.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				new LoginAsyncTask().execute();
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
			LoginActivity.this.testJNI.testRTD();
			return null;
		}

	}
	
	
	private class GetAuthTokenAsyncTask extends AsyncTask<String, Void, String> {

		@Override
		protected void onPostExecute(String result) {
			super.onPostExecute(result);
		}

		@Override
		protected String doInBackground(String... params) {
			
			try {
				capabilityToken = HttpHelper.httpGet(params[0]);;
			} catch (Exception e) {
				e.printStackTrace();
			}
			return capabilityToken;
		}
    }
	
}
