#include    <panel-2.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Panel_2::Panel_2(size_t input_wires_num,
                 size_t output_wires_num,
                 QObject *parent)
    : ElectricModule(input_wires_num, output_wires_num, parent)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Panel_2::~Panel_2()
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Panel_2::step(double t, double dt)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Panel_2::init(const QString &custom_cfg_dir)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Panel_2::preStep(state_vector_t &Y, double t)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Panel_2::ode_system(const state_vector_t &Y, state_vector_t &dYdt, double t)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Panel_2::load_config(CfgReader &cfg)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void Panel_2::stepKeysControl(double t, double dt)
{

}
