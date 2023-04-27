#include "sqlmanager.h"
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>

SqlManager * SqlManager::instance = nullptr;

// 数据库设置
QSqlDatabase SqlManager::database = QSqlDatabase::addDatabase("QMYSQL");
QString SqlManager::dbHostName = "127.0.0.1";
QString SqlManager::dbDatabaseName = "qtproject";
QString SqlManager::dbUserName = "root";
QString SqlManager::dbPassword = "123456";
int SqlManager::dbPort = 3306;

SqlManager::SqlManager()
{
    initConnect();
    query = new QSqlQuery();
    queryModel = new QSqlQueryModel();
}

SqlManager::~SqlManager()
{

}

// 登录查询
bool SqlManager::loginQuery(QString user_id, QString user_pswd)
{
    QSqlQuery query;
    return query.exec(QString("select `admin_id`,`admin_password` from admin where `admin_id`= %1 and `admin_password` = '%2'" ).arg(user_id).arg(user_pswd));
}

QString SqlManager::roleQuery(QString user_id)
{
    QSqlQuery query(QString("select `admin_role` from admin where `admin_id`= %1 ").arg(user_id));
    query.next();
    QString role = query.value(0).toString();
    return role;
}

QSqlQueryModel* SqlManager::collegeDataQuery()
{
    queryModel->setQuery("SELECT * FROM college GROUP BY college_id;");
    return queryModel;
}

QMap<QString, QString> SqlManager::collegeDataQuery(QString where)
{
    Q_UNUSED(where);
    QMap<QString, QString> collegeDdataMap;
    QSqlQuery query("SELECT college_id,college_name FROM college;");
    while (query.next()) {
        collegeDdataMap.insert(query.value(0).toString(),query.value(1).toString());
    }
    return collegeDdataMap;
}


// major
QSqlQueryModel *SqlManager::majorDataQuery()
{
    queryModel->setQuery("SELECT * FROM view_major_info GROUP BY major_id;");
    return  queryModel;
}

QSqlQueryModel *SqlManager::majorDataQuery(QString condition1, QString para1)
{
    queryModel->setQuery(QString("Select * from view_major_info where `%1` = '%2';").arg(condition1).arg(para1));
    return queryModel;
}

QMap<QString, QString> SqlManager::majorDataQuery(QString condition1,QString para1,int mode)
{
    Q_UNUSED(mode);
    QMap<QString, QString> majorDdataMap;
    QSqlQuery query(QString("SELECT * FROM view_major_info where %1 = '%2';").arg(condition1).arg(para1));
    while (query.next()) {
        majorDdataMap.insert(query.value(0).toString(),query.value(1).toString());
    }
    return majorDdataMap;
}

QMap<QString, QString> SqlManager::majorDataQuery(QString condition1,QString para1,QString condition2,QString para2)
{
    QMap<QString, QString> majorDdataMap;
    QSqlQuery query(QString("SELECT major_id,major_name FROM view_major_info where %1 = '%2' and %3 = '%4';").arg(condition1).arg(para1).arg(condition2).arg(para2));
    while (query.next()) {
        majorDdataMap.insert(query.value(0).toString(),query.value(1).toString());
    }
    return majorDdataMap;
}


// curriculum
QSqlQueryModel *SqlManager::curriculumDataQuery()
{
    queryModel->setQuery(QString("SELECT * FROM view_curriculum_info GROUP BY curriculum_id;"));
    return queryModel;
}

// 检测课程是否存在
bool SqlManager::curriculumExistQuery(QString condition)
{
    queryModel->setQuery(QString("select curriculum_id,curriculum_name from curriculum where curriculum_id = '%1';").arg(condition));
    if(queryModel->rowCount() == 0)
        return false;
    else
        return true;
}

QSqlQueryModel *SqlManager::curriculumDataQuery(QString condition1, QString para1)
{
    qDebug() << QString("SELECT * FROM view_curriculum_info where %1 = '%2';").arg(condition1).arg(para1);
    queryModel->setQuery(QString("SELECT * FROM view_curriculum_info where %1 = '%2';").arg(condition1).arg(para1));
    return queryModel;
}

