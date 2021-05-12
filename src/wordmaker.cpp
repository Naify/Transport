#include "wordmaker.h"
#include <OdsTemplateManager>
#include "mainwindow.h"
#include "global_defs.h"
#include <QAction>
#include <QFileDialog>
#include <CatalogManager.h>
#include <QMessageBox>
#include "Planner.h"
#include "Condition.h"
#include "Contact.h"

WordMaker::WordMaker()
{    
}

WordMaker::~WordMaker()
{

}

void WordMaker::makeDoc(const QString &docName, int current_vc_id)
{
    OdsTemplateManager tmpMan = OdsTemplateManager(ODS::OdsInterface::self());
    bool ok = false;
    OdsTemplate templ;
    if (docName == COMMANDER_DOC) {
        constructCommander(templ, ok, current_vc_id);
    } else if (docName == ORDER_TRANSP_DOC) {
        templ = tmpMan.getTemplate("2_Распоряжение_об_обеспечении", &ok);
    } else if (docName == ACTS_DOC) {
         constructActs (templ, ok, current_vc_id);
    } else if (docName == IN_OUT_DOC) {
        constructinOutDoc(templ, ok, current_vc_id);
    } else if (docName == TABEL_DOC) {
        constructTabel(templ, ok, current_vc_id);
    } else if (docName == POSTS_DOC) {
        templ = tmpMan.getTemplate("7_Постовая_ведомость", &ok);
    } else if (docName == CARS_COMMANDER_DOC) {
        templ = tmpMan.getTemplate("Удостоверение НАК пуст", &ok);
//        constructCarsCommander(templ, ok, current_vc_id);
    } else if (docName == TRAVEL_AUTH_DOC) {
        constructComAuth(templ, ok, current_vc_id, docName);        
    } else if (docName == TRAIN_COMMANDER_DOC){
        templ = tmpMan.getTemplate("Удостоверение НЭШ пуст", &ok);
//        constructTrainCommander(templ, ok, current_vc_id);
    } else if (docName == PLAN_DOC){
        openPlan();
    }

    if (!ok) {
        return;
    }

    saveAsDoc(templ, docName);
}

