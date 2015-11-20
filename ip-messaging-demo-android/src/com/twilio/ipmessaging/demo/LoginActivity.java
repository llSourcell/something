package com.twilio.ipmessaging.demo;

import java.io.IOException;
import java.net.URLEncoder;

import com.google.android.gms.gcm.GoogleCloudMessaging;
import com.google.android.gms.iid.InstanceID;
import com.twilio.ipmessaging.Constants.StatusListener;
import com.twilio.ipmessaging.demo.BasicIPMessagingClient.LoginListener;
import com.twilio.rtd.demo.R;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.provider.Settings.Secure;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;


public class LoginActivity extends Activity implements LoginListener {
	private static final Logger logger = Logger.getLogger(LoginActivity.class);
		
	//private static final String AUTH_PHP_SCRIPT = "http://companyfoo.com/token";
	//Test with shared instance - Kumkum
	private static final String AUTH_PHP_SCRIPT = "https://twilio-ip-messaging-token.herokuapp.com/token?ttl=999999&account_sid=AC96ccc904753b3364f24211e8d9746a93&auth_token=647e1a16c9e5285b4188ca36e4aca150&service_sid=IS388ffb847f314e289175d682cc24cd72&identity=";
	
	private static final String DEFAULT_CLIENT_NAME = "TestUser";
	private ProgressDialog progressDialog;
	private Button login;
	private Button logout;
	private String capabilityToken = null;
	private EditText clientNameTextBox;
	private BasicIPMessagingClient chatClient;
	private String endpoint_id = "";
	public static String local_author = DEFAULT_CLIENT_NAME;
    private String PROJECT_NUMBER = "215048275735";
    private EditText etRegId;
	
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
				String idChosen = clientNameTextBox.getText().toString();
				String endpointIdFull = idChosen + "-" + LoginActivity.this.endpoint_id + "-android-"+ getApplication().getPackageName();

				StringBuilder url = new StringBuilder();
				url.append(AUTH_PHP_SCRIPT);
				url.append("&identity=");
				url.append(URLEncoder.encode(idChosen));
				url.append("&endpointId=" + URLEncoder.encode(endpointIdFull));
				url.append(clientNameTextBox.getText().toString());
				url.append("&endpoint_id=" + LoginActivity.this.endpoint_id);
				logger.e("url string : " + url.toString());
				new GetCapabilityTokenAsyncTask().execute(url.toString());
				//getRegId();
			}
		});

		this.logout = (Button) findViewById(R.id.logout);
		this.logout.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				chatClient.cleanupTest();
			}
		});
		etRegId = (EditText) findViewById(R.id.etRegId);
		chatClient = TwilioApplication.get().getBasicClient();
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
		logger.d("Log in started");
	}

	
	
	@Override
	public void onLoginFinished() {
//		?setUp();
		getRegId();
		LoginActivity.this.progressDialog.dismiss();
		Intent intent = new Intent(this, ChannelActivity.class);
		this.startActivity(intent);
	}

	@Override
	public void onLoginError(String errorMessage) {
		LoginActivity.this.progressDialog.dismiss();
		logger.e("Error logging in : " + errorMessage);
		Toast.makeText(getBaseContext(), errorMessage, Toast.LENGTH_LONG).show();
	}

	@Override
	public void onLogoutFinished() {
		// TODO Auto-generated method stub

	}
	
	public void setUp(){
		InstanceID instanceID = InstanceID.getInstance(getApplicationContext());
		try {
			instanceID.getToken(PROJECT_NUMBER, "/topics/global", null);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	
	
	 public void getRegId(){
	        new AsyncTask<Void, Void, String>() {
	            @Override
	            protected String doInBackground(Void... params) {
	                String token = "";
	                InstanceID instanceId = InstanceID.getInstance(getApplicationContext());
	                try {
	                	Log.e("LA", "GCM - Getting Token");
						token = instanceId.getToken(PROJECT_NUMBER, null);
						Log.e("LA", "GCM - Token: " + token);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
	                return token; 
	            }

	            @Override
	            protected void onPostExecute(String token) {
	            	etRegId.setText(token);
	            	Log.e("LA", "GCM - registering with native library");
	            	chatClient.getIpMessagingClient().registerGCMToken(token, new StatusListener() {
            			
    					@Override
    					public void onError() {
    						Log.e("LA", "GCM - onError()");
    					}
    	
    					@Override
    					public void onSuccess() {
    						Log.e("LA", "GCM - onSuccess()");
    					}
    	      		});
	            }
	        }.execute(null, null, null);
	    }

}
