package lib.kalu.encryption.multidex;

import android.app.Application;
import android.content.Context;

public class BoostMultiDexApplication extends Application {
    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);

        if (!BoostMultiDex.isOptimizeProcess(Utility.getCurProcessName(base))) {
            return;
        }

        BoostMultiDex.install(base);
    }
}