OdsTemplate WordMaker::constructCommander(OdsTemplate& templ, bool& ok, int current_vc_id) {

    OdsTemplateManager tmpMan = OdsTemplateManager(ODS::OdsInterface::self());
    templ = tmpMan.getTemplate("1_Приказ_командира", &ok);

    if (!ok)
    {
        QMessageBox::warning(0,"Внимание","Не выполнена загрузка шаблона: "+COMMANDER_DOC);
        emit(msgToStatus("Не выполнена загрузка шаблона: "+COMMANDER_DOC));
        ok = false;
        return OdsTemplate();
    }

    if (!templ.isValid())
    {
        QMessageBox::warning(0,"Внимание","Недействительный шаблон: "+COMMANDER_DOC);
        emit(msgToStatus("Недействительный шаблон: "+COMMANDER_DOC));
        ok = false;
        return OdsTemplate();
    }

//    QString querySQL = "SELECT \"Контроль перевозок\".\"Распоряжения\".\"io_id\", "
//            "\"Информационно-справочная подсистема\".\"Классификатор грузов\".\"Наименование\" as \"Наименование груза\", "
//            "\"Контроль перевозок\".\"Содержание таблица\".\"Кол-во\" as \"Кол-во\", "
//            "\"Части1\".\"Условное наименование\" as \"Отправитель\", "
//            "\"Части2\".\"Условное наименование\" as \"Получатель\", "
//            "\"Контроль перевозок\".\"Содержание таблица\".\"Дата отправки\" as \"Дата отправки\", "
//            "\"Контроль перевозок\".\"Содержание\".\"Вид транспорта\" as \"Вид транспорта\", "
//            "\"Части3\".\"Условное наименование\" as \"Ответственный\" "
//            "FROM \"Контроль перевозок\".\"Содержание таблица\" "
//            "INNER JOIN \"Информационно-справочная подсистема\".\"Классификатор грузов\" "
//            "ON \"Информационно-справочная подсистема\".\"Классификатор грузов\".\"io_id\" = \"Контроль перевозок\".\"Содержание таблица\".\"Наименование груза\" "
//            "INNER JOIN \"Информационно-справочная подсистема\".\"Классификатор воинских частей\" as \"Части1\" "
//            "ON \"Части1\".\"io_id\" = \"Контроль перевозок\".\"Содержание таблица\".\"Отправитель\" "
//            "INNER JOIN \"Информационно-справочная подсистема\".\"Классификатор воинских частей\" as \"Части2\" "
//            "ON \"Части2\".\"io_id\" = \"Контроль перевозок\".\"Содержание таблица\".\"Получатель\" "
//            "INNER JOIN \"Контроль перевозок\".\"Распоряжения\" "
//            "ON \"Контроль перевозок\".\"Распоряжения\".\"io_id\" = \"Контроль перевозок\".\"Содержание таблица\".\"Распоряжение\" "
//            "INNER JOIN \"Контроль перевозок\".\"Содержание\" "
//            "ON \"Контроль перевозок\".\"Распоряжения\".\"Содержание\" = \"Контроль перевозок\".\"Содержание\".\"io_id\" "
//            "INNER JOIN \"Информационно-справочная подсистема\".\"Классификатор воинских частей\" as \"Части3\" "
//            "ON \"Части3\".\"io_id\" = \"Контроль перевозок\".\"Содержание\".\"Ответственный\" "
//            "WHERE \"Контроль перевозок\".\"Распоряжения\".\"io_id\" = "+QString::number(current_vc_id);

    QString querySQL = "SELECT \"Контроль перевозок\".\"Распоряжения\".\"io_id\", "
            "\"Контроль перевозок\".\"Содержание таблица\".\"Наименование груза\" as \"Наименование груза\", "
            "\"Контроль перевозок\".\"Содержание таблица\".\"Кол-во\" as \"Кол-во\", "
            "\"Контроль перевозок\".\"Содержание таблица\".\"Отправитель\" as \"Отправитель\", "
            "\"Контроль перевозок\".\"Содержание таблица\".\"Получатель\" as \"Получатель\", "
            "\"Контроль перевозок\".\"Содержание таблица\".\"Дата отправки\" as \"Дата отправки\", "
            "\"Контроль перевозок\".\"Содержание\".\"Вид транспорта\" as \"Вид транспорта\", "
            "\"Контроль перевозок\".\"Содержание\".\"Ответственный\" as \"Ответственный\" "
            "FROM \"Контроль перевозок\".\"Содержание таблица\" "
            "INNER JOIN \"Контроль перевозок\".\"Распоряжения\" "
            "ON \"Контроль перевозок\".\"Распоряжения\".\"io_id\" = \"Контроль перевозок\".\"Содержание таблица\".\"Распоряжение\" "
            "INNER JOIN \"Контроль перевозок\".\"Содержание\" "
            "ON \"Контроль перевозок\".\"Распоряжения\".\"Содержание\" = \"Контроль перевозок\".\"Содержание\".\"io_id\" "
            "WHERE \"Контроль перевозок\".\"Распоряжения\".\"io_id\" = "+QString::number(current_vc_id);

    ODS::IORecordCursor rec_cursor = ODS::OdsInterface::self()->iobjectManager().exec(querySQL);

    WordTable wordTable(0, 7);

    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalogVc = catalMgr.getCatalogByName("Классификатор воинских частей");
    IRS::Catalog::ptr catalogCargo = catalMgr.getCatalogByName("Классификатор грузов");

    catalogCargo->createCache();

    if (rec_cursor.size() == 0){
        QMessageBox::warning(0,"Внимание","Отсутствуют данные содержания распоряжения.\nЗаполните форму \"Распоряжение\"");
        emit(msgToStatus("Отсутствуют данные содержания распоряжения"));
        ok = false;
        return OdsTemplate();
    }

    for (ODS::IORecordCursor::iterator It = rec_cursor.begin(); It != rec_cursor.end(); ++It){
        ODS::Record iobj = (*It);

        int entryId = iobj.getIntAttr("Наименование груза");
        IRS::CatalogEntry::ptr entry = catalogCargo->entryById(entryId);
        QString cargoName;
        if (!entry.isNull()){
            cargoName = entry->stringValue("Наименование");
            if (cargoName == ""){
                QMessageBox::warning(0,"Внимание","Отсутствует наименование груза.\nЗаполните форму \"Распоряжение\"");
                emit(msgToStatus("Отсутствует наименование груза"));
                ok = false;
                return OdsTemplate();
            }
        } else {
            emit(msgToStatus("Получена недействительная запись из Классификатора грузов"));
            ok = false;
            return OdsTemplate();
        }

        entryId = iobj.getIntAttr("Отправитель");
        entry = catalogVc->entryById(entryId);
        QString senderName;
        if (!entry.isNull()){
            senderName= entry->stringValue("Условное наименование");
            if (senderName == ""){
                QMessageBox::warning(0,"Внимание","Отсутствует отправитель груза.\nЗаполните форму \"Распоряжение\"");
                emit(msgToStatus("Отсутствует отправитель груза"));
                ok = false;
                return OdsTemplate();
            }
        } else {
            emit(msgToStatus("Получена недействительная запись из Классификатора грузов"));
            ok = false;
            return OdsTemplate();
        }

        entryId = iobj.getIntAttr("Получатель");
        entry = catalogVc->entryById(entryId);
        QString receiverName;
        if (!entry.isNull()){
            receiverName = entry->stringValue("Условное наименование");
            if (receiverName == ""){
                QMessageBox::warning(0,"Внимание","Отсутствует получатель груза.\nЗаполните форму \"Распоряжение\"");
                emit(msgToStatus("Отсутствует получатель груза"));
                ok = false;
                return OdsTemplate();
            }
        } else {
            emit(msgToStatus("Получена недействительная запись из Классификатора грузов"));
            ok = false;
            return OdsTemplate();
        }

        entryId = iobj.getIntAttr("Ответственный");
        entry = catalogVc->entryById(entryId);
        QString responName;
        if (!entry.isNull()){
            responName = entry->stringValue("Условное наименование");
            if (responName == ""){
                QMessageBox::warning(0,"Внимание","Отсутствует ответственный за груз.\nЗаполните форму \"Распоряжение\"");
                emit(msgToStatus("Отсутствует ответственный за груз"));
                ok = false;
                return OdsTemplate();
            }
        } else {
            emit(msgToStatus("Получена недействительная запись из Классификатора грузов"));
            ok = false;
            return OdsTemplate();
        }

        QString count = iobj.getStringAttr("Кол-во");
        if (count == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует количество груза.\nЗаполните форму \"Распоряжение\"");
            emit(msgToStatus("Отсутствует количество груза"));
            ok = false;
            return OdsTemplate();
        }

        CDate date = iobj.getDateTimeAttr("Дата отправки");
        if (!date.isNull()){
            QMessageBox::warning(0,"Внимание","Отсутствует дата отправки груза.\nЗаполните форму \"Распоряжение\"");
            emit(msgToStatus("Отсутствует дата отправки груза"));
            ok = false;
            return OdsTemplate();
        }

        QString transType = iobj.getStringAttr("Вид транспорта");
        if (transType == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует вид транспорта у распоряжения.\nЗаполните форму \"Распоряжение\"");
            emit(msgToStatus("Отсутствует вид транспорта у распоряжения"));
            ok = false;
            return OdsTemplate();
        }

        wordTable.appendRow();

        wordTable.addText(wordTable.TRCount(), 1, cargoName);
        wordTable.addText(wordTable.TRCount(), 2, count);
        wordTable.addText(wordTable.TRCount(), 3, senderName);
        wordTable.addText(wordTable.TRCount(), 4, receiverName);
        QDateTime dt = QDateTime::fromTime_t(date.getDateTime());
        wordTable.addText(wordTable.TRCount(), 5, dt.date().toString("dd.MM.yy"));
        wordTable.addText(wordTable.TRCount(), 6, transType);
        wordTable.addText(wordTable.TRCount(), 7, responName);
    }

    WordTable wordTable2 (0, 3);

    querySQL = "SELECT \"Транспортные средства\".\"Вагоны\".\"Тип вагона\" as \"Тип вагона\", "
            "\"Транспортные средства\".\"Вагоны\".\"Назначение\" as \"Назначение\", "
            "\"Транспортные средства\".\"Вагоны\".\"Регистр. номер\" as \"Номер вагона\" "
            "FROM \"Транспортные средства\".\"Вагоны\" "
            "WHERE \"Транспортные средства\".\"Вагоны\".\"Распоряжение\" = "+QString::number(current_vc_id);

    rec_cursor = ODS::OdsInterface::self()->iobjectManager().exec(querySQL);

    if (rec_cursor.size() == 0){
        QMessageBox::warning(0,"Внимание","Отсутвуют записи о составе эшелона.\nЗаполните форму \"Состав эшелона\"");
        emit(msgToStatus("Не найдено записи о составе эшелона"));
        ok = false;
        return OdsTemplate();
    }

    for (ODS::IORecordCursor::iterator It = rec_cursor.begin(); It != rec_cursor.end(); ++It){
        ODS::Record iobj = (*It);

        wordTable2.appendRow();

        QString carrigeType = iobj.getStringAttr("Тип вагона");
        if (carrigeType == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует тип вагона.\nЗаполните форму \"Состав эшелона\"");
            emit(msgToStatus("Отсутствует тип вагона"));
            ok = false;
            return OdsTemplate();
        }

        QString destination = iobj.getStringAttr("Назначение");
        if (destination == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует назачение вагона.\nЗаполните форму \"Состав эшелона\"");
            emit(msgToStatus("Отсутствует назачение вагона"));
            ok = false;
            return OdsTemplate();
        }

        QString carrigeNumber = iobj.getStringAttr("Номер вагона");
        if (carrigeNumber == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует номер вагона.\nЗаполните форму \"Состав эшелона\"");
            emit(msgToStatus("Отсутствует номер вагона"));
            ok = false;
            return OdsTemplate();
        }

        wordTable2.addText(wordTable2.TRCount(), 1, carrigeType);
        wordTable2.addText(wordTable2.TRCount(), 2, destination);
        wordTable2.addText(wordTable2.TRCount(), 3, carrigeNumber);
    }

    wordTable2.appendRow();
    wordTable2.addText(wordTable2.TRCount(), 1, QString());
    wordTable2.addText(wordTable2.TRCount(), 2, QString());
    wordTable2.addText(wordTable2.TRCount(), 3, QString());
    wordTable2.appendRow();
    wordTable2.addText(wordTable2.TRCount(), 1, QString());
    wordTable2.addText(wordTable2.TRCount(), 2, QString());
    wordTable2.addText(wordTable2.TRCount(), 3, QString());
    wordTable2.appendRow();
    wordTable2.addText(wordTable2.TRCount(), 1, QString());
    wordTable2.addText(wordTable2.TRCount(), 2, QString());
    wordTable2.addText(wordTable2.TRCount(), 3, QString());
    wordTable2.appendRow();
    wordTable2.addText(wordTable2.TRCount(), 1, QString());
    wordTable2.addText(wordTable2.TRCount(), 2, QString());
    wordTable2.addText(wordTable2.TRCount(), 3, QString());

    WordTable wordTable3 (0, 4);

    querySQL = "SELECT \"Состав команды\".\"Сопровождение\".\"Должность\" as \"Должность\", "
            "\"Состав команды\".\"Сопровождение\".\"В/звание\" as \"В/звание\", "
            "\"Состав команды\".\"Сопровождение\".\"Фамилия\" as \"Фамилия\", "
            "\"Состав команды\".\"Сопровождение\".\"Имя\" as \"Имя\", "
            "\"Состав команды\".\"Сопровождение\".\"Отчество\" as \"Отчество\", "
            "\"Состав команды\".\"Сопровождение\".\"Примечание\" as \"Примечание\" "
            "FROM \"Состав команды\".\"Сопровождение\" "
            "WHERE \"Состав команды\".\"Сопровождение\".\"Распоряжение\" = "+QString::number(current_vc_id);

    rec_cursor = ODS::OdsInterface::self()->iobjectManager().exec(querySQL);

    if (rec_cursor.size() == 0){
        QMessageBox::warning(0,"Внимание","Отсутвуют записи о составе команды сопровождения.\nЗаполните форму \"Состав команды сопровождения\"");
        emit(msgToStatus("Не найдено записей о составе команды сопровождения"));
        ok = false;
        return OdsTemplate();
    }

    for (ODS::IORecordCursor::iterator It = rec_cursor.begin(); It != rec_cursor.end(); ++It){
        ODS::Record iobj = (*It);

        QString post = iobj.getStringAttr("Должность");
        if (post == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует должность.\nЗаполните форму \"Состав команды сопровождения\"");
            emit(msgToStatus("Отсутствует должность. Форма \"Состав команды сопровождения\""));
            ok = false;
            return OdsTemplate();
        }

        QString rank = iobj.getStringAttr("В/звание");
        if (rank == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует звание.\nЗаполните форму \"Состав команды сопровождения\"");
            emit(msgToStatus("Отсутствует звание. Форма \"Состав команды сопровождения\""));
            ok = false;
            return OdsTemplate();
        }

        QString fio = iobj.getStringAttr("Фамилия")+" "+iobj.getStringAttr("Имя")+" "+iobj.getStringAttr("Отчество");
        if (fio == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует ФИО.\nЗаполните форму \"Состав команды сопровождения\"");
            emit(msgToStatus("Отсутствует ФИО. Форма \"Состав команды сопровождения\""));
            ok = false;
            return OdsTemplate();
        }

        wordTable3.appendRow();

        wordTable3.addText(wordTable3.TRCount(), 1, iobj.getStringAttr("Должность"));
        wordTable3.addText(wordTable3.TRCount(), 2, iobj.getStringAttr("В/звание"));        
        wordTable3.addText(wordTable3.TRCount(), 3, fio);
        wordTable3.addText(wordTable3.TRCount(), 4, iobj.getStringAttr("Примечание"));
    }

    WordTable wordTable4 (0, 4); //5

    querySQL = "SELECT \"Транспортные средства\".\"Автоколонна\".\"Назначение\" as \"Назначение\", "
            "\"Транспортные средства\".\"Автоколонна\".\"Номер\" as \"Номер\", "
            "\"Транспортные средства\".\"Автоколонна\".\"Должность\" as \"Должность\", "
            "\"Транспортные средства\".\"Автоколонна\".\"В/звание\" as \"В/звание\", "
            "\"Транспортные средства\".\"Автоколонна\".\"ФИО\" as \"ФИО\"  "
            "FROM \"Транспортные средства\".\"Автоколонна\""
            "WHERE \"Транспортные средства\".\"Автоколонна\".\"Распоряжение\" = "+QString::number(current_vc_id);

    rec_cursor = ODS::OdsInterface::self()->iobjectManager().exec(querySQL);    

    if (rec_cursor.size() == 0){
        QMessageBox::warning(0,"Внимание","Отсутвуют записи о составе автоколонны.\nЗаполните форму \"Состав автоколонны\"");
        emit(msgToStatus("Не найдено записей о составе автоколонны"));
        ok = false;
        return OdsTemplate();
    }

    for (ODS::IORecordCursor::iterator It = rec_cursor.begin(); It != rec_cursor.end(); ++It){
        ODS::Record iobj = (*It);

        QString destination = iobj.getStringAttr("Назначение");
        if (destination == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует назначение автомобиля.\nЗаполните форму \"Состав автоколонны\"");
            emit(msgToStatus("Отсутствует назначение автомобиля"));
            ok = false;
            return OdsTemplate();
        }

        QString number = iobj.getStringAttr("Номер");
        if (number == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует номер автомобиля.\nЗаполните форму \"Состав автоколонны\"");
            emit(msgToStatus("Отсутствует номер автомобиля"));
            ok = false;
            return OdsTemplate();
        }

        QString firstField =destination+"  "+number;

        QString post = iobj.getStringAttr("Должность");
        if (post == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует должность.\nЗаполните форму \"Состав автоколонны\"");
            emit(msgToStatus("Отсутствует должность. Форма \"Состав автоколонны\""));
            ok = false;
            return OdsTemplate();
        }

        QString rank = iobj.getStringAttr("В/звание");
        if (rank == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует звание.\nЗаполните форму \"Состав автоколонны\"");
            emit(msgToStatus("Отсутствует звание. Форма \"Состав автоколонны\""));
            ok = false;
            return OdsTemplate();
        }

        QString fio = iobj.getStringAttr("ФИО");
        if (fio == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует фио.\nЗаполните форму \"Состав автоколонны\"");
            emit(msgToStatus("Отсутствует фио. Форма \"Состав автоколонны\""));
            ok = false;
            return OdsTemplate();
        }

        wordTable4.appendRow();

        wordTable4.addText(wordTable4.TRCount(), 1, firstField);
        wordTable4.addText(wordTable4.TRCount(), 2, post);
        wordTable4.addText(wordTable4.TRCount(), 3, rank);
        wordTable4.addText(wordTable4.TRCount(), 4, fio);
    }

    WordTable wordTable5 (0, 10);

    querySQL = "SELECT \"Контроль перевозок\".\"Изделие\".\"Наименование\" as \"Наименование\", "
            "\"Контроль перевозок\".\"Изделие\".\"Зав. ном-р\" as \"Зав номер\", "
            "\"Контроль перевозок\".\"Изделие\".\"СГ\" as \"СГ\", "
            "\"Контроль перевозок\".\"Изделие\".\"Блок\" as \"Блок\", "
            "\"Контроль перевозок\".\"Изделие\".\"Тип ТЗУ\" as \"Тип ТЗУ\", "
            "\"Контроль перевозок\".\"Изделие\".\"Зав. ном-р ТЗУ\" as \"Номер ТЗУ\", "
            "\"Контроль перевозок\".\"Изделие\".\"Бригада\" as \"Бригада\", "
            "\"Контроль перевозок\".\"Изделие\".\"Сооружение\" as \"Сооружение\", "
            "\"Контроль перевозок\".\"Изделие\".\"Автоколонна\" as \"Автоколонна\", "
            "\"Контроль перевозок\".\"Изделие\".\"Рейс\" as \"Рейс\" "
            "FROM \"Контроль перевозок\".\"Изделие\" "
            "WHERE \"Контроль перевозок\".\"Изделие\".\"Распоряжение\" = "+QString::number(current_vc_id);

    rec_cursor = ODS::OdsInterface::self()->iobjectManager().exec(querySQL);

    if (rec_cursor.size() == 0){
        QMessageBox::warning(0,"Внимание","Отсутвуют записи о изделиях.\nЗаполните форму \"Изделия\"");
        emit(msgToStatus("Не найдено записей о изделиях"));
        ok = false;
        return OdsTemplate();
    }

    for (ODS::IORecordCursor::iterator It = rec_cursor.begin(); It != rec_cursor.end(); ++It){
        ODS::Record iobj = (*It);

        QString team = iobj.getStringAttr("Бригада");
        if (team == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует бригада.\nЗаполните форму \"Схема-расчет выдачи\"");
            emit(msgToStatus("Отсутствует бригада. Форма \"Изделия\""));
            ok = false;
            return OdsTemplate();
        }

        QString build = iobj.getStringAttr("Сооружение");
        if (build == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует сооружение.\nЗаполните форму \"Схема-расчет выдачи\"");
            emit(msgToStatus("Отсутствует сооружение. Форма \"Изделия\""));
            ok = false;
            return OdsTemplate();
        }

        QString autoCol = iobj.getStringAttr("Автоколонна");
        if (autoCol == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует автоколонна.\nЗаполните форму \"Схема-расчет выдачи\"");
            emit(msgToStatus("Отсутствует автоколонна. Форма \"Изделия\""));
            ok = false;
            return OdsTemplate();
        }

        QString trip = iobj.getStringAttr("Рейс");
        if (trip == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует рейс.\nЗаполните форму \"Схема-расчет выдачи\"");
            emit(msgToStatus("Отсутствует рейс. Форма \"Организация работ: Схема-расчет выдачи\""));
            ok = false;
            return OdsTemplate();
        }

        QString name = iobj.getStringAttr("Наименование");
        if (name == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует наименование.\nЗаполните форму \"Изделия\"");
            emit(msgToStatus("Отсутствует наименование. Форма \"Изделия\""));
            ok = false;
            return OdsTemplate();
        }

        QString zavNumb = iobj.getStringAttr("Зав номер");
        if (zavNumb == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует заводской номер.\nЗаполните форму \"Изделия\"");
            emit(msgToStatus("Отсутствует заводской номер. Форма \"Изделия\""));
            ok = false;
            return OdsTemplate();
        }

        QString ready = iobj.getStringAttr("СГ");
        if (ready == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует степень готовности.\nЗаполните форму \"Изделия\"");
            emit(msgToStatus("Отсутствует степень готовности. Форма \"Изделия\""));
            ok = false;
            return OdsTemplate();
        }

        QString block = iobj.getStringAttr("Блок");
        if (block == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует блок.\nЗаполните форму \"Изделия\"");
            emit(msgToStatus("Отсутствует блок. Форма \"Изделия\""));
            ok = false;
            return OdsTemplate();
        }

        QString tzuType = iobj.getStringAttr("Тип ТЗУ");
        if (tzuType == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует тип тзу.\nЗаполните форму \"Изделия\"");
            emit(msgToStatus("Отсутствует фио. Форма \"Изделия\""));
            ok = false;
            return OdsTemplate();
        }

        QString tzuNumb = iobj.getStringAttr("Номер ТЗУ");
        if (tzuNumb == ""){
            QMessageBox::warning(0,"Внимание","Отсутствует номер тзу.\nЗаполните форму \"Изделия\"");
            emit(msgToStatus("Отсутствует фио. Форма \"Изделия\""));
            ok = false;
            return OdsTemplate();
        }

        wordTable5.appendRow();

        wordTable5.addText(wordTable5.TRCount(), 1, team);
        wordTable5.addText(wordTable5.TRCount(), 2, build);
        wordTable5.addText(wordTable5.TRCount(), 3, autoCol);
        wordTable5.addText(wordTable5.TRCount(), 4, trip);
        wordTable5.addText(wordTable5.TRCount(), 5, name);
        wordTable5.addText(wordTable5.TRCount(), 6, zavNumb);
        wordTable5.addText(wordTable5.TRCount(), 7, ready);
        wordTable5.addText(wordTable5.TRCount(), 8, block);
        wordTable5.addText(wordTable5.TRCount(), 9, tzuType);
        wordTable5.addText(wordTable5.TRCount(), 10, tzuNumb);
    }

    WordTable wordTable6 (0, 3);

    wordTable6.appendRow();

    wordTable6.addText(wordTable6.TRCount(), 1, QString());
    wordTable6.addText(wordTable6.TRCount(), 2, QString());
    wordTable6.addText(wordTable6.TRCount(), 3, QString());

    templ.insertTableData("raspor", &wordTable);
    templ.insertTableData("transp", &wordTable2);
    templ.insertTableData("command", &wordTable3);
    templ.insertTableData("auto", &wordTable4);
    templ.insertTableData("cargo", &wordTable5);
    templ.insertTableData("commision", &wordTable6);

    return templ;
}

