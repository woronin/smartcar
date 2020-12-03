#ifndef DEFINEJSON_H
#define DEFINEJSON_H


//table dla razbora file json

#define NUM_COMM_JSON 17
// всегда первый байт
#define CM1 "[[\"whenGreenFlag\"]"
#define CM1_16 "8E"
#define CM2 "when @greenFlag clicked"
#define CM2_16 "8E"
//# вперед
#define CM3 "move"
#define CM3_16 "00"
#define CM4 "[\"forward:\""
#define CM4_16 "00"
//# начало цикла
#define CM5 "repeat"
#define CM5_16 "03"
//# влево
#define CM6 "[\"heading:\", -90], [\"forward:\", 5]"
#define CM6_16 "04"
#define CM7 "turn @turnLeft"
#define CM7_16 "04"
#define CM8 "[\"turnLeft:\","
#define CM8_16 "04"
//# сигнал звук
#define CM9 "[\"playSound:\","
#define CM9_16 "07"
//# вправо
#define CM10 "[\"heading:\""
#define CM10_16 "08"
#define CM11 "turn @turnRight"
#define CM11_16 "08"
#define CM12 "[\"turnRight:\","
#define CM12_16 "08"
//# конец цикла
#define CM13 "end"
#define CM13_16 "0B"
//# назад
#define CM14 "[\"heading:\", 180], [\"forward:\", 5]"
#define CM14_16 "0C"
#define CM15 "[\"heading:\", 180], [\"forward:\", 15]"
#define CM15_16 "0D"
#define CM16 "play sound"
#define CM16_16 "07"
#define CM17 "pen down"
#define CM17_16 "0F"


#endif
