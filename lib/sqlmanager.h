#ifndef SQLMANAGER_H
#define SQLMANAGER_H
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

/* 使用单例模式 */

class SqlManager
{
public:
    SqlManager();//禁止构造函数
    SqlManager(const SqlManager&){}//禁止拷贝构造函数
    SqlManager & operator=(const SqlManager &){}//禁止赋值拷贝构造函数
    ~SqlManager();

    static SqlManager * getInstance()
    {
        if(instance == nullptr)
            instance = new SqlManager();
        return instance;
    }

    // --- 查询
    bool loginQuery(QString user_id,QString user_pswd);
    QString roleQuery(QString user_id);

    // 1.基础查询
    QSqlQueryModel* collegeDataQuery();
    QSqlQueryModel* majorDataQuery();
    QSqlQueryModel* curriculumDataQuery();
    QSqlQueryModel* classDataQuery();
    QSqlQueryModel* studentDataQuery();

    // 2.筛选查询
    QSqlQueryModel* curriculumDataQuery(QString condition1,QString para1,QString condition2,QString para2);
    QSqlQueryModel* classDataQuery(QString condition1,QString para1,QString condition2,QString para2,QString condition3,QString para3);
    QSqlQueryModel* studentDataQuery(QString condition1,QString para1,QString condition2,QString para2,QString condition3,QString para3);
    QSqlQueryModel* studentDataQuery(QString condition1);

    // 3.键值对查询
    QMap<QString,QString> collegeDataQuery(QString where); // 查学院
    QMap<QString,QString> majorDataQuery(QString condition1,QString para1); // 专业
    QMap<QString,QString> majorDataQuery(QString condition1,QString para1,QString condition2,QString para2);
    QMap<QString,QString> classDataQuery(QString condition1,QString para1); // 班级
    QMap<QString,QString> classDataQuery(QString condition1,QString para1,QString condition2,QString para2,QString condition3,QString para3,QString where);
    QMap<QString,QString> teacherDataQuery(QString condition1, QString para1);

    // 4.分组查询
    QSqlQueryModel* studentDataGroupQuery(QString condition1);

    // 5.区间查询
    QSqlQueryModel* studentDataQuery(int start,int end);

    // 更新
    void loginUpdate(QString user_id);
    bool collegeDateUpdate(QString college_id1,QString colleg_id2,QString college_name);
    bool majorDateUpdate(QString major_id1,QString major_id2,QString major_name,QString colleg_id);
    bool classDataUpdate(QString class_id1,QString class_id2,QString class_name,QString class_grade,QString major_id,QString colleg_id);
    bool curriculumDataUpdate(QString curriculum_id1,QString curriculum_id2,QString curriculum_name,QString tea_id,QString major_id,QString colleg_id);
    bool studentPswdUpdate(QString stu_id,QString stu_password);
    bool studentUpdate(QString stu_id1,QString stu_id2,QString stu_name,QString stu_gender,QString class_id);

    // 插入
    bool collegeDataInsert(QString colleg_id,QString college_name);
    bool majorDataInsert(QString major_id,QString major_name,QString colleg_id);
    bool classDataInsert(QString class_id,QString class_name,QString class_grade,QString major_id,QString colleg_id);
    bool curriculumDataInsert(QString curriculum_id,QString curriculum_name,QString tea_id,QString major_id,QString colleg_id);
    bool studentDataInsert(QString stu_id,QString stu_name,QString stu_gender,QString stu_password,QString class_id);

    // 删除
    bool formCollegeDelete(QString college_id);
    bool formMajorDelete(QString major_id);
    bool formClassDelete(QString class_id);
    bool formCurriuclumDelete(QString curriuclum_id);
    bool formStudentDelete(QString stu_id);


private:
    bool initConnect();
    static SqlManager *instance;
    static QSqlDatabase database;
    static QString dbHostName;
    static QString dbDatabaseName;
    static int dbPort;
    static QString dbUserName;
    static QString dbPassword;
    QSqlQueryModel *queryModel;
    QSqlTableModel *model;
    QSqlQuery *query;

};

#endif // SQLMANAGER_H

