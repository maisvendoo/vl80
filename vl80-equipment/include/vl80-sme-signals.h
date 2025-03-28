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
};

#endif // VL80S_SME_SIGNALS_H
