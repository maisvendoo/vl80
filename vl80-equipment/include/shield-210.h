#ifndef     SHIELD_210_H
#define     SHIELD_210_H

#include    <electric-module.h>
#include    <relay.h>
#include    <trigger.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
enum S210_InputWires
{
    S210_INPUTS_NUM = 3,
    /// Вход питания от АКБ
    S210_N113 = 0,
    /// Вход питания ЦУ от 2 секции
    S210_E62 = 1,
    /// Вход питания ЦУ от 2 секции
    S210_E66 = 2
};

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
enum S210_OutputtWires
{
    S210_OUTPUT_NUM = 9,
    S210_N119 = 0,
    S210_N0 = 1,
    S210_N49 = 2,
    S210_N401 = 3,
    S210_N402 = 4,
    S210_N66 = 5,
    /// Выход питания ЦУ на 2 секцию
    S210_E61 = 6,
    /// Выход питания ЦУ на 2 секцию
    S210_E65 = 7,
    S210_N116 = 8
};

/*!
 * \class
 * \brief Щиток 210 (распределительный щит)
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class Shield_210 : public ElectricModule
{
public:

    Shield_210(size_t input_wires_num = 1,
               size_t output_wires_num = 1,
               QObject *parent = Q_NULLPTR);

    ~Shield_210();

    void init(const QString &custom_cfg_dir);

    void step(double t, double dt) override;

    /// Задать напряжение питания от обмотки собственных нужд
    void setVoltageAC(double Uac)
    {
        this->Uac = Uac;
    }


    /// Задать ток нагрузки
    void setLoadCurrent(double I_load)
    {
        this->I_load = I_load;
    }

private:

    /// Рубильник подключения батареи
    Trigger switch_2R;

    /// Рубильник режима работы (нормально/аварийно)
    Trigger switch_3R;

    enum
    {
        K_NUM_CONTACTS = 3,
        K_9_21 = 0,
        K_31_32 = 1,
        K_16_17 = 2
    };

    /// Контактор питания от батареи
    Relay *K = new Relay(K_NUM_CONTACTS);

    /// Ток, потребляемый нагрузкой
    double I_load = 0.0;

    /// Эквивалентное внутреннее сопротивление выпрямителя
    double r = 0.03;

    /// Коэффициент трансформации выпрямителя
    double K_rect = 0.18;

    /// Напряжение на выходе выпрямителя
    double U_rect = 0.0;

    /// Коэффициент передачи регулятора напряжения
    double K_u = 1.0;

    /// Уставка напряжения питания ЦУ и заряда АКБ
    double U_ref = 60.0;

    /// Напряжение питания от обмотки собственных нужд тягового трансформатора
    double Uac = 0.0;

    void preStep(state_vector_t &Y, double t) override;

    void ode_system(const state_vector_t &Y,
                    state_vector_t &dYdt,
                    double t) override;

    void load_config(CfgReader &cfg) override;

    void stepKeysControl(double t, double dt) override;
};


#endif // SHIELD_210_H
