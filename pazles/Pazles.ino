/*  ──────────────────────────────────────────────────────────
    Линейный трек-рекордер + исполнение шаблонов движения
    (c) 2025 woronin05@yandex.ru 
    ────────────────────────────────────────────────────────── */

// ───── ПИНЫ ─────
#define RIGHT_DIR_PIN 5
#define RIGHT_PWM_PIN 6
#define LEFT_DIR_PIN  8
#define LEFT_PWM_PIN  9

#define LINE_SENSOR   7               // 0 = white, 1 = black
#define LED_PIN       13              // built-in LED
#define BUTTON_PIN    0               // RX, button to GND (INPUT_PULLUP)

// ───── ДВИЖЕНИЕ ─────
int BASE_SPEED  = 150;
int CORRECTION  = 5;

// ───── ТАЙМИНГИ ─────
unsigned long WARMUP_MS      = 3000;   // стоим
unsigned long DRIVE_MS       = 1000;   // едем и пишем лог
unsigned long BLINK_PREP_MS  = 500;    // ST_PREP
unsigned long BLINK_WAIT_MS  = 50;     // ST_WAIT (10 раз/с)

// ───── ДРЕБЕЗГ ─────
const unsigned long DEBOUNCE_US = 500; // 0,5 мс — датчик линии
const unsigned long BTN_DEB_MS  = 30;  // 30 мс — кнопка

// ───── ЛОГ ─────
const uint16_t MAX_EVENTS  = 200;
const byte     PATTERN_BITS = 6;
const byte     MAX_PATTERNS = 4;

unsigned long eventTimeUs[MAX_EVENTS];
byte          eventState [MAX_EVENTS];
uint16_t      nEvents = 0;

/* ─────────────── СЛУЖЕБНЫЕ ─────────────── */
byte          prevState;
unsigned long driveStartUs;
unsigned long lastEdgeUs;
bool          driving = false;

/* ─────────────── ШАБЛОНЫ ДЛЯ ПОСЛЕДУЮЩИХ ДЕЙСТВИЙ ─────────────── */
uint8_t patternList[MAX_PATTERNS];
byte     numPatterns = 0;

/* ─────────────── МАШИНА СОСТОЯНИЙ ─────────────── */
enum State : byte { ST_ON, ST_PREP, ST_READ, ST_WAIT };
State          curState   = ST_ON;
unsigned long  stateStart = 0;
unsigned long  lastBlink  = 0;
bool           lastBtn    = HIGH;

/********************************************************************/
void setup() {
  // моторы
  pinMode(RIGHT_DIR_PIN, OUTPUT);
  pinMode(RIGHT_PWM_PIN, OUTPUT);
  pinMode(LEFT_DIR_PIN,  OUTPUT);
  pinMode(LEFT_PWM_PIN,  OUTPUT);

  // периферия
  pinMode(LINE_SENSOR, INPUT);
  pinMode(LED_PIN,      OUTPUT);
  pinMode(BUTTON_PIN,   INPUT_PULLUP);

  Serial.begin(115200);
  Serial.println(F("Система включена. Нажмите кнопку — старт."));
  digitalWrite(LED_PIN, HIGH);                     // ST_ON: горит
}

/********************************************************************/
void loop() {
  unsigned long nowMs = millis();
  unsigned long nowUs = micros();

  /* ───── чтение кнопки ───── */
  bool btn = digitalRead(BUTTON_PIN);
  static unsigned long lastBtnChange = 0;
  if (btn != lastBtn && nowMs - lastBtnChange >= BTN_DEB_MS) {
    lastBtn       = btn;
    lastBtnChange = nowMs;
    if (btn == LOW) {                               // нажали
      if (curState == ST_ON) {
        enterState(ST_PREP);
      } else if (curState == ST_WAIT) {
        executePatterns();                          // ① мигаем, ② едем
        enterState(ST_ON);                          // обратно в ожидание
      }
    }
  }

  /* ───── логика состояний ───── */
  switch (curState) {

    /* ─── ST_ON: ждём кнопку ─── */
    case ST_ON:
      break;

    /* ─── ST_PREP: t=3 с, затем старт ─── */
    case ST_PREP:
      if (nowMs - stateStart >= WARMUP_MS) {
        startLogging();
        enterState(ST_READ);
      }
      break;

    /* ─── ST_READ: едем, пишем лог ─── */
    case ST_READ:
      if (driving) {
        forwardCorrected(BASE_SPEED, CORRECTION);

        byte cur = digitalRead(LINE_SENSOR);
        if (cur != prevState && (nowUs - lastEdgeUs) >= DEBOUNCE_US) {
          if (nEvents < MAX_EVENTS) {
            eventTimeUs[nEvents] = nowUs - driveStartUs;
            eventState [nEvents] = cur;
            ++nEvents;
          }
          prevState  = cur;
          lastEdgeUs = nowUs;
        }
        if (nowUs - driveStartUs >= DRIVE_MS * 1000UL) {
          stopMotors();
          driving = false;
          processPatterns();
          Serial.println(F("— цикл завершён —\nОжидание кнопки…"));
          enterState(ST_WAIT);
        }
      }
      break;

    /* ─── ST_WAIT: ждём кнопку ─── */
    case ST_WAIT:
      break;
  }

  /* ───── Обновление светодиода ───── */
  updateLed(nowMs);
}

