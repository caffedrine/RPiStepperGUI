package beta.sophiaapp;

import android.content.pm.ActivityInfo;
import android.support.annotation.NonNull;
import android.support.design.widget.BottomNavigationView;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.FrameLayout;

public class MainActivity extends AppCompatActivity
{
	private BottomNavigationView mMainNav;
	private FrameLayout mMainFrame;

	private AutoFragment autoFragment;
	private ManualFragment manualFragment;
	private ConnectionFragment connectionFragment;

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		/* Allow only portrait mode */
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		mMainFrame = (FrameLayout) findViewById(R.id.main_frame);
		mMainNav = (BottomNavigationView) findViewById(R.id.main_nav);

		autoFragment = new AutoFragment();
		manualFragment = new ManualFragment();
		connectionFragment = new ConnectionFragment();

		setFragment(autoFragment);

		mMainNav.setOnNavigationItemSelectedListener(new BottomNavigationView.OnNavigationItemSelectedListener()
		{
			@Override
			public boolean onNavigationItemSelected(@NonNull MenuItem menuItem)
			{
				switch( menuItem.getItemId() )
				{
					case R.id.nav_auto:
					{
						setFragment(autoFragment);
						return true;
					}

					case R.id.nav_manual:
					{
						setFragment(manualFragment);
						return true;
					}

					case R.id.nav_connection:
					{
						setFragment(connectionFragment);
						return true;
					}

				}
				return false;
			}
		});
	}

	// create an action bar button
	@Override
	public boolean onCreateOptionsMenu(Menu menu)
	{
		getMenuInflater().inflate(R.menu.top_menu, menu);
		return super.onCreateOptionsMenu(menu);
	}

	// handle button activities
	@Override
	public boolean onOptionsItemSelected(MenuItem item)
	{
		int id = item.getItemId();

		if( id == R.id.reset_button )
		{
			// do something here
		}
		return super.onOptionsItemSelected(item);
	}


	private void setFragment(Fragment fragment)
	{
		FragmentTransaction fragmentTransaction = getSupportFragmentManager().beginTransaction();
		fragmentTransaction.replace(R.id.main_frame, fragment);
		fragmentTransaction.commit();
	}
}
