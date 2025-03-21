#include    "vl80s.h"

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
void VL80s::initPneumoSupply(const QString &modules_dir, const QString &custom_cfg_dir)
{
    (void) modules_dir;
    (void) custom_cfg_dir;

    // Мотор-компрессор
    press_reg = new PressureRegulator();
    press_reg->read_config("pressure-regulator");

    // Мотор-компрессор
    motor_compressor = new ACMotorCompressor();
    motor_compressor->read_config("motor-compressor-ac");

    // Главный резервуар и питательная магистраль
    main_reservoir = new Reservoir(main_reservoir_volume);
    main_reservoir->setLeakCoeff(main_reservoir_leak_flow);

    // Концевые краны питательной магистрали
    anglecock_fl_fwd = new PneumoAngleCock();
    anglecock_fl_fwd->setKeyCode(KEY_F6);
    anglecock_fl_fwd->read_config("pneumo-anglecock-FL");

    anglecock_fl_bwd = new PneumoAngleCock();
    anglecock_fl_bwd->setKeyCode(KEY_F7);
    anglecock_fl_bwd->read_config("pneumo-anglecock-FL");

    // Рукава питательной магистрали
    hose_fl_fwd = new PneumoHose();
    hose_fl_fwd->setKeyCode(KEY_F5);
    hose_fl_fwd->read_config("pneumo-hose-FL");
    forward_connectors.push_back(hose_fl_fwd);

    hose_fl_bwd = new PneumoHose();
    hose_fl_bwd->setKeyCode(KEY_F8);
    hose_fl_bwd->read_config("pneumo-hose-FL");
    backward_connectors.push_back(hose_fl_bwd);
}
