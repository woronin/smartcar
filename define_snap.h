#ifndef DEFINESPAP_H
#define DEFINESNAP_H


//table dla razbora file SNAP

#define NUM_COMM_SNAP 8
// всегда первый байт
#define SM1 "receiveGo"
#define SM1_16 "8E"

//# вперед
#define SM2 "forward"
#define SM2_16 "00"

//# начало цикла
#define SM3 "doRepeat"
#define SM3_16 "03"

//# влево
#define SM4 "turnLeft"
#define SM4_16 "04"

//# сигнал звук
#define SM5 "playSound"
#define SM5_16 "07"

//# вправо
#define SM6 "turn"
#define SM6_16 "08"

//# конец цикла
#define SM7 "/doRepeat"
#define SM7_16 "0B"

//# назад

//# фонарик
#define SM8 "setBrightnes"
#define SM8_16 "0F"


#endif
