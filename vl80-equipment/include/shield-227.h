#ifndef     SHIELD_227_H
#define     SHIELD_227_H

#include    <electric-module.h>
#include    <trigger.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class Shield_227 : public ElectricModule
{
public:

    Shield_227(size_t input_wires_num = 1,
               size_t output_wires_num = 1,
               QObject *parent = Q_NULLPTR);

    ~Shield_227();

    enum
    {
        TUMBLERS_NUM = 7,
        /// Без фазоращепителя
        TUMBLER_WITHOUT_PHASE_SPLITTER = 0,
        /// Фазоращепитель
        TUMBLER_PHASE_SPLITTER = 1,
        /// Вентилятор 1
        TUMBLER_FAN1 = 2,
        /// Вентилятор 3
        TUMBLER_FAN3 = 3,
        /// Маслонасос
        TUMBLER_OILPUMP = 4,
        /// Низкая температура масла
        TUMBLER_LOW_OIL_TEMPERATURE = 5,
        /// Компрессор токоприемника
        TUMBLER_AUX_COMPRESSOR = 6
    };

    enum S227_InputWires
    {
        INPUTS_NUM = 7,
        E18 = 0,
        N511 = 1,
        N513 = 2,
        N505 = 3,
        N010 = 4,
        N101 = 5,
        N66 = 6
    };

    enum S227_OutputWires
    {
        OUTPUTS_NUM = 7,
        N169 = 0,
        N127 = 1,
        N131 = 2,
        N121 = 3,
        N426 = 4,
        N103 = 5,
        N67 = 6
    };

    /// Вернуть положение тумблеров
    float getTumblerPos(size_t idx) const
    {
        if (idx >= tumbler.size())
        {
            return 0.0f;
        }

        return static_cast<float>(tumbler[idx].getState());
    }

private:

    /// Тумблеры на щитке
    std::array<Trigger, TUMBLERS_NUM> tumbler;

    /// Механическая блокировка одновременного включения тумблеров
    /// "Без фазоращепителя и "Фазоращепитель"
    Trigger phase_splitter_mode_switch_lock;

    void preStep(state_vector_t &Y, double t) override;

    void ode_system(const state_vector_t &Y,
                    state_vector_t &dYdt,
                    double t) override;

    void load_config(CfgReader &cfg) override;

    void stepKeysControl(double t, double dt) override;
};

#endif // SHIELD_226_H