/********************************************************************/
/* ─────────── ФУНКЦИИ ПЕРЕХОДА ─────────── */
void enterState(State s) {
  curState   = s;
  stateStart = millis();
  lastBlink  = stateStart;

  switch (s) {
    case ST_ON:   digitalWrite(LED_PIN, HIGH);  break;
    case ST_READ: digitalWrite(LED_PIN, LOW);   break;
    default:      /* ST_PREP & ST_WAIT — управляет updateLed() */ break;
  }
}

/********************************************************************/
/* ─────────── LED: управление по состоянию ─────────── */
void updateLed(unsigned long nowMs) {
  switch (curState) {
    case ST_ON:   /* уже горит постоянно */             break;

    case ST_PREP:
      if (nowMs - lastBlink >= BLINK_PREP_MS) {
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        lastBlink = nowMs;
      }
      break;

    case ST_READ: /* выключен */                        break;

    case ST_WAIT:
      if (nowMs - lastBlink >= BLINK_WAIT_MS) {
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        lastBlink = nowMs;
      }
      break;
  }
}

/********************************************************************/
/* ─────────── ЗАПУСК ЛОГИРОВАНИЯ ─────────── */
void startLogging() {
  driveStartUs = micros();
  prevState    = digitalRead(LINE_SENSOR);
  lastEdgeUs   = driveStartUs;

  eventTimeUs[0] = 0;
  eventState [0] = prevState;
  nEvents        = 1;
  driving        = true;

  numPatterns    = 0;              // очистка предыдущих шаблонов
  Serial.println(F("Поехали! (1,0 с съёмки)"));
}

/********************************************************************/
/* ─────────── РАЗБОР ШАБЛОНОВ ─────────── */
void processPatterns() {
  const byte MAX_EDGES = 32;
  unsigned long rise[MAX_EDGES];
  byte nRise = 0;

  byte prev = eventState[0];
  for (uint16_t i = 1; i < nEvents; ++i) {
    byte cur = eventState[i];
    if (prev == 0 && cur == 1 && nRise < MAX_EDGES)
      rise[nRise++] = eventTimeUs[i];
    prev = cur;
  }

  if (nRise < 2) {
    Serial.println(F("ERROR: менее двух подъёмов — нет синхронизации"));
    return;
  }

  const float TOL = 0.5f;
  byte pCount = 0, i = 0;

  while (i + 1 < nRise && pCount < MAX_PATTERNS) {
    unsigned long slotUs = rise[i + 1] - rise[i];
    if (slotUs < 100) { ++i; continue; }

    unsigned long t0 = rise[i];
    float slot = float(slotUs);

    bool bit[PATTERN_BITS] = {false};
    bit[0] = bit[1] = true;

    byte k = i + 2;
    while (k < nRise) {
      float p   = (rise[k] - t0) / slot;
      int   idx = int(round(p));
      if (fabs(p - idx) > TOL || idx >= PATTERN_BITS) break;
      bit[idx] = true;
      ++k;
    }

    uint8_t mask = 0;
    for (byte b = 0; b < PATTERN_BITS; ++b)
      if (bit[b]) mask |= (1 << (PATTERN_BITS - 1 - b));

    if (pCount == 0) {
      Serial.println();
      Serial.print(F("Slot: ")); Serial.print(slot, 1); Serial.println(F(" µs"));
    }
    Serial.print(F("Pattern ")); Serial.print(pCount + 1);
    Serial.print(F(": 0b"));
    for (int b = PATTERN_BITS - 1; b >= 0; --b) Serial.print(bitRead(mask, b));
    Serial.print(F(" (0x"));
    if (mask < 0x10) Serial.print('0');
    Serial.print(mask, HEX);
    Serial.println(')');

    patternList[pCount] = mask;    // — сохраняем
    ++pCount;
    i = k;
  }
  numPatterns = pCount;

  Serial.print(F("\nОбнаружено шаблонов: ")); Serial.println(pCount);
}

