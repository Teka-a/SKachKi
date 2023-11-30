#ifndef COMMUNICATION_H
#define COMMUNICATION_H


#include <QString>
#include "client.h"

void requestOpenKey(QString userLogin);
void sendShortTermOpenKey(Point shortTermOpenKey, QString to = "Server");
void sendCredentials();
void requestGeneralInfoContests();
void requestDetailedInfoContest(QString id);
void request3Contests();
void requestHorsesForContest(QString id);
void regForContest(QString contestId, QString horseId, QString jockeyLogin);
#endif // COMMUNICATION_H
