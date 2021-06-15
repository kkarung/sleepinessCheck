/*
 * Copyright (c) 2021. jisoo Kim all rights reserved.
 * This file is licenced under a Creative Commons license:
 * https://creativecommons.org/licenses/by/2.5/
*/

int trig = 4;               // 기기 연결
int echo = 3;               // 기기 연결
int led[] = {11, 12, 13};   // LED port
boolean s_init = false;     // 사용자와의 거리를 초기화하는 flag
int standard = 0;           // 초기 사용자와의 거리를 저장하는 변수(기준값)
int range = 10;             // {range}초동안의 (사용자와의 거리) 정보를 저장할 것
int record[10] = {0};       // state 기록을 저장하는 배열
int i = 0;                  // record의 index. 매초마다 state 기록

void setup() {
  pinMode(trig, OUTPUT);      // 초음파 기기 설정
  pinMode(echo, INPUT);       // 초음파 기기 설정
  for (int i = 0; i < 3; i++)
    pinMode(led[i], OUTPUT);  // LED 설정
  Serial.begin(9600);         // 시리얼 통신 시작
}

int checkState(int dis) {   // 사용자와의 거리를 입력 받고 state를 반환하는 함수
  int state;
  if (dis < standard - 20) {
    state = 2;
  }
  else if (dis < standard - 10) { // 
    state = 1;
  }
  else if (dis <= standard + 10) {  // 
    state = 0;
  }
  else {        // out of distance
    state = 3;
  }
  return state;
}

void loop() {
  digitalWrite(trig, HIGH);   // 초음파 기기 설정
  delayMicroseconds(10);      // 초음파 기기 설정
  digitalWrite(trig, LOW);    // 초음파 기기 설정
  
  // 초음파 발사

  int duration = pulseIn(echo, HIGH); // 초음파 기기 설정
  int dis = duration / 29 / 2;        // 초음파가 발사되면 물체까지의 시간 측정(단위: cm)

  if (s_init == false) {            // 시작할 때, standard 정하기
    standard = dis;                   // 현재 거리를 기준점으로 결정
    s_init = true;                    // flag 변환하여 standard를 고정함
    Serial.println("standard" + standard);
  }
  
  else {          // 시작 이후 distance
    i++;
    int cnt[5] = {0};                            // state 빈도수를 기록하는 배열 (매초 초기화됨!)
    record[i%range] = checkState(dis);           // state를 받아 기록하기(최근 10초 이내 정보만 기록)
    Serial.println(dis);
    for (int j=0; j<range; j++) {               // 기록된 값(최근 {range}초 state)을 훑어보면서
      cnt[record[j]]++;                         // state 분포도 기록
      if (record[j] == 2 && (record[j-1] <2))   // state가 1이하 ➝ 2가 되면
        cnt[4]++;
    }
    if (cnt[3] == range) {                      // {range}초 동안 state가 3이면
      Serial.print(i);
      Serial.println("초 자리 이탈 or 엎드림");
      digitalWrite(led[2], HIGH); //led[0]을 켜라
      digitalWrite(led[0], LOW); // led[0]을 꺼줘라.
      digitalWrite(led[1], LOW); // led[0]을 꺼줘라.
    }
    else if (record[i%range] == 0 || record[i%range] == 1) { // 0>2, 1>2 로 이동
      Serial.print(i);
      Serial.println("초 잘 듣고 있음");
      digitalWrite(led[0], HIGH); //led[0]을 켜라
      digitalWrite(led[1], LOW); // led[0]을 꺼줘라.
      digitalWrite(led[2], LOW); // led[0]을 꺼줘라.
    }

    else {
      Serial.print(i);
      Serial.println("초 졸고 있음");
      digitalWrite(led[1], HIGH); //led[0]을 켜라
      digitalWrite(led[0], LOW); // led[0]을 꺼줘라.
      digitalWrite(led[2], LOW); // led[0]을 꺼줘라.
    }
  }
  delay(1000);                                     // 1초 대기
}