/********************************************************************/
/* ─────────── ДВИЖЕНИЕ ─────────── */
void forwardCorrected(int baseSpeed, int correction) {
  digitalWrite(RIGHT_DIR_PIN, LOW);
  digitalWrite(LEFT_DIR_PIN,  LOW);
  analogWrite(RIGHT_PWM_PIN, baseSpeed + correction);
  analogWrite(LEFT_PWM_PIN,  baseSpeed);
}

void stopMotors() {
  digitalWrite(RIGHT_DIR_PIN, LOW);
  digitalWrite(LEFT_DIR_PIN,  LOW);
  analogWrite(RIGHT_PWM_PIN, 0);
  analogWrite(LEFT_PWM_PIN,  0);
}

/* ─────────── ПРОСТЫЕ КОМАНДЫ ДВИЖЕНИЯ ─────────── */
void moveForward (unsigned long ms) {
  forwardCorrected(BASE_SPEED, 0);
  delay(ms);
  stopMotors();
}

void moveBackward(unsigned long ms) {           // оба мотора назад
  digitalWrite(RIGHT_DIR_PIN, HIGH);
  digitalWrite(LEFT_DIR_PIN , HIGH);
  analogWrite (RIGHT_PWM_PIN, 255 - BASE_SPEED);
  analogWrite (LEFT_PWM_PIN , 255 - BASE_SPEED);
  delay(ms);
  stopMotors();
}

void turnLeft (unsigned long ms) {              // правый — вперёд, левый — назад
  digitalWrite(RIGHT_DIR_PIN, LOW);
  digitalWrite(LEFT_DIR_PIN , LOW);
  analogWrite (RIGHT_PWM_PIN, BASE_SPEED);
  analogWrite (LEFT_PWM_PIN , 0);
  delay(ms);
  stopMotors();
}

void turnRight(unsigned long ms) {              // правый — назад, левый — вперёд
  digitalWrite(RIGHT_DIR_PIN, LOW);
  digitalWrite(LEFT_DIR_PIN , LOW);
  analogWrite (RIGHT_PWM_PIN, 0);
  analogWrite (LEFT_PWM_PIN , BASE_SPEED);
  delay(ms);
  stopMotors();
}

/* ─────────── МИГАНИЕ LED ─────────── */
void blinkLed(byte times, unsigned int msDelay) {
  for (byte i = 0; i < times; ++i) {
    digitalWrite(LED_PIN, HIGH);
    delay(msDelay);
    digitalWrite(LED_PIN, LOW);
    delay(msDelay);
  }
}

/* ─────────── ИСПОЛНЕНИЕ СЧИТАННЫХ ШАБЛОНОВ ─────────── */
void executePatterns() {
  blinkLed(3, 500);               // 3 × 500 мс — индикация старта

  for (byte i = 0; i < numPatterns; ++i) {
    switch (patternList[i]) {

      /* ── ВПЕРЁД ── */
      case 0x32:      // 110010
        moveForward(3000); break;
      case 0x31:      // 110001
        moveForward(2000); break;
      case 0x30:      // 110000
        moveForward(1000); break;

      /* ── НАЛЕВО ── */
      case 0x36:      // 110110
        turnLeft(3000);  break;
      case 0x35:      // 110101
        turnLeft(2000);  break;
      case 0x34:      // 110100
        turnLeft(1000);  break;

      /* ── НАЗАД ── */
      case 0x3E:      // 111110
        moveBackward(3000); break;
      case 0x3D:      // 111101
        moveBackward(2000); break;
      case 0x3C:      // 111100
        moveBackward(1000); break;

      /* ── НАПРАВО ── */
      case 0x3A:      // 111010
        turnRight(3000); break;
      case 0x39:      // 111001
        turnRight(2000); break;
      case 0x38:      // 111000
        turnRight(1000); break;

      default:                        // неизвестный код
        Serial.print(F("Неизвестный шаблон: 0x"));
        Serial.println(patternList[i], HEX);
        break;
    }
    delay(200);                       // пауза между командами
  }

  digitalWrite(LED_PIN, HIGH);        // индикатор — «готов»
}