QSqlQueryModel *SqlManager::curriculumDataQuery(QString condition1, QString para1, QString condition2, QString para2)
{
    qDebug() << QString("SELECT * FROM view_curriculum_info where %1 = '%2' and %3 = '%4';").arg(condition1).arg(para1).arg(condition2).arg(para2);
    queryModel->setQuery(QString("SELECT * FROM view_curriculum_info where %1 = '%2' and %3 = '%4';").arg(condition1).arg(para1).arg(condition2).arg(para2));
    return queryModel;
}

QMap<QString, QString> SqlManager::curriculumDataQuery(QString condition1, QString para1, QString condition2, QString para2, int mode)
{
    Q_UNUSED(mode);
    QMap<QString, QString> curriculumDdataMap;
    QSqlQuery query(QString("SELECT curriculum_id,curriculum_name FROM view_curriculum_info where %1 = '%2' and %3 = '%4';").arg(condition1).arg(para1).arg(condition2).arg(para2));
    while (query.next()) {
        curriculumDdataMap.insert(query.value(0).toString(),query.value(1).toString());
    }
    return curriculumDdataMap;
}

QSqlQueryModel *SqlManager::classDataQuery(QString condition1, QString para1, QString condition2, QString para2, QString condition3, QString para3)
{
    queryModel->setQuery(QString("SELECT * FROM view_class_info where %1 = '%2' and %3 = '%4' and %5 = '%6' GROUP BY class_id;").arg(condition1).arg(para1).arg(condition2).arg(para2).arg(condition3).arg(para3));
    return queryModel;
}

QSqlQueryModel *SqlManager::studentDataQuery(QString condition1, QString para1, QString condition2, QString para2, QString condition3, QString para3)
{
    qDebug() << QString("SELECT * FROM view_student_info where %1 = '%2' and %3 = '%4' and %5 = '%6';").arg(condition1).arg(para1).arg(condition2).arg(para2).arg(condition3).arg(para3);
    queryModel->setQuery(QString("SELECT * FROM view_student_info where %1 = '%2' and %3 = '%4' and %5 = '%6';").arg(condition1).arg(para1).arg(condition2).arg(para2).arg(condition3).arg(para3));
    return queryModel;
}

QSqlQueryModel *SqlManager::studentDataQuery(QString condition1)
{
    queryModel->setQuery(QString("SELECT * FROM view_student_info where stu_id = '%1' or stu_name = '%2';").arg(condition1).arg(condition1));
    return queryModel;
}

QMap<QString, QString> SqlManager::classDataQuery(QString condition1, QString para1, QString condition2, QString para2, QString condition3, QString para3, QString where)
{
    Q_UNUSED(where);
    QMap<QString, QString> classDdataMap;
    QSqlQuery query(QString("SELECT class_id,class_name FROM view_class_info where %1 = '%2' and %3 = '%4' and %5 = '%6';").arg(condition1).arg(para1).arg(condition2).arg(para2).arg(condition3).arg(para3));
    while (query.next()) {
        classDdataMap.insert(query.value(0).toString(),query.value(1).toString());
    }
    return classDdataMap;
}

QSqlQueryModel *SqlManager::classDataQuery()
{
    queryModel->setQuery("SELECT * FROM view_class_info GROUP BY class_id;");
    return  queryModel;
}

QMap<QString, QString> SqlManager::classDataQuery(QString condition1, QString para1)
{
    QMap<QString, QString> classDdataMap;
    QSqlQuery query(QString("SELECT class_id,class_name FROM view_class_info where %1 = '%2';").arg(condition1).arg(para1));
    while (query.next()) {
        classDdataMap.insert(query.value(0).toString(),query.value(1).toString());
    }
    return classDdataMap;
}

QMap<QString, QString> SqlManager::teacherDataQuery(QString condition1, QString para1)
{
    QMap<QString, QString> teaDataMap;
    query->exec(QString("Select * from teacher where `%1` = '%2';").arg(condition1).arg(para1));
    while (query->next()) {
        teaDataMap.insert(query->value(0).toString(),query->value(1).toString());
    }
    return teaDataMap;
}

bool SqlManager::teacherExistQuery(QString condition1)
{
    queryModel->setQuery(QString("select tea_id,tea_name from teacher where tea_id = '%1';").arg(condition1));
    if(queryModel->rowCount() == 0)
        return false;
    else
        return true;
}