OdsTemplate WordMaker::constructinOutDoc(OdsTemplate& templ, bool& ok, int current_vc_id){

    OdsTemplateManager tmpMan = OdsTemplateManager(ODS::OdsInterface::self());
    templ = tmpMan.getTemplate("5_Прием_сдат_эшелон", &ok);

    if (!ok) {
        emit(msgToStatus("Не выполнена загрузка шаблона: "+COMMANDER_DOC));
        return OdsTemplate();
    }

    if (!templ.isValid()) {
        emit(msgToStatus("Недействительный шаблон: "+COMMANDER_DOC));
        return OdsTemplate();
    }

//    QString querySQL = "SELECT \"Информационно-справочная подсистема\".\"Классификатор воинских частей\".\"Условное наименование\" as \"Ответственный\" "
//            "FROM \"Контроль перевозок\".\"Распоряжения\" INNER JOIN \"Контроль перевозок\".\"Содержание\" "
//            "ON \"Контроль перевозок\".\"Распоряжения\".\"Содержание\" = \"Контроль перевозок\".\"Содержание\".\"io_id\" "
//            "INNER JOIN \"Информационно-справочная подсистема\".\"Классификатор воинских частей\" "
//            "ON \"Контроль перевозок\".\"Содержание\".\"Ответственный\" = \"Информационно-справочная подсистема\".\"Классификатор воинских частей\".\"io_id\" "
//            "WHERE \"Контроль перевозок\".\"Распоряжения\".\"io_id\" = "+QString::number(current_vc_id);

    QString querySQL = "SELECT \"Контроль перевозок\".\"Содержание\".\"Ответственный\" as \"Ответственный\" "
            "FROM \"Контроль перевозок\".\"Распоряжения\" INNER JOIN \"Контроль перевозок\".\"Содержание\" "
            "ON \"Контроль перевозок\".\"Распоряжения\".\"Содержание\" = \"Контроль перевозок\".\"Содержание\".\"io_id\" "
            "WHERE \"Контроль перевозок\".\"Распоряжения\".\"io_id\" = "+QString::number(current_vc_id);

    ODS::IORecordCursor rec_cursor = ODS::OdsInterface::self()->iobjectManager().exec(querySQL);

//    for (ODS::IORecordCursor::iterator It = rec_cursor.begin(); It != rec_cursor.end(); ++It){
//        ODS::Record iobj = (*It);

//        templ.insertText("vcSenderNumber", iobj.getStringAttr("Ответственный"));
//    }

    if (!rec_cursor.size()){
        emit(msgToStatus("Не найдено содержание распоряжения"));
        QMessageBox::warning(0,"Ошибка","Не найдено содержание распоряжения.\nЗаполните форму распоряжения");
        return templ;
    }

    int entryId=0;
    for (ODS::IORecordCursor::iterator It = rec_cursor.begin(); It != rec_cursor.end(); ++It){
        ODS::Record iobj = (*It);

//        templ.insertText("vcSenderNumber", iobj.getStringAttr("Ответственный"));
        entryId = iobj.getIntAttr("Ответственный");
    }

    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalog = catalMgr.getCatalogByName("Классификатор воинских частей");
    IRS::CatalogEntry::ptr entry = catalog->entryById(entryId);
    QString vcSender = entry->stringValue("Условное наименование");
    templ.insertText("vcSenderNumber", vcSender);

//    querySQL = SELECT "Перечень грузов"."Запчасти"."Номер вагона" as "Номер трансп ср-ва",
//            "Перечень грузов"."Запчасти"."Шифр" as "Шифр тары", "Перечень грузов"."Запчасти"."Гриф" as "Гриф секр",
//            "Перечень грузов"."Запчасти"."Зав. ном-р" as "Заводск номер", "Перечень грузов"."Запчасти"."К-во мест" as "Колич мест",
//            "Перечень грузов"."Запчасти"."К-во пломб" as "К-во пломб тары", "Перечень грузов"."Запчасти"."Пломба" as "Оттиски пломб тары",
//            "Перечень грузов"."Запчасти"."Примечание" as "Примеч", "Контроль перевозок"."Изделие"."Пломба вагона" as "Плоба вагона"
//            FROM "Перечень грузов"."Запчасти" RIGHT JOIN "Контроль перевозок"."Изделие"
//            ON "Контроль перевозок"."Изделие"."Распоряжение" = "Перечень грузов"."Запчасти"."Распоряжение"
//    "WHERE \"Контроль перевозок\".\"Распоряжения\".\"io_id\" ="+QString::number(current_vc_id);

    querySQL = "SELECT DISTINCT \"Контроль перевозок\".\"Изделие\".\"Номер вагона\" as \"Номер вагона\", "
            "\"Контроль перевозок\".\"Изделие\".\"Пломба вагона\" as \"Пломба вагона\" "
            "FROM \"Контроль перевозок\".\"Изделие\" "
            "WHERE \"Контроль перевозок\".\"Изделие\".\"Распоряжение\" = "+QString::number(current_vc_id);

    WordTable wordTable(0, 8);

    rec_cursor = ODS::OdsInterface::self()->iobjectManager().exec(querySQL);

    if (!rec_cursor.size()){
        emit(msgToStatus(""));
        QMessageBox::warning(0,"Ошибка","Не найдены пломбы вагонов. \nЗаполните форму изделий");
        return templ;
    }

    for (ODS::IORecordCursor::iterator It = rec_cursor.begin(); It != rec_cursor.end(); ++It){
        ODS::Record iobj = (*It);

        wordTable.appendRow();

        wordTable.addText(wordTable.TRCount(), 1, iobj.getStringAttr("Номер вагона"));
        wordTable.addText(wordTable.TRCount(), 2, QString());
        wordTable.addText(wordTable.TRCount(), 3, QString());
        wordTable.addText(wordTable.TRCount(), 4, QString());
        wordTable.addText(wordTable.TRCount(), 5, QString());
        wordTable.addText(wordTable.TRCount(), 6, QString());
        wordTable.addText(wordTable.TRCount(), 7, iobj.getStringAttr("Пломба вагона"));
        wordTable.addText(wordTable.TRCount(), 8, QString());
    }

    templ.insertTableData("table", &wordTable);

    templ.insertText("count",QString::number(rec_cursor.size()));

    QDateTime date;
    date.setDate(QDate::currentDate());

    templ.insertText("current_date0",date.toString("dd.MM.yyyy"));
    templ.insertText("current_date1",date.toString("dd.MM.yyyy"));
    templ.insertText("current_date2",date.toString("dd.MM.yyyy"));

    return templ;
}

