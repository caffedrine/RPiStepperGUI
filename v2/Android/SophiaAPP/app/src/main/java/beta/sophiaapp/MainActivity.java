package beta.sophiaapp;

import android.support.annotation.NonNull;
import android.support.design.widget.BottomNavigationView;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
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
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		mMainFrame = (FrameLayout) findViewById(R.id.main_frame);
		mMainNav = (BottomNavigationView)  findViewById(R.id.main_nav);

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


	private void setFragment(Fragment fragment)
	{
		FragmentTransaction fragmentTransaction  = getSupportFragmentManager().beginTransaction();
		fragmentTransaction.replace(R.id.main_frame, fragment);
		fragmentTransaction.commit();
	}
}
