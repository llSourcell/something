package com.twilio.ipmessaging.demo;

import com.twilio.example.R;
import com.twilio.ipmessaging.demo.BasicIPMessagingClient.LoginListener;

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
	public  boolean DevEnvV2 = false; 
	//PROD TDL
	//private static final String AUTH_PHP_SCRIPT = "https://twilio-ip-messaging-token.herokuapp.com/token?ttl=999999&account_sid=AC96ccc904753b3364f24211e8d9746a93&auth_token=647e1a16c9e5285b4188ca36e4aca150&service_sid=IS2f100be24f76492abdc40352aa22e367&identity=";
	//STAGE TDL
	private static final String AUTH_PHP_SCRIPT = "https://twilio-ip-messaging-token.herokuapp.com/token?ttl=999999&account_sid=AC018d3c27feceaee897b3a8494c0140e1&auth_token=932bc154071289a4938b54fd77e05ae4&service_sid=ISe7e7ee18b69b4b29919dbc3a448269d4&identity=";
	//STAGE kumkum
	//private static final String AUTH_PHP_SCRIPT = "https://twilio-ip-messaging-token.herokuapp.com/token?ttl=999999&account_sid=AC6641b263fc8b4f5b3789bd7dc5821f36&auth_token=ad0c505ba447949c68c8d82da9a60161&service_sid=IS254427d0c166405db8f04b0381adadaa&identity=";
	//STAGE randy
	//private static final String AUTH_PHP_SCRIPT = "https://twilio-ip-messaging-token.herokuapp.com/token?account_sid=AC92db4e3791e1df3f0058418e957c02bd&auth_token=083a6ba873e5e0b80864f6df5477963e&ttl=1800&service_sid=IS01d66f0e2cdf4d869f2658c7d303b0ae&identity=";
	//PROD kumkum
	//private static final String AUTH_PHP_SCRIPT = "https://twilio-ip-messaging-token.herokuapp.com/token?ttl=999999&account_sid=AC522a45bc9300658950cebbac64f2bf78&auth_token=3e9b9f7d914c4b8e1e59ff29331d86df&service_sid=ISf24591d8f4a84fc7b0c99dff0ee3124b&identity=";
	private static final String AUTH_PHP_SCRIPT_RELEASE = "http://companyfoo.com/token";
	private static final String DEFAULT_CLIENT_NAME = "TestUser";
	
	private String devEnvToken = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJBQzc4ZThlNjdmYzAyNDY1MjE0OTBmYjk5MDdmZDBjMTY1IiwiZXhwIjoiMTQ0ODEwMjQ3NSIsInNjb3BlIjoic2NvcGU6Y2xpZW50Om91dGdvaW5nP2FwcFNpZD1BUDcwMGQ3M2ZlMTVjYWRjMzU3MWU4YTYyZTRkODFjODU3JmFwcFBhcmFtcz1jcmVkZW50aWFsX3NpZCUzRENSODUyNjAzNDgxZGM0YTBhZjc1NTJkODhhZTdjOWZiYTclMjZlbmRwb2ludF9pZCUzRDgzMGY2ZmQ0LWU0YTUtNDNlOC05OWEzLThiNDFlYTIzMzUzZiUyNmlkZW50aXR5JTNEZ3Jha2VuJTI2c2VydmljZV9zaWQlM0RJUzZiMGZhNjA4MzljNTExZTVhMTUxZmVmZjgxOWNkYzlmIn0.jcx2hDyvOe3BS4mqHrfahL8Web-_MjWlNlNTNcco6Tc";
			     
	private ProgressDialog progressDialog;
	private Button login;
	private Button logout;
	private String capabilityToken = null;
	private EditText clientNameTextBox;
	private BasicIPMessagingClient chatClient;
	private String endpoint_id = "";
	public static String local_author = DEFAULT_CLIENT_NAME;
	
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
				//new GetCapabilityTokenAsyncTask().execute(url.toString());
				if(DevEnvV2) {
					LoginActivity.this.chatClient.doLogin(devEnvToken, LoginActivity.this);
				} else {
					new GetCapabilityTokenAsyncTask().execute(url.toString());
				}
			}
		});

		this.logout = (Button) findViewById(R.id.logout);
		this.logout.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				chatClient.cleanupTest();
			}
		});

		chatClient = TwilioApplication.get().getRtdJni();
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
			LoginActivity.this.chatClient.doLogin(capabilityToken, LoginActivity.this);
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
				chatClient.setCapabilityToken(capabilityToken);
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
		if(!DevEnvV2) {
			LoginActivity.this.progressDialog.dismiss();
		}
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