OdsTemplate WordMaker::constructTabel(OdsTemplate& templ, bool& ok, int current_vc_id){

    OdsTemplateManager tmpMan = OdsTemplateManager(ODS::OdsInterface::self());
    templ = tmpMan.getTemplate("6_Табель_постам", &ok);

    if (!ok){
        emit(msgToStatus("Не выполнена загрузка шаблона: "+COMMANDER_DOC));
        return OdsTemplate();
    }

    if (!templ.isValid()){
        emit(msgToStatus("Недействительный шаблон: "+COMMANDER_DOC));
        return OdsTemplate();
    }

    QString querySQL = "SELECT \"Состав команды\".\"Сопровождение\".\"Должность\" as \"Должноcть\" "
            "FROM \"Состав команды\".\"Сопровождение\" "
            "WHERE \"Состав команды\".\"Сопровождение\".\"Распоряжение\" ="+QString::number(current_vc_id);

    ODS::IORecordCursor rec_cursor = ODS::OdsInterface::self()->iobjectManager().exec(querySQL);

    int nk = 0, pnk = 0, razv = 0, kar = 0;

    for (ODS::IORecordCursor::iterator It = rec_cursor.begin(); It != rec_cursor.end(); ++It){
        ODS::Record iobj = (*It);

        QString post = iobj.getStringAttr("Должноcть");

        if (post == "НК"){
            nk += 1;
        } else if (post == "ПНК"){
            pnk += 1;
        } else if (post =="Разв"){
            razv += 1;
        } else if (post =="Кар"){
            kar += 1;
        }
    }

    QString result = "НК - "+QString::number(nk)+";\u2028\n"
            "ПНК - "+QString::number(pnk)+";\u2028\n"
            "Разводящий - "+QString::number(razv)+";\u2028\n"
            "Караульный - "+QString::number(kar)+";\u2028\n"
            "Постов         сменных - ";

    templ.insertText("content", result);

    return templ;
}