QMap<QString, QString> SqlManager::examHallDataQuery()
{
    // 返回考务人员姓名和考场的一对一关系；
    QMap<QString, QString> examHDataMap;
    query->exec(QString("Select examHall_id,examHall_name from view_examHall_info;"));
    while (query->next()) {
        examHDataMap.insert(query->value(0).toString(),query->value(1).toString());
    }
    return examHDataMap;
}

QSqlQueryModel *SqlManager::studentDataGroupQuery(QString condition1)
{
    queryModel->setQuery(QString("select * from view_student_info ORDER BY %1 DESC;").arg(condition1));
    return queryModel;
}

QSqlQueryModel *SqlManager::studentDataQuery(int start, int end)
{
    queryModel->setQuery(QString("select * from view_student_info limit %1,%2;").arg(start).arg(end));
    return queryModel;
}


QSqlQueryModel *SqlManager::studentDataQuery()
{
    queryModel->setQuery("select * from view_student_info GROUP BY stu_id;");
    return queryModel;
}


// 登录更新
void SqlManager::loginUpdate(QString user_id)
{
    QString curTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    query->exec(QString("UPDATE admin SET admin_lastLogin_time = '%1' WHERE admin_id = '%2';").arg(curTime).arg(user_id));
}

bool SqlManager::collegeDateUpdate(QString college_id1,QString college_id2, QString college_name)
{
    return query->exec(QString("UPDATE college SET college_id = '%1',college_name = '%2' WHERE college_id = '%3';").arg(college_id2).arg(college_name).arg(college_id1));
}

bool SqlManager::majorDateUpdate(QString major_id1, QString major_id2, QString major_name, QString colleg_id)
{
    return query->exec(QString("UPDATE major SET major_id = '%1',major_name = '%2' ,college_id = '%3' WHERE major_id = '%4';").arg(major_id2).arg(major_name).arg(colleg_id).arg(major_id1));
}

bool SqlManager::classDataUpdate(QString class_id1, QString class_id2, QString class_name,QString class_grade,QString major_id, QString colleg_id)
{
    return query->exec(QString("UPDATE class SET class_id = '%1',class_name = '%2',class_grade = '%3',major_id = '%4',college_id = '%5' WHERE class_id = '%6';")
                       .arg(class_id2).arg(class_name).arg(class_grade).arg(major_id).arg(colleg_id).arg(class_id1));
}

bool SqlManager::curriculumDataUpdate(QString curriculum_id1, QString curriculum_id2, QString curriculum_name, QString tea_id, QString major_id, QString colleg_id)
{
    return query->exec(QString("UPDATE curriculum SET curriculum_id = '%1',curriculum_name = '%2',tea_id = '%3',major_id = '%4',college_id = '%5' WHERE curriculum_id = '%6';")
                       .arg(curriculum_id2).arg(curriculum_name).arg(tea_id).arg(major_id).arg(colleg_id).arg(curriculum_id1));
}

bool SqlManager::studentPswdUpdate(QString stu_id, QString stu_password)
{
    return query->exec(QString("UPDATE student SET stu_password = '%1' WHERE stu_id = '%2';").arg(stu_password).arg(stu_id));
}

bool SqlManager::studentUpdate(QString stu_id1,QString stu_id2, QString stu_name, QString stu_gender, QString class_id)
{
    return query->exec(QString("UPDATE student SET stu_id = '%1',stu_name = '%2',stu_gender='%3',class_id = '%4' WHERE stu_id = '%5';")
                       .arg(stu_id2).arg(stu_name).arg(stu_gender).arg(class_id).arg(stu_id1));
}

// 插入
bool SqlManager::collegeDataInsert(QString colleg_id, QString college_name)
{
    return query->exec(QString("insert into college (`college_id`,`college_name`) values('%1','%2')").arg(colleg_id).arg(college_name));
}

bool SqlManager::majorDataInsert(QString major_id, QString major_name, QString colleg_id)
{
    return query->exec(QString("insert into major (`major_id`,`major_name`,`college_id`) values('%1','%2','%3')").arg(major_id).arg(major_name).arg(colleg_id));
}

bool SqlManager::classDataInsert(QString class_id, QString class_name,QString class_grade, QString major_id, QString colleg_id)
{
    return query->exec(QString("insert into class (`class_id`,`class_name`,`class_grade`,`major_id`,`college_id`) values('%1','%2','%3','%4','%5')").arg(class_id).arg(class_name).arg(class_grade).arg(major_id).arg(colleg_id));
}

