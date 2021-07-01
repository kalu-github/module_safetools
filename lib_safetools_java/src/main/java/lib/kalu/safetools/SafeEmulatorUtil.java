package lib.kalu.safetools;

import android.content.pm.PackageManager;
import android.hardware.Sensor;
import android.hardware.SensorManager;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.InputStreamReader;

import static android.content.Context.SENSOR_SERVICE;

final class SafeEmulatorUtil {

    static final boolean checkEmulatorTel() {
        PackageManager packageManager = SafeContentProvider.mContext.getPackageManager();
        return packageManager.hasSystemFeature(PackageManager.FEATURE_TELEPHONY);
    }

    static final boolean checkEmulatorSensor() {
        SensorManager sensorManager = (SensorManager) SafeContentProvider.mContext.getSystemService(SENSOR_SERVICE);
        Sensor sensor = sensorManager.getDefaultSensor(Sensor.TYPE_LIGHT);
        return (null != sensor);
    }

    static final boolean checkEmulatorDrivers() {
        for (int i = 0; i < SafeConstant.EMULATOR_ARGS_DRIVERS.length; i++) {
            File file = new File(SafeConstant.EMULATOR_ARGS_DRIVERS[i]);
            if (null != file && file.exists() && file.canRead()) {
                byte[] data = new byte[1024];
                try {
                    InputStream is = new FileInputStream(file);
                    is.read(data);
                    is.close();
                } catch (Exception e) {
                    SafeLogUtil.log("checkEmulatorDrivers => " + e.getMessage());
                }

                String value = new String(data);
                for (String temp : SafeConstant.EMULATOR_ARGS_DRIVERS_VALUE) {
                    if (value.contains(temp)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    static final boolean checkEmulatorCpu() {
        try {
            ProcessBuilder cmd = new ProcessBuilder(SafeConstant.EMULATOR_ARGS_CPU);
            Process process = cmd.start();
            StringBuffer sb = new StringBuffer();
            String readLine;
            BufferedReader responseReader = new BufferedReader(new InputStreamReader(process.getInputStream(), "utf-8"));
            while ((readLine = responseReader.readLine()) != null) {
                sb.append(readLine);
            }
            responseReader.close();

            String s = sb.toString().toLowerCase();
            if (null != s && s.contains(SafeConstant.EMULATOR_INTEL))
                return false;

            if (null != s && s.contains(SafeConstant.EMULATOR_AMD))
                return false;

            return true;
        } catch (Exception e) {
            SafeLogUtil.log("checkEmulatorCpu => " + e.getMessage());
            return true;
        }
    }
}