void WordMaker::constructComAuth(OdsTemplate& templ, bool& ok, int current_vc_id, QString docName){

    OdsTemplateManager tmpMan = OdsTemplateManager(ODS::OdsInterface::self());
    templ = tmpMan.getTemplate("9_Командировочн_удост", &ok);

    if (!ok) {
        emit(msgToStatus("Не выполнена загрузка шаблона: "+COMMANDER_DOC));
        return;
    }

    if (!templ.isValid()) {
        emit(msgToStatus("Недействительный шаблон: "+COMMANDER_DOC));
        return;
    }

    QString querySQL = "SELECT \"Состав команды\".\"Сопровождение\".\"Должность\" as \"Должность\", "
            "\"Состав команды\".\"Сопровождение\".\"io_id\" as \"io_id\", "
            "\"Состав команды\".\"Сопровождение\".\"Имя\" as \"Имя\", "
            "\"Состав команды\".\"Сопровождение\".\"Отчество\" as \"Отчество\", "
            "\"Состав команды\".\"Сопровождение\".\"Фамилия\" as \"Фамилия\" "
            "FROM \"Состав команды\".\"Сопровождение\" "
            "WHERE \"Состав команды\".\"Сопровождение\".\"Распоряжение\" = "+QString::number(current_vc_id);

    ODS::IORecordCursor rec_cursor = ODS::OdsInterface::self()->iobjectManager().exec(querySQL);

    for (ODS::IORecordCursor::iterator It = rec_cursor.begin(); It != rec_cursor.end(); ++It){
        ODS::Record iobj = (*It);

        QString fio = iobj.getStringAttr("Должность") + " " + iobj.getStringAttr("Фамилия") + " " +iobj.getStringAttr("Имя") + " " + iobj.getStringAttr("Отчество");

        templ.insertText("name0", fio);
        templ.insertText("name1", fio);

        int personId = iobj.getIntAttr("io_id");

        constructComAuthPerson(templ, current_vc_id, personId);

        QString fileName = docName+"_"+iobj.getStringAttr("Должность") + "_" + iobj.getStringAttr("Фамилия");

        saveAsDoc(templ, fileName);
    }
}

