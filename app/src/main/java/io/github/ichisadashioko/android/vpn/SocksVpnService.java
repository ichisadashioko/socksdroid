package io.github.ichisadashioko.android.vpn;

import android.content.Intent;
import android.net.VpnService;

public class SocksVpnService extends VpnService {
    public static final String TAG = SocksVpnService.class.getName();

    public boolean isRunning = false;

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if (intent == null) {
            return START_STICKY;
        }

        if (isRunning) {
            return START_STICKY;
        }

        return START_STICKY;
    }
}
