#ifndef		VL80S_H
#define		VL80S_H

#include    <vehicle-api.h>
#include    <km-84.h>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class VL80s : public Vehicle
{
public:

    VL80s(QObject *parent = Q_NULLPTR);

    ~VL80s();

    /// Инициализация тормозных приборов
    void initBrakeDevices(double p0, double pBP, double pFL) override;

private:

    /// Чтение дополнительных параметров из конфига
    void loadConfig(QString cfg_path) override;

    // Параметры:
    /// Передаточное число зубчатой передачи тяговых электродвигателей
    double  ip = 4.19;

    /// Имя модуля сцепного устройства
    QString coupling_module_name = "sa3";
    /// Имя конфига сцепного устройства
    QString coupling_config_name = "sa3";

    /// Объём главных резервуаров
    double main_reservoir_volume = 900.0;
    /// Темп утечек из главных резервуаров
    double main_reservoir_leak_flow = 1.0e-6;

    /// Начальное состояние блокировки тормозов
    bool brake_lock_init_state = 0;

    /// Начальное положение комбинированного крана
    int combine_crane_init_pos = 0;

    /// Имя модуля поездного крана
    QString brake_crane_module_name = "krm395";
    /// Имя конфига поездного крана
    QString brake_crane_config_name = "krm395";
    /// Начальное положение поездного крана
    int brake_crane_init_pos = 6;

    /// Имя модуля локомотивного крана
    QString loco_crane_module_name = "kvt254";
    /// Имя конфига локомотивного крана
    QString loco_crane_config_name = "kvt254";
    /// Начальное положение поездного крана
    double loco_crane_init_pos = 1.0;

    /// Имя модуля воздухораспределителя
    QString airdist_module_name = "vr483";
    /// Имя конфига воздухорапределителя
    QString airdist_config_name = "vr483";

    // Оборудование:
    /// Мотор-компрессор
    ACMotorCompressor *motor_compressor = nullptr;

    /// Регулятор давления в ГР
    PressureRegulator *press_reg = nullptr;

    /// Главный резервуар
    Reservoir   *main_reservoir = nullptr;

    /// Контроллер машиниста КМ-84
    ControllerKM84 *km = new ControllerKM84;

    // Инициализация:
    /// Инициализация подсистем секции электровоза
    void initialization() override;

    /// Инициализация питательной магистрали
    void initPneumoSupply(const QString &modules_dir, const QString &custom_cfg_dir);

    // Симуляция:
    /// Шаг симуляции всех систем электровоза
    void step(double t, double dt) override;

    /// Моделирование питательной магистрали
    void stepPneumoSupply(double t, double dt);

    /// Отладочный вывод
    void stepDebugPrint(double t, double dt);
};

#endif // VL80S_H