void WordMaker::constructComAuthPerson(OdsTemplate& templ, int current_vc_id, int personId){

//    QString querySQL = "SELECT \"Информационно-справочная подсистема\".\"Классификатор воинских частей\".\"Условное наименование\" "
//            "as \"Ответственный\" FROM \"Контроль перевозок\".\"Распоряжения\" "
//            "INNER JOIN \"Контроль перевозок\".\"Содержание\" ON "
//            "\"Контроль перевозок\".\"Распоряжения\".\"Содержание\" = \"Контроль перевозок\".\"Содержание\".\"io_id\" "
//            "INNER JOIN \"Информационно-справочная подсистема\".\"Классификатор воинских частей\" "
//            "ON \"Информационно-справочная подсистема\".\"Классификатор воинских частей\".\"io_id\" = "
//            "\"Контроль перевозок\".\"Содержание\".\"Ответственный\" "
//            "WHERE \"Контроль перевозок\".\"Распоряжения\".\"io_id\" ="+QString::number(current_vc_id);

    QString querySQL = "SELECT \"Контроль перевозок\".\"Содержание\".\"Ответственный\" as \"Ответственный\" "
            "FROM \"Контроль перевозок\".\"Распоряжения\" INNER JOIN \"Контроль перевозок\".\"Содержание\" "
            "ON \"Контроль перевозок\".\"Распоряжения\".\"Содержание\" = \"Контроль перевозок\".\"Содержание\".\"io_id\" "
            "WHERE \"Контроль перевозок\".\"Распоряжения\".\"io_id\" = "+QString::number(current_vc_id);

    ODS::IORecordCursor rec_cursor = ODS::OdsInterface::self()->iobjectManager().exec(querySQL);

    int entryId=0;
    for (ODS::IORecordCursor::iterator It = rec_cursor.begin(); It != rec_cursor.end(); ++It){
        ODS::Record iobj = (*It);

//        templ.insertText("respon0", iobj.getStringAttr("Ответственный"));
//        templ.insertText("respon1", iobj.getStringAttr("Ответственный"));

        entryId = iobj.getIntAttr("Ответственный");
    }

    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalog = catalMgr.getCatalogByName("Классификатор воинских частей");
    IRS::CatalogEntry::ptr entry = catalog->entryById(entryId);
    QString responName = entry->stringValue("Условное наименование");
    templ.insertText("respon0", responName);
    templ.insertText("respon1", responName);

    QDateTime date;
    date.setDate(QDate::currentDate());

    templ.insertText("current_date0",date.toString("dd.MM.yyyy"));
    templ.insertText("current_date1",date.toString("dd.MM.yyyy"));

    querySQL = "SELECT \"Документы КП\".\"Командировочное удостоверение\".\"Дата начала\" as \"Дата начала\", "
            "\"Документы КП\".\"Командировочное удостоверение\".\"Дата окончания\" as \"Дата окончания\", "
            "\"Документы КП\".\"Командировочное удостоверение\".\"Дата приказа\" as \"Дата приказа\", "
            "\"Документы КП\".\"Командировочное удостоверение\".\"Номер приказа\" as \"Номер приказа\", "
            "\"Документы КП\".\"Командировочное удостоверение\".\"Пункты назначения\" as \"Пункты назначения\" "
            "FROM \"Документы КП\".\"Командировочное удостоверение\" "
            "WHERE \"Документы КП\".\"Командировочное удостоверение\".\"Распоряжение\" = "+QString::number(current_vc_id);

    rec_cursor = ODS::OdsInterface::self()->iobjectManager().exec(querySQL);

    for (ODS::IORecordCursor::iterator It = rec_cursor.begin(); It != rec_cursor.end(); ++It){
        ODS::Record iobj = (*It);

        templ.insertText("destination0", iobj.getStringAttr("Пункты назначения"));
        templ.insertText("destination1", iobj.getStringAttr("Пункты назначения"));

        CDate date = iobj.getDateTimeAttr("Дата начала");
        QDateTime dt = QDateTime::fromTime_t(date.getDateTime());

        templ.insertText("dateFrom0", dt.date().toString("dd.MM.yy"));
        templ.insertText("dateFrom1", dt.date().toString("dd.MM.yy"));

        date = iobj.getDateTimeAttr("Дата окончания");
        QDateTime dt2 = QDateTime::fromTime_t(date.getDateTime());

        templ.insertText("dateTo0", dt2.date().toString("dd.MM.yy"));
        templ.insertText("dateTo1", dt2.date().toString("dd.MM.yy"));

        int days = dt.daysTo(dt2);

        templ.insertText("days0", QString::number(days));
        templ.insertText("days1", QString::number(days));

        date = iobj.getDateTimeAttr("Дата приказа");
        dt = QDateTime::fromTime_t(date.getDateTime());

        templ.insertText("orderDate0", dt.date().toString("dd.MM.yy"));
        templ.insertText("orderDate1", dt.date().toString("dd.MM.yy"));

        templ.insertText("orderNumber0", iobj.getStringAttr("Номер приказа"));
        templ.insertText("orderNumber1", iobj.getStringAttr("Номер приказа"));

    }

    querySQL = "SELECT \"Состав команды\".\"Сопровождение\".\"Наимен. вооруж.\" as \"Наимен. вооруж.\", "
            "\"Состав команды\".\"Сопровождение\".\"Номер вооруж.\" as \"Номер вооруж.\" "
            "FROM \"Состав команды\".\"Сопровождение\" "
            "WHERE \"Состав команды\".\"Сопровождение\".\"io_id\" = "+QString::number(personId);

    rec_cursor = ODS::OdsInterface::self()->iobjectManager().exec(querySQL);

    WordTable wordTable (0, 3);

    for (ODS::IORecordCursor::iterator It = rec_cursor.begin(); It != rec_cursor.end(); ++It){
        ODS::Record iobj = (*It);

        wordTable.appendRow();

        wordTable.addText(wordTable.TRCount(), 1, iobj.getStringAttr("Наимен. вооруж."));
        wordTable.addText(wordTable.TRCount(), 2, iobj.getStringAttr("Номер вооруж."));
        wordTable.addText(wordTable.TRCount(), 3, QString());
    }

    templ.insertTableData("weapon0", &wordTable);
    templ.insertTableData("weapon1", &wordTable);
}

