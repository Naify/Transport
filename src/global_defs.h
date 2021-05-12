#ifndef GLOBALS_H
#define GLOBALS_H

#include <OdsInterface>

bool setIObjRights(const ODS::IObject &iobj);

#define DATE_FORMAT_PG QString("yyyy-MM-dd")
#define DATE_FORMAT QString("dd.MM.yyyy")
#define TIME_FORMAT QString("hh:mm")

#define USER_GROUP_NAME QString("Пользователи Контроль перевозок")
#define ADMIN_GROUP_NAME QString ("Администраторы Контроль перевозок")

#define MY_APP_NAME QString("Контроль перевозок - учет")

#define TECH_TRANSIT_REGISTRATION QString("Учет перевозок спецтехники")
#define DOCS_TRANSIT QString("Руководящие документы по перевозкам спецтехники")
#define ADD_CONTENT QString("Добавление содержания")

#define ORDER_RECIEVE_SEND_TECH QString("Распоряжение")
#define CREATE_SHUTTLE_TASK QString("Дорожка/Марш. задание")
#define ESCORT_MENU QString("Команда сопровождения")
#define TRANSPORT_MENU QString("Транспортные ср-ва")
#define CARGO_MENU QString("Груз")
#define DOCS_MENU QString("Орг-план. документы")
#define ORG_MENU QString("Организация работ")
#define ASSIGN_MENU QString("Допуска")
#define RESCALC_MENU QString("Определение сил и средств")

#define CARGO QString("Изделия")
#define PARTS QString("ЗИП")
#define IN_OUT_DOC QString("Прием.-сдат. ведомость")
#define AUTO_TR QString("Состав автоколонны")
#define TRAIN_TR QString("Состав эшелона")
#define ESCORT QString("Состав команды сопровождения")
#define TRAVEL_AUTH QString("Командировочное удостоверение")
#define TRAVEL_AUTH_DOC QString("Командир. удост. документ")
#define TRAIN_COMMANDER_DOC QString("Удост. НЭШ")
#define CARS_COMMANDER_DOC QString("Удост. НАК")
#define TABEL_DOC QString("Табель постам")
#define POSTS_DOC QString("Постовая ведомость")
#define COMMANDER_DOC QString("Приказ командира")
#define ACTS_DOC QString("Акты формы 5ГУ")
#define ORDER_TRANSP_DOC QString("Распоряжение об обеспечении")
#define PLAN_DOC QString("План-график проведения работ")
#define ESCORT_CTRL QString("Учет командировок")
#define SHIPMENT QString("Схема-расчет выдачи")
#define START_ASSIGN QString("Данные по допускам личного состава")
#define PERSONAL_ASSIGN QString("Личный состав")
#define SI_ASSIGN QString("Допуск к эксплуатации СИ")
#define MED_ASSIGN QString("Данные медицинских обследований (ВВК)")
#define AGROUP_ASSIGN QString("Данные аттестации персонала группы \"А\"")
#define ZDE_ASSIGN QString("Начальник ЖДЭ")
#define AK_ASSIGN QString("Начальник АК")
#define SOPR_ASSIGN QString("Сопровождающие")
#define ZNK_ASSIGN QString("Начальник караула")
#define GUARD_ASSIGN QString("Караульные")
#define KRAN_ASSIGN QString("Крановщики")
#define DAMAGEDSI_ASSIGN QString("Допуск к обращению с аварийными СИ")
#define TECHLPA_ASSIGN QString("Допуск к техническим средствам ЛПА")
#define START_RESCALC QString("Определение сил и средств")
#define RAILS QString("Учет спец вагонов")
#define CARS QString("Учет спец автомобилей")
#define TZUSK QString("Учет ТЗУ и СК")
#define ENGTECH QString("Учет инженерной техники")
#define SUPPMACHINES QString("Учет машин обеспечения")
#define CALCRAILS QString("Определение количества вагонов")
#define CALCPEOPLE QString("Определение состава команды сопровождения")
#define CALCWEAPONS QString("Определение вооружения и материальных средств")
#define CLAIM QString("Заявка на специальную перевозку")

#define JOURNAL_TYPE QString("Контроль перевозок.Распоряжения")

#define DATE_ATTR QString("Дата")
#define NUMBER_ATTR QString("Номер")
#define CONTENT_ATTR QString("Содержание")

#define ORDER_TYPE QString("Контроль перевозок.Содержание")

#define TYPE_TRANSPORT_ATTR QString("Вид транспорта")
#define RESPONSIBLE_ATTR QString("Ответственный")
#define SPECIAL_ORDERS_ATTR QString("Особые указания")

#define ORDER_TABLE_TYPE QString("Контроль перевозок.Содержание таблица")

#define NUMBER_P_P_ATTR QString("№№ п/п")
#define SENDER_ATTR QString("Отправитель")
#define RECEIVER_ATTR QString("Получатель")
#define DATE_SEND_ATTR QString("Дата отправки")
#define CARGO_NAME_ATTR QString("Наименование груза")
#define AMOUNT_ATTR QString("Кол-во")
#define READYNESS_STATUS_ATTR QString("СГ")
#define BLOCK_ATTR QString("Блокировка")
#define ORDER_ATTR QString("Распоряжение")

#define CODE_ATTR QString("Код")
#define NAME_ATTR QString("Наименование")
#define ENCRYPT_PACK_ATTR QString("Шифр тары")

#define DISLOCATION_ATTR QString("Дислокация")
#define COORDINATES_ATTR QString("Координаты")
#define SHORT_NAME_ATTR QString("Кратк наименование")
#define FULL_NAME_ATTR QString("Полн наименование")

#define CARGO_TYPE QString("Контроль перевозок.Изделие")

#define NOMENKLAT_ATTR QString("Номенклатура")
#define ZAV_NUMB_ATTR QString("Зав. ном-р")
#define BLOCK2_ATTR QString("Блок")
#define PLOMB_ATTR QString("Пломба тары")
#define TZU_ATTR QString("Тип ТЗУ")
#define ZAV_TZU_ATTR QString("Зав. ном-р ТЗУ")
#define PLOMB_TZU_ATTR QString("Пломба ТЗУ")
#define WAGON_TYPE_ATTR QString("Тип вагона")
#define WAGON_NUMB_ATTR QString("Номер вагона")
#define WAGON_PLOMB_ATTR QString("Пломба вагона")

#endif // GLOBALS_H
