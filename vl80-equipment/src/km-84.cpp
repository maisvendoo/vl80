#include    <km-84.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
ControllerKM84::ControllerKM84(size_t input_wires_num,
                               size_t output_wires_num,
                               QObject *parent)
    : ElectricModule(input_wires_num, output_wires_num, parent)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
ControllerKM84::~ControllerKM84()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM84::init(const QString &custom_cfg_dir)
{
    // Инициализируем развертку главного вала
    init_main_shaft();

    // Инициализируем развертку реверсивного вала
    init_revers_shaft();

    // Инициализитуем развертку тормозного вала
    init_brake_shaft();

    selsyn->read_config("BD-404A", custom_cfg_dir);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
float ControllerKM84::getMainHandlePos() const
{
    if (main_pos < POS_0)
    {
        return static_cast<float>(main_pos - 2) / 2.0f;
    }

    return static_cast<float>(main_pos - 2) / 5.0f;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
float ControllerKM84::getReversHandlePos() const
{
    if (revers_pos < 0)
    {
        return static_cast<float>(revers_pos - 2);
    }

    return static_cast<float>(revers_pos - 1) / 4.0f;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
float ControllerKM84::getBrakeHandlePos() const
{
    return static_cast<float>(brake_shaft_angle / brake_shaft_angle_max);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
sound_state_t ControllerKM84::getSoundState(size_t idx) const
{
    if (idx < sounds.size())
    {
        return sounds[idx];
    }

    return Device::getSoundState();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
float ControllerKM84::getSoundSignal(size_t idx) const
{
    if (idx < sounds.size())
    {
        return sounds[idx].createSoundSignal();
    }

    return Device::getSoundSignal();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM84::step(double t, double dt)
{
    ElectricModule::step(t, dt);

    main_shaft_timer->step(t, dt);
    revers_shaft_timer->step(t, dt);
    brake_shaft_timer->step(t, dt);

    selsyn->step(t, dt);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM84::preStep(state_vector_t &Y, double t)
{
    // Цепи главного вала
    output_wire[N2] =
        input_wire[N1] *
        static_cast<double>(main_shaft_state[K_69_70][main_pos]) *
        static_cast<double>(main_shaft_state[K_63_64][main_pos]);

    output_wire[N415] =
        output_wire[N414] *
        static_cast<double>(main_shaft_state[K_65_66][main_pos]);

    output_wire[E115] =
        output_wire[N414] *
        static_cast<double>(main_shaft_state[K_67_68][main_pos]);

    output_wire[E8] =
        input_wire[N04] *
        static_cast<double>(main_shaft_state[K_51_52][main_pos]);

    output_wire[E10] =
        input_wire[N04] *
        static_cast<double>(main_shaft_state[K_55_56][main_pos]);

    output_wire[E11] =
        input_wire[N04] *
        static_cast<double>(main_shaft_state[K_57_58][main_pos]);

    output_wire[E12] =
        input_wire[N04] *
        static_cast<double>(main_shaft_state[K_59_60][main_pos]);

    output_wire[E13] =
        input_wire[N88] *
        static_cast<double>(main_shaft_state[K_61_62][main_pos]);

    // Цепи тормозного вала
    output_wire[N302] =
        input_wire[N1] *
        static_cast<double>(brake_shaft_state[K_39_40][brake_pos]);

    output_wire[E56] =
        input_wire[N03] *
        static_cast<double>(brake_shaft_state[K_31_32][brake_pos]);

    output_wire[E60] =
        input_wire[N03] *
        static_cast<double>(brake_shaft_state[K_33_34][brake_pos]);

    output_wire[E59] =
        input_wire[N03] *
        static_cast<double>(brake_shaft_state[K_35_36][brake_pos]);

    // Цепи реверсивного вала
    output_wire[E2] =
        input_wire[N306] *
        static_cast<double>(revers_shaft_state[K_11_12][revers_pos]);

    output_wire[E3] =
        input_wire[N306] *
        static_cast<double>(revers_shaft_state[K_9_10][revers_pos]);

    output_wire[E4] =
        input_wire[N03] *
        static_cast<double>(revers_shaft_state[K_1_2][revers_pos]);

    output_wire[E5] =
        input_wire[N03] *
        static_cast<double>(revers_shaft_state[K_3_4][revers_pos]);

    output_wire[E6] =
        input_wire[N03] *
        static_cast<double>(revers_shaft_state[K_5_6][revers_pos]);

    output_wire[N414] =
        input_wire[N05] *
        static_cast<double>(revers_shaft_state[K_13_14][revers_pos]);

    output_wire[E119] =
        input_wire[N05] *
        static_cast<double>(revers_shaft_state[K_15_16][revers_pos]);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM84::ode_system(const state_vector_t &Y,
                                state_vector_t &dYdt,
                                double t)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM84::load_config(CfgReader &cfg)
{
    QString secName = "Device";

    cfg.getDouble(secName, "SwitchTimeout", switch_timeout);

    main_shaft_timer = new Timer(switch_timeout);
    connect(main_shaft_timer, &Timer::process, this, &ControllerKM84::slotMainShaftUpdate);

    revers_shaft_timer = new Timer(switch_timeout);
    connect(revers_shaft_timer, &Timer::process, this, &ControllerKM84::slotReversShaftUpdate);

    brake_shaft_timer = new Timer(switch_timeout);
    connect(brake_shaft_timer, &Timer::process, this, &ControllerKM84::slotBrakeShaftUpdate);

    cfg.getDouble(secName, "BrakeShaftOmega", brake_shaft_omega);
    cfg.getDouble(secName, "BrakeShaftMaxAngle", brake_shaft_angle_max);

    QDomNode secNode = cfg.getFirstSection("BrakePos");

    while (!secNode.isNull())
    {
        int pos = 0;
        cfg.getInt(secNode, "Pos", pos);
        cfg.getDouble(secNode, "Angle", bs_angles[pos]);

        secNode = cfg.getNextSection();
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM84::stepKeysControl(double t, double dt)
{
    // Главная рукоятка на себя
    if (getKeyState(KEY_A))
    {
        if (!main_shaft_timer->isStarted())
        {
            main_shaft_dir = 1;
            main_shaft_timer->start();
        }
    }
    else
    {
        if (main_shaft_dir > 0)
            main_shaft_timer->stop();

        // Самовозврат из положения АП
        if (main_pos == POS_AP)
        {
            main_shaft_dir = -1;
            slotMainShaftUpdate();
        }
    }

    // Главная рукоятка от себя
    if (getKeyState(KEY_D))
    {
        if (getKeyState(KEY_Control_L) || getKeyState(KEY_Control_R))
        {
            if (main_pos != POS_0)
            {
                main_pos = POS_0;
                sounds[MAIN_CHANGE_POS_SOUND].play();
            }
        }
        else
        {
            if (!main_shaft_timer->isStarted())
            {
                main_shaft_dir = -1;
                main_shaft_timer->start();
            }
        }
    }
    else
    {
        if (main_shaft_dir < 0)
            main_shaft_timer->stop();

        // Самовозврат из положения БВ
        if (main_pos == POS_BV)
        {
            main_shaft_dir = 1;
            slotMainShaftUpdate();
        }
    }

    // Реверсивка от себя
    if (getKeyState(KEY_W))
    {
        if (!revers_shaft_timer->isStarted())
        {
            revers_shaft_dir = 1;
            revers_shaft_timer->start();
        }
    }
    else
    {
        if (revers_shaft_dir > 0)
            revers_shaft_timer->stop();
    }

    // Реверсивка на себя
    if (getKeyState(KEY_S))
    {
        if (!revers_shaft_timer->isStarted())
        {
            revers_shaft_dir = -1;
            revers_shaft_timer->start();
        }
    }
    else
    {
        if (revers_shaft_dir < 0)
            revers_shaft_timer->stop();
    }

    // Тормозная рукоятка на себя
    if (getKeyState(KEY_Q))
    {
        if (!brake_shaft_timer->isStarted())
        {
            brake_shaft_dir = 1;
            brake_shaft_timer->start();
        }
    }
    else
    {
        if (brake_shaft_dir > 0)
            brake_shaft_timer->stop();
    }

    // Тормозная рукоятка от себя
    if (getKeyState(KEY_E))
    {
        if (!brake_shaft_timer->isStarted())
        {
            brake_shaft_dir = -1;
            brake_shaft_timer->start();
        }
    }
    else
    {
        if (brake_shaft_dir < 0)
            brake_shaft_timer->stop();
    }    
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM84::set_shaft_contacts_state(km84_contact_state_t &shaft_state,
                                             bool state)
{
    std::fill(shaft_state.begin(), shaft_state.end(), state);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM84::init_main_shaft()
{
    for (size_t i = 0; i < main_shaft_state.size(); ++i)
    {
        main_shaft_state[i].resize(KM84_Position::NUM_POS);
    }

    set_shaft_contacts_state(main_shaft_state[K_51_52], false);
    main_shaft_state[K_51_52][POS_FP] = true;
    main_shaft_state[K_51_52][POS_RP] = true;
    main_shaft_state[K_51_52][POS_AP] = true;

    set_shaft_contacts_state(main_shaft_state[K_53_54], false);
    main_shaft_state[K_53_54][POS_FP] = true;
    main_shaft_state[K_53_54][POS_RP] = true;
    main_shaft_state[K_53_54][POS_AP] = true;

    set_shaft_contacts_state(main_shaft_state[K_55_56], false);
    main_shaft_state[K_55_56][POS_AV] = true;
    main_shaft_state[K_55_56][POS_RV] = true;
    main_shaft_state[K_55_56][POS_RP] = true;
    main_shaft_state[K_55_56][POS_AP] = true;

    set_shaft_contacts_state(main_shaft_state[K_57_58], false);
    main_shaft_state[K_57_58][POS_AV] = true;
    main_shaft_state[K_57_58][POS_FV] = true;
    main_shaft_state[K_57_58][POS_FP] = true;
    main_shaft_state[K_57_58][POS_AP] = true;

    set_shaft_contacts_state(main_shaft_state[K_59_60], true);
    main_shaft_state[K_59_60][POS_BV] = false;
    main_shaft_state[K_59_60][POS_0] = false;

    set_shaft_contacts_state(main_shaft_state[K_61_62], true);
    main_shaft_state[K_61_62][POS_BV] = false;

    set_shaft_contacts_state(main_shaft_state[K_63_64], true);
    main_shaft_state[K_63_64][POS_BV] = false;
    main_shaft_state[K_63_64][POS_0] = false;

    set_shaft_contacts_state(main_shaft_state[K_65_66], false);
    main_shaft_state[K_65_66][POS_0] = true;

    set_shaft_contacts_state(main_shaft_state[K_67_68], true);
    main_shaft_state[K_67_68][POS_BV] = false;
    main_shaft_state[K_67_68][POS_0] = false;

    set_shaft_contacts_state(main_shaft_state[K_69_70], true);
    main_shaft_state[K_69_70][POS_BV] = false;
    main_shaft_state[K_69_70][POS_0] = false;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM84::init_revers_shaft()
{
    for (size_t i = 0; i < revers_shaft_state.size(); ++i)
    {
        revers_shaft_state[i].resize(REVERS_POS_NUM);
    }

    set_shaft_contacts_state(revers_shaft_state[K_1_2], false);
    revers_shaft_state[K_1_2][REVERS_POS_OP1] = true;
    revers_shaft_state[K_1_2][REVERS_POS_OP2] = true;
    revers_shaft_state[K_1_2][REVERS_POS_OP3] = true;

    set_shaft_contacts_state(revers_shaft_state[K_3_4], false);
    revers_shaft_state[K_3_4][REVERS_POS_OP2] = true;
    revers_shaft_state[K_3_4][REVERS_POS_OP3] = true;

    set_shaft_contacts_state(revers_shaft_state[K_5_6], false);
    revers_shaft_state[K_5_6][REVERS_POS_OP3] = true;

    set_shaft_contacts_state(revers_shaft_state[K_7_8], false);

    set_shaft_contacts_state(revers_shaft_state[K_9_10], false);
    revers_shaft_state[K_9_10][REVERS_POS_BACKWARD] = true;

    set_shaft_contacts_state(revers_shaft_state[K_11_12], true);
    revers_shaft_state[K_11_12][REVERS_POS_BACKWARD] = false;
    revers_shaft_state[K_11_12][REVERS_POS_0] = false;

    set_shaft_contacts_state(revers_shaft_state[K_13_14], true);
    revers_shaft_state[K_13_14][REVERS_POS_0] = false;

    set_shaft_contacts_state(revers_shaft_state[K_15_16], true);
    revers_shaft_state[K_15_16][REVERS_POS_0] = false;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM84::init_brake_shaft()
{
    for (size_t i = 0; i < brake_shaft_state.size(); ++i)
    {
        brake_shaft_state[i].resize(BRAKE_NUM_POS);
    }

    set_shaft_contacts_state(brake_shaft_state[K_31_32], true);
    brake_shaft_state[K_31_32][BRAKE_POS_0] = false;

    set_shaft_contacts_state(brake_shaft_state[K_33_34], false);
    brake_shaft_state[K_33_34][BRAKE_POS_P] = true;

    set_shaft_contacts_state(brake_shaft_state[K_35_36], true);
    brake_shaft_state[K_35_36][BRAKE_POS_0] = false;
    brake_shaft_state[K_35_36][BRAKE_POS_P] = false;

    set_shaft_contacts_state(brake_shaft_state[K_37_38], true);
    brake_shaft_state[K_37_38][BRAKE_POS_0] = false;
    brake_shaft_state[K_37_38][BRAKE_POS_PT] = false;

    set_shaft_contacts_state(brake_shaft_state[K_39_40], true);
    brake_shaft_state[K_39_40][BRAKE_POS_0] = false;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM84::slotMainShaftUpdate()
{
    // Блокировка поворота главной рукоятки при нулевом положении реверсивной
    if (revers_pos == REVERS_POS_0)
    {
        return;
    }

    // Блокировка поворота главной рукоятки при ненулевом положении тормозной
    if (brake_pos != BRAKE_POS_0)
    {
        return;
    }

    int main_pos_old = main_pos;

    main_pos += main_shaft_dir;

    main_pos = cut(main_pos, static_cast<int>(POS_BV), static_cast<int>(POS_AP));

    if (main_pos != main_pos_old)
    {
        sounds[MAIN_CHANGE_POS_SOUND].play();
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM84::slotReversShaftUpdate()
{
    // Исключение постановки реверса в 0 при ненулевом положении главной
    // и тормозной рукояток
    if (revers_shaft_dir > 0)
    {
        if ( (revers_pos == REVERS_POS_BACKWARD) && ( (main_pos != POS_0) || (brake_pos != BRAKE_POS_0) ) )
        {
            return;
        }
    }

    if (revers_shaft_dir < 0)
    {
        if ( (revers_pos == REVERS_POS_FORWARD) && ( (main_pos != POS_0) || (brake_pos != BRAKE_POS_0) ) )
        {
            return;
        }
    }

    // Блокировка положений ОП1 - ОП3 при ненулевом положении тормозной рукоятки
    if ( ( (revers_pos == REVERS_POS_FORWARD) || (revers_pos == REVERS_POS_BACKWARD) ) && (brake_pos != BRAKE_POS_0) )
    {
        return;
    }

    int revers_pos_old = revers_pos;

    revers_pos += revers_shaft_dir;

    revers_pos = cut(revers_pos,
                     static_cast<int>(REVERS_POS_BACKWARD),
                     static_cast<int>(REVERS_POS_OP3));

    if (revers_pos != revers_pos_old)
    {
        sounds[REVERS_CHANGE_POS_SOUND].play();
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void ControllerKM84::slotBrakeShaftUpdate()
{
    // Блокировка поворота тормозной рукоятки при нулевом положении реверсивной
    if (revers_pos == REVERS_POS_0)
    {
        return;
    }

    // Блокировка поворота при ненулевом положении главной рукоятки
    if (main_pos != POS_0)
    {
        return;
    }

    int brake_pos_old = brake_pos;

    // Переключение позиции только если вал не находися в зоне
    // работы задатчика скорости
    if (brake_shaft_angle <= bs_angles[BRAKE_POS_T])
    {
        brake_pos += brake_shaft_dir;
    }

    brake_pos = cut(brake_pos,
                    static_cast<int>(BRAKE_POS_0),
                    static_cast<int>(BRAKE_POS_T));

    // До позиции "Т" поворачиваем вал дискретно
    if (brake_pos != BRAKE_POS_T)
    {
        brake_shaft_angle = bs_angles[brake_pos];
    }
    else // При "Т" - непрерывно
    {
        brake_shaft_angle += brake_shaft_omega * brake_shaft_dir * switch_timeout;

        // Недопускаем "залипание" рукоятки в положении "Т"
        // при возврате в дискретную зону
        if (brake_shaft_angle < bs_angles[BRAKE_POS_T])
        {
            brake_pos += brake_shaft_dir;
            brake_shaft_angle = bs_angles[brake_pos];
        }
        else
        {
            brake_shaft_angle = cut(brake_shaft_angle,
                                    bs_angles[BRAKE_POS_T],
                                    brake_shaft_angle_max);
        }
    }

    if (brake_pos != brake_pos_old)
    {
        sounds[BRAKE_CHANGE_POS_SOUND].play();
    }
}