void WordMaker::openPlan()
{
    using namespace Pln;
    Planner* planner= new Planner("План", ODS::OdsInterface::self(), false);

    Condition condTempl = crCond(QString(PlanAttrs::ATTR_TEMPLATE), true, Op_Eq);
    Condition condName = crCond(QString(PlanAttrs::ATTR_SHORTNAME), QString("План-график проведения работ"), Op_Eq);
    QList<PlanPt> plansList = planner->loadPlans(condTempl & condName);

    if (plansList.isEmpty()){
        QMessageBox::critical(0, "Ошибка", "Не найден шаблон плана: 'План-график проведения работ'");
        return;
    }

    ODS::User user = ODS::OdsInterface::self()->subjectManager().getCurrentUser();
    ContactPt contact = planner->contact(user);

    for(QList<PlanPt>::iterator It = plansList.begin(); It != plansList.end(); ++It){
        PlanPt planTemple = (*It);
        PlanPt plan = planner->createAndSaveFromTemplate(planTemple, contact);
        if (plan.isNull()){
            QMessageBox::critical(0, "Ошибка", "Ошибка при создании плана");
            msgToStatus(QString("Ошибка при создании плана"));
            return;
        }
        planner->sendPlanForEvCodeCreatedSignal(plan, true);
    }
}