bool SqlManager::curriculumDataInsert(QString curriculum_id, QString curriculum_name, QString tea_id, QString major_id, QString colleg_id)
{
    return query->exec(QString("insert into curriculum (`curriculum_id`,`curriculum_name`,`tea_id`,`major_id`,`college_id`) values('%1','%2','%3','%4','%5')")
                       .arg(curriculum_id).arg(curriculum_name).arg(tea_id).arg(major_id).arg(colleg_id));
}

bool SqlManager::studentDataInsert(QString stu_id, QString stu_name, QString stu_gender, QString stu_password,QString class_id)
{
    return query->exec(QString("insert into student (stu_id,stu_name,stu_gender,stu_password,class_id) values('%1','%2','%3','%4','%5')").arg(stu_id).arg(stu_name).arg(stu_gender).arg(stu_password).arg(class_id));
}

bool SqlManager::questionsDataInsert(QString question_id, QString question_type, QString question_tigan, QString question_options, QString question_biaoda, int question_score, QString question_star, QString question_jiexi, QString strTestPId)
{
    return query->exec(QString("INSERT INTO questions (question_id,question_type,question_tigan,question_options,question_biaoda,question_score,question_star,question_jiexi,testP_id) "
                               "value('%1','%2','%3','%4','%5',%6,'%7','%8','%9');").arg(question_id).arg(question_type).arg(question_tigan).arg(question_options).arg(question_biaoda).arg(question_score).arg(question_star).arg(question_jiexi).arg(strTestPId));
}

bool SqlManager::testPaperDataInsert(QString testP_id, QString testP_name, QString tea_id, QString curriculum_id)
{
    qDebug() << QString("INSERT into testpaper (testP_id,testP_name,tea_id,curriculum_id) VALUE('%1','%2','%3','%4');")
                .arg(testP_id).arg(testP_name).arg(tea_id).arg(curriculum_id);
    return query->exec(QString("INSERT into testpaper (testP_id,testP_name,tea_id,curriculum_id) VALUE('%1','%2','%3','%4');")
                       .arg(testP_id).arg(testP_name).arg(tea_id).arg(curriculum_id));
}

bool SqlManager::examinationDataInsert(QString examination_id, QString examination_name, QString examination_comment, int examination_duration, QString examination_start_time, QString examination_end_time, QString testP_id,QString examH_id)
{
    return query->exec(QString("INSERT into examination (examination_id,examination_name,examination_comment,examination_duration,examination_start_time,examination_end_time,testP_id,examH_id) VALUE('%1','%2','%3',%4,'%5','%6','%7','%8');")
                       .arg(examination_id).arg(examination_name).arg(examination_comment).arg(examination_duration).arg(examination_start_time).arg(examination_end_time).arg(testP_id).arg(examH_id));
}

// 删除
bool SqlManager::formCollegeDelete(QString college_id)
{
    return query->exec(QString("Delete from college where college_id = '%1'").arg(college_id));
}

bool SqlManager::formMajorDelete(QString major_id)
{
    return query->exec(QString("Delete from major where major_id = '%1'").arg(major_id));
}

bool SqlManager::formClassDelete(QString class_id)
{
    return query->exec(QString("Delete from class where class_id = '%1'").arg(class_id));
}

bool SqlManager::formCurriuclumDelete(QString curriuclum_id)
{
    return query->exec(QString("Delete from curriculum where curriculum_id = '%1'").arg(curriuclum_id));
}

bool SqlManager::formStudentDelete(QString stu_id)
{
    return query->exec(QString("Delete from student where stu_id = '%1'").arg(stu_id));
}

// 初始化连接数据库
bool SqlManager::initConnect()
{
    qDebug() << "success";
    database.setHostName(dbHostName);
    database.setDatabaseName(dbDatabaseName);
    database.setPort(dbPort);
    database.setUserName(dbUserName);
    database.setPassword(dbPassword);
    if(!database.open())
    {
        qDebug() << "fail";
        QMessageBox::critical(0,"无法连接系统数据库!!!","请联系管理员解决.",QMessageBox::Cancel);
        return false;
    }
    return true;

}
