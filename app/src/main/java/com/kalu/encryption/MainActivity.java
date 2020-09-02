package com.kalu.encryption;

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import lib.kalu.encryption.cipher.CipherTool;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // 加密
        findViewById(R.id.button1).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                TextView textView0 = findViewById(R.id.button0);
                CharSequence text = textView0.getText();

                String aesEncode = CipherTool.aesEncode(String.valueOf(text));
                TextView textView1 = findViewById(R.id.button1);
                textView1.setText(aesEncode);
            }
        });

        // 解密
        findViewById(R.id.button2).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                TextView textView1 = findViewById(R.id.button1);
                CharSequence text = textView1.getText();

                if ("点击加密".equals(text)) {
                    Toast.makeText(getApplicationContext(), "请先点击加密", Toast.LENGTH_SHORT).show();
                    return;
                }

                String aesDecode = CipherTool.aesDecode(String.valueOf(text));
                TextView textView2 = findViewById(R.id.button2);
                textView2.setText(aesDecode);
            }
        });

        // 重置
        findViewById(R.id.button3).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                TextView textView1 = findViewById(R.id.button1);
                textView1.setText("点击加密");

                TextView textView2 = findViewById(R.id.button2);
                textView2.setText("点击解密");
            }
        });
    }
}