OdsTemplate WordMaker::constructActs(OdsTemplate& templ, bool& ok, int current_vc_id){

    OdsTemplateManager tmpMan = OdsTemplateManager(ODS::OdsInterface::self());
    templ = tmpMan.getTemplate("4_Акты_формы_5ГУ", &ok);

    if (!ok) {
        emit(msgToStatus("Не выполнена загрузка шаблона: "+ACTS_DOC));
        return OdsTemplate();
    }

    if (!templ.isValid()) {
        emit(msgToStatus("Недействительный шаблон: "+ACTS_DOC));
        return OdsTemplate();
    }

//    QString querySQL = "SELECT \"Информационно-справочная подсистема\".\"Классификатор воинских частей\".\"Условное наименование\" "
//            "as \"Ответственный\", \"Контроль перевозок\".\"Распоряжения\".\"Дата\" as \"Дата\", "
//            "\"Информационно-справочная подсистема\".\"Классификатор воинских частей\".\"io_id\" as \"respon_id\", "
//            "\"Контроль перевозок\".\"Распоряжения\".\"Номер\" as \"Номер\" "
//            "FROM \"Контроль перевозок\".\"Распоряжения\" "
//            "INNER JOIN \"Контроль перевозок\".\"Содержание\" ON "
//            "\"Контроль перевозок\".\"Распоряжения\".\"Содержание\" = \"Контроль перевозок\".\"Содержание\".\"io_id\" "
//            "INNER JOIN \"Информационно-справочная подсистема\".\"Классификатор воинских частей\" "
//            "ON \"Информационно-справочная подсистема\".\"Классификатор воинских частей\".\"io_id\" = "
//            "\"Контроль перевозок\".\"Содержание\".\"Ответственный\" "
//            "WHERE \"Контроль перевозок\".\"Распоряжения\".\"io_id\" ="+QString::number(current_vc_id);

    QString querySQL = "SELECT \"Контроль перевозок\".\"Содержание\".\"Ответственный\" as \"Ответственный\", "
            "\"Контроль перевозок\".\"Распоряжения\".\"Дата\" as \"Дата\", "
            "\"Контроль перевозок\".\"Распоряжения\".\"Номер\" as \"Номер\" "
            "FROM \"Контроль перевозок\".\"Распоряжения\" "
            "INNER JOIN \"Контроль перевозок\".\"Содержание\" ON "
            "\"Контроль перевозок\".\"Распоряжения\".\"Содержание\" = \"Контроль перевозок\".\"Содержание\".\"io_id\" "
            "WHERE \"Контроль перевозок\".\"Распоряжения\".\"io_id\" ="+QString::number(current_vc_id);

    ODS::IORecordCursor rec_cursor = ODS::OdsInterface::self()->iobjectManager().exec(querySQL);

    int respon_id = 0;

    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalog = catalMgr.getCatalogByName("Классификатор воинских частей");

    for (ODS::IORecordCursor::iterator It = rec_cursor.begin(); It != rec_cursor.end(); ++It){
        ODS::Record iobj = (*It);

//        QString sender = iobj.getStringAttr("Ответственный");
        int entryId = iobj.getIntAttr("Ответственный");
        IRS::CatalogEntry::ptr entry = catalog->entryById(entryId);
        QString sender = entry->stringValue("Условное наименование");

        templ.insertText("sender0", sender);
        templ.insertText("sender1", sender);
        templ.insertText("sender2", sender);

        CDate date = iobj.getDateTimeAttr("Дата");
        QDateTime dt = QDateTime::fromTime_t(date.getDateTime());

        templ.insertText("dateRaspor", dt.date().toString("dd.MM.yy"));
        templ.insertText("numberRaspor", iobj.getStringAttr("Номер"));

        respon_id = entry->id();
    }

    querySQL = "SELECT \"Контроль перевозок\".\"Содержание таблица\".\"Получатель\" FROM \"Контроль перевозок\".\"Содержание таблица\" "
            "WHERE \"Контроль перевозок\".\"Содержание таблица\".\"Распоряжение\" = "+QString::number(current_vc_id)+
            " AND \"Контроль перевозок\".\"Содержание таблица\".\"Отправитель\" = "+QString::number(respon_id);

    rec_cursor = ODS::OdsInterface::self()->iobjectManager().exec(querySQL);

    for (ODS::IORecordCursor::iterator It = rec_cursor.begin(); It != rec_cursor.end(); ++It){
        ODS::Record iobj = (*It);

//        QString sender = iobj.getStringAttr("Получатель");
        int entryId = iobj.getIntAttr("Получатель");
        IRS::CatalogEntry::ptr entry = catalog->entryById(entryId);
        QString sender;
        if (entry){
             sender = entry->stringValue("Условное наименование");
        } else {
            emit(msgToStatus(""));
        }

        templ.insertText("receiver0", sender);
        templ.insertText("receiver1", sender);
    }

    QDateTime date;
    date.setDate(QDate::currentDate());

    templ.insertText("date0",date.toString("dd.MM.yyyy"));
    templ.insertText("date1",date.toString("dd.MM.yyyy"));
    templ.insertText("date2",date.toString("dd.MM.yyyy"));

    QString result;
    int position = 1;

    querySQL = "SELECT \"Контроль перевозок\".\"Изделие\".\"Наименование\" as "
            "\"Наименование\", \"Контроль перевозок\".\"Изделие\".\"Зав. ном-р\" as "
            "\"Зав. ном-р\" FROM \"Контроль перевозок\".\"Изделие\" "
            "WHERE \"Контроль перевозок\".\"Изделие\".\"Распоряжение\" ="+QString::number(current_vc_id);

    rec_cursor = ODS::OdsInterface::self()->iobjectManager().exec(querySQL);

    for (ODS::IORecordCursor::iterator It = rec_cursor.begin(); It != rec_cursor.end(); ++It){
        ODS::Record iobj = (*It);

        result += QString::number(position)+". "+iobj.getStringAttr("Наименование")+", заводские № "+iobj.getStringAttr("Зав. ном-р")+"\u2028\n";

        position++;
    }

    querySQL = "SELECT \"Перечень грузов\".\"Запчасти\".\"Наименование\" as \"Наименование\", "
            "\"Перечень грузов\".\"Запчасти\".\"Зав. ном-р\" as \"Зав. ном-р\", "
            "\"Перечень грузов\".\"Запчасти\".\"К-во мест\" as \"К-во мест\" "
            "FROM \"Перечень грузов\".\"Запчасти\" "
            "WHERE \"Перечень грузов\".\"Запчасти\".\"Распоряжение\" ="+QString::number(current_vc_id);

    rec_cursor = ODS::OdsInterface::self()->iobjectManager().exec(querySQL);

    for (ODS::IORecordCursor::iterator It = rec_cursor.begin(); It != rec_cursor.end(); ++It){
        ODS::Record iobj = (*It);

        result += QString::number(position)+". ЗИП на "+iobj.getStringAttr("К-во мест")+" изделий "+iobj.getStringAttr("Наименование")
                +", заводские № "+iobj.getStringAttr("Зав. ном-р")+"\u2028\n";

        position++;
    }

    templ.insertText("weapon", result);

    return templ;
}

void WordMaker::saveAsDoc(OdsTemplate& templ, const QString& docName) {

    QWidget* parent= 0;
    QString fileName = QFileDialog::getSaveFileName(parent, "Сохранить в файл", docName, "Docx Files (*.docx)");

//    QString fileName = "/tmp/";

    if (fileName.isEmpty()){
        emit msgToStatus("Документ не сохранен");
        return;
    }

    #ifdef Q_OS_WIN
        if (!templ.saveTemplate("C:\\Program Files\\Eureca\\transport\\doc", WordProcessingDocument::DocX))
        {
            emit(msgToStatus("Ошибка при сохранении документа!"));
            return;
        } else {
            emit msgToStatus("Документ сохранен");
        }
    #elif defined Q_OS_UNIX
        if (!templ.saveTemplate(fileName, WordProcessingDocument::DocX)){
            emit(msgToStatus("Ошибка при сохранении документа!"));
            return;
        } else {
            emit msgToStatus("Документ сохранен");
        }
    #endif
}
