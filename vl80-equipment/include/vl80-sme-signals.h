#ifndef     VL80S_SME_SIGNALS_H
#define     VL80S_SME_SIGNALS_H

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
enum
{
    /// Количество сигналов в соединениях СМЕ
    NUM_VL80_SME_SIGNALS = 100,

    SME_OUTPUT_REF_STATE = 0,   ///< Сигнал управления соединением СМЕ
    SME_INPUT_IS_CONNECTED = 0, ///< Сигнал состояния соединения СМЕ

    SME_NUM_SECTIONS = 1,       ///< Сигнал-счётчик секций, чтобы не позволять более четырёх

    // Cигналы с ведущей секции
    SME_NO_REVERS_HANDLE = 2,   ///< Сигнал запрета реверсивки в других кабинах
    SME_NO_BRAKELOCK_KEY = 3,   ///< Сигнал запрета разблокировки УБТ в других кабинах

    // TODO // всё остальное взаимодействие секций вл80

    // Цепи управления токеоприемниками
    SME_E15_IN,
    SME_E15_OUT,

    SME_E16_IN,
    SME_E16_OUT,

    SME_E17_IN,
    SME_E17_OUT,

    SME_E35_IN,
    SME_E35_OUT,

    SME_E27_IN,
    SME_E27_OUT,

    SME_E28_IN,
    SME_E28_OUT,

    SME_E37_IN,
    SME_E37_OUT,

    SME_E30_IN,
    SME_E30_OUT,
};

#endif // VL80S_SME_SIGNALS_H
