package com.twilio.example;

import com.twilio.example.TestRTDJNI.LoginListener;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.provider.Settings.Secure;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class LoginActivity extends Activity implements LoginListener {
	private static final Logger logger = Logger.getLogger(LoginActivity.class);
	private static final String AUTH_PHP_SCRIPT = "https://twilio-ip-messaging-token.herokuapp.com/token?ttl=999999&account_sid=AC522a45bc9300658950cebbac64f2bf78&auth_token=3e9b9f7d914c4b8e1e59ff29331d86df&service_sid=ISf24591d8f4a84fc7b0c99dff0ee3124b&identity=";
	private static final String DEFAULT_CLIENT_NAME = "Bagchi";

	private ProgressDialog progressDialog;
	private Button login;
	private Button logout;
	private String capabilityToken = null;
	private EditText clientNameTextBox;
	private TestRTDJNI testJNI;
	private String endpoint_id = "";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_login);

		this.clientNameTextBox = (EditText) findViewById(R.id.client_name);
		this.clientNameTextBox.setText(DEFAULT_CLIENT_NAME);
		this.endpoint_id = Secure.getString(this.getApplicationContext().getContentResolver(), Secure.ANDROID_ID);

		this.login = (Button) findViewById(R.id.register);
		this.login.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				StringBuilder url = new StringBuilder();
				url.append(AUTH_PHP_SCRIPT);
				url.append(clientNameTextBox.getText().toString());
				url.append("&endpoint_id=" + LoginActivity.this.endpoint_id);
				logger.e("url string : " + url.toString());
				new GetCapabilityTokenAsyncTask().execute(url.toString());
			}
		});

		this.logout = (Button) findViewById(R.id.logout);
		this.logout.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				testJNI.cleanupTest();
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
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

	private class GetCapabilityTokenAsyncTask extends AsyncTask<String, Void, String> {

		@Override
		protected void onPostExecute(String result) {
			super.onPostExecute(result);
			LoginActivity.this.testJNI.doTest(capabilityToken, LoginActivity.this);
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
				logger.e("capabilityToken string : " + capabilityToken);
				testJNI.setCapabilityToken(capabilityToken);
			} catch (Exception e) {
				e.printStackTrace();
			}
			return capabilityToken;
		}
	}

	@Override
	public void onLoginStarted() {
		// TODO Auto-generated method stub

	}

	@Override
	public void onLoginFinished() {
		LoginActivity.this.progressDialog.dismiss();
		Intent intent = new Intent(this, ChannelActivity.class);
		this.startActivity(intent);
	}

	@Override
	public void onLoginError(String errorMessage) {
		LoginActivity.this.progressDialog.dismiss();
		Toast.makeText(getBaseContext(), errorMessage, Toast.LENGTH_SHORT).show();
	}

	@Override
	public void onLogoutFinished() {
		// TODO Auto-generated method stub

	}

}
