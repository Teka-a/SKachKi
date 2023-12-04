#include "functions.h"

Functions::Functions()
{
    Database *db = Database::getInstance();
    Curve *secp256k1 = Curve::getInstance();
}

//Authentication
QString Functions::checkPassword(QString login, QString password)
{
    QString responce = "auth&";
    QSqlQuery query;
    query.prepare("SELECT password FROM users "
                  "WHERE login = :login");
    query.bindValue(":login", login);
    query.exec();

    QSqlRecord rec = query.record();
    const int key = rec.indexOf("password");

    QString hashDB = "";
    while(query.next()){
        hashDB = query.value(key).toString();

    }
    password = password.toUtf8().toHex();
    QString hash = getHash512(password);
    //qDebug() << hash;
    //qDebug() << hashDB;
    if (hash == hashDB) {
        responce += "success";
    }
    else {
        responce += "failed";
    }
    //add status
    query.prepare("SELECT status.name, users.login FROM users "
                  "INNER JOIN status ON users.status_id = status.id "
                  "WHERE users.login = :login");
    query.bindValue(":login", login);
    query.exec();
    //qDebug() << query.exec();
    //qDebug() << query.lastError();
    QString userStatus = "";

    while(query.next()){
        userStatus = query.value(0).toString();
    }
    //qDebug() << userStatus;
    responce += "&" + userStatus;
    responce = responce.simplified();
    responce.replace( " ", "" );

    //qDebug() << "checkPass" << responce;
    return responce;
}

QString Functions::sendOpenKey(QString login)
{

    QString responce = "openKey&" + login + "&";

    QSqlQuery query;
    query.prepare("SELECT ltok FROM users "
                  "WHERE login = :login");
    query.bindValue(":login", login);
    query.exec();

    QSqlRecord rec = query.record();
    const int key = rec.indexOf("ltok");

    while(query.next()){
        //qDebug() << query.value(0);
        responce += query.value(key).toString();
        //qDebug() << responce;
    }
    //qDebug() << "openKey" << responce;
    return responce;
}

void Functions::generateShortTermKey(uint1024_t &shortTermPrivateKey)
{
    QString uuid = QUuid::createUuid().toString();
    uuid.remove(QRegularExpression("{|}"));
    QStringList partsUuid = uuid.split("-", Qt::SkipEmptyParts);
    //qDebug() << partsUuid[3];
    bool ok;
    shortTermPrivateKey = partsUuid[3].toLongLong(&ok, 16);
    //qDebug() << QString::fromStdString(shortTermPrivateKey.str());
    this->shortTermOpenKey = Curve::getInstance()->countComposition(shortTermPrivateKey);
}

QString Functions::sendShortTermOpenKey(QString login)
{
    //зачем логин?
    QString responce = "shortTermOpenKey&Server&" + login + "&";

    responce += QString::fromStdString(this->shortTermOpenKey.x.str()) + "&" + QString::fromStdString(this->shortTermOpenKey.y.str()) + "&";
    return responce;
}

Point Functions::mqv(Point longTermOpenKey, Point shortTermOpenKey)
{
    //Server's privates keys
    uint1024_t dA = 83666;
    uint1024_t kA;
    generateShortTermKey(kA);
    //Server's open keys
    //shortTerm
    Point rA = this->shortTermOpenKey;
    //qDebug() << "Server short open: " << QString::fromStdString(rA.x.str()) << " " << QString::fromStdString(rA.y.str());
    uint1024_t xA = (uint1024_t)rA.x;
    //longTerm
    Point qA = secp256k1->countComposition(83666);
    //qDebug() << "Server long open: " << QString::fromStdString(qA.x.str()) << " " << QString::fromStdString(qA.y.str());
    uint256_t aA = qA.x;
    //Client's open keys
    //shortTerm
    Point rB = shortTermOpenKey;
    //qDebug() << "Client short open: " << QString::fromStdString(rB.x.str()) << " " << QString::fromStdString(rB.y.str());
    uint1024_t xB = (uint1024_t)rB.x;
    //longTerm
    Point qB = longTermOpenKey;
    //qDebug() << "Client long open: " << QString::fromStdString(qB.x.str()) << " " << QString::fromStdString(qB.y.str());
    uint1024_t aB = (uint1024_t)qB.x;


    //qDebug() << "MQV stage 1";

    uint1024_t sA = xA*aA*dA;
    sA += kA;
    sA %= (uint1024_t)secp256k1->getN().toStdString();
    //qDebug() << "sA =" << QString::fromStdString(sA.str());


    //qDebug() << "MQV stage 2";


    Point uA = secp256k1->countComposition(aB, qB);
    uA = secp256k1->countComposition(xB, uA);
    uA = secp256k1->addPoint(rB, uA);
    //qDebug() << "uA" << QString::fromStdString(uA.x.str()) << QString::fromStdString(uA.y.str());

    //qDebug() << "MQV stage 3";
    Point W = secp256k1->countComposition(sA, uA);
    //qDebug() << "W" << QString::fromStdString(W.x.str()) << QString::fromStdString(W.y.str());
    return W;

}


//Contest info
//contestsList& id & name & date & time & status & place
//              id & name & date & time & status & place
QString Functions::sendContestsGeneralInfo()
{
    QString responce = "contestsList";
    QSqlQuery query;
    QString queryState = "SELECT c.id, c.name AS contestname, c.date, c.time, c.status, h.name AS placename FROM contests AS c "
                         "JOIN hippodrome AS h ON h.id = c.hippodrome_id "
                         "ORDER BY c.date DESC";
    query.prepare(queryState);
    query.exec();

    QSqlRecord rec = query.record();
    const int id = rec.indexOf("id");
    const int name = rec.indexOf("contestname");
    const int date = rec.indexOf("date");
    const int time = rec.indexOf("time");
    const int status = rec.indexOf("status");
    const int place = rec.indexOf("placename");


    QString contests = "";
    while(query.next()){
        contests += "&" + query.value(id).toString() +
                    "&" + query.value(name).toString() +
                    "&" + query.value(date).toString() +
                    "&" + query.value(time).toString() +
                    "&" + query.value(place).toString() +
                    "&" + query.value(status).toString();
    }
    responce += contests;
    qDebug() << "Contests" << responce;
    return responce;
}


//contestsDetailedInfo & ContId & ContName & ContDate & ContTime & ContRegStatus &
//                       HippId & HippName & HippAddr & HippDesc
//                       Horse & Jockey & time & place
QString Functions::sendContestDetailedInfo(QString contestId)
{
    QString responce = "contestsDetailedInfo&";
    //get info about contest: name, date, time, reg_status
    QSqlQuery query;
    QString queryState = "SELECT * FROM contests "
                         "WHERE id = :contestId";
    query.prepare(queryState);
    query.bindValue(":contestId", contestId);

    query.exec();
    //qDebug() << query.executedQuery();
    QSqlRecord rec = query.record();
    const int id = rec.indexOf("id");
    const int name = rec.indexOf("name");
    const int date = rec.indexOf("date");
    const int time = rec.indexOf("time");
    const int status = rec.indexOf("status");
    const int placeId = rec.indexOf("hippodrome_id");
    QString place_id = "";
    QString contest = "";
    while(query.next()){
        contest +=  query.value(id).toString() +
                    "&" + query.value(name).toString() +
                    "&" + query.value(date).toString() +
                    "&" + query.value(time).toString() +
                    "&" + query.value(status).toString() + "&";
        place_id = query.value(placeId).toString();
    }
    //qDebug() << contest;
    responce += contest;
    //qDebug() << place_id;
    //get info about hippodrome: name, address, description
    QString queryState2 = "SELECT * FROM hippodrome "
                          "WHERE id = :hippodromeId";
    query.prepare(queryState2);
    query.bindValue(":hippodromeId", place_id.toInt());
    //qDebug() << query.executedQuery();
    query.exec();
    //qDebug() << query.executedQuery();
    rec = query.record();
    const int idPlace = rec.indexOf("id");
    const int namePlace = rec.indexOf("name");
    const int address = rec.indexOf("address");
    const int description = rec.indexOf("description");

    QString place = "";
    while(query.next()){
        place +=  query.value(idPlace).toString() +
                   "&" + query.value(namePlace).toString() +
                   "&" + query.value(address).toString() +
                   "&" + query.value(description).toString();
    }
    //qDebug() << place;
    responce += place;
    //get info about participants: surname name, horse name, time, place
    queryState = "SELECT c_u_h.chip_time AS time, c_u_h.place AS place, horses.name AS horse, users.name AS name, users.surname AS surname FROM c_u_h "
                 "JOIN horses ON horses.id = c_u_h.horse_id "
                 "JOIN users  ON users.id = c_u_h.jockey_id "
                 "WHERE contest_id = :contestId"
                 "ORDER BY time DESC";
    query.prepare(queryState);
    query.bindValue(":contestId", contestId);
    //qDebug() << query.executedQuery();
    query.exec();

    rec = query.record();
    const int chipTime = rec.indexOf("time");
    const int jockeyName = rec.indexOf("name");
    const int jockeySurname = rec.indexOf("surname");
    const int horse = rec.indexOf("horse");
    const int placeGetted = rec.indexOf("place");

    QString participants = "";
    QString defaultTime = "00:00:00";
    QString defaultPlace = "-";
    int count = 0;
    while(query.next()){
        count += 1;
        if (query.value(chipTime).toString() != "")
            defaultTime = query.value(chipTime).toString();
        if (query.value(placeGetted).toString() != "0")
            defaultPlace = query.value(placeGetted).toString();

        participants += "&" + query.value(horse).toString() +
                   "&" + query.value(jockeyName).toString() + " " + query.value(jockeySurname).toString() +
                   "&" + defaultTime +
                   "&" + defaultPlace;
    }
    while (count < 7) {
        count += 1;
        participants += "&-&-&00:00:00&-";
    }
    participants += "&";
    //qDebug() << participants;
    responce += participants;
    qDebug() << responce;
    return responce;
}


QString Functions::send3FutureContests()
{
    QString responce = "contests3";
    QSqlQuery query;
    QString queryState = "SELECT c.id, c.name AS contestname, c.date, c.time, c.status, h.name AS placename FROM contests AS c "
                         "JOIN hippodrome AS h ON h.id = c.hippodrome_id "
                         "WHERE c.date > current_date";
    query.prepare(queryState);
    query.exec();

    QSqlRecord rec = query.record();
    const int id = rec.indexOf("id");
    const int name = rec.indexOf("contestname");
    const int date = rec.indexOf("date");
    const int time = rec.indexOf("time");
    const int status = rec.indexOf("status");
    const int place = rec.indexOf("placename");

    int count = 0;
    QString contests = "";
    while(query.next()){
        count += 1;
        contests += "&" + query.value(id).toString() +
                    "&" + query.value(name).toString() +
                    "&" + query.value(date).toString() +
                    "&" + query.value(time).toString() +
                    "&" + query.value(place).toString() +
                    "&" + query.value(status).toString();
    }
    if (count < 3) {
        contests += "&end";
    }
    responce += contests;
    qDebug() << "Contests" << responce;
    return responce;
}


//horsesForContest&contName& horseId & horseName & horseSex & horseAge & horseOwner
QString Functions::sendHorsesForContest(QString contestId)
{
    QString responce = "horsesForContest&" + contestId + "&";
    QSqlQuery query;

    //select h.id, h.name AS horseName, h.sex, h.age, h.available, u.name, u.surname FROM horses AS h
    //inner join users AS u ON u.id = h.owner_id
    QString queryState = "SELECT horse_id FROM c_u_h "
                         "WHERE contest_id = :contestId";
    query.prepare(queryState);
    query.bindValue(":contestId", contestId);
    query.exec();

    QSqlRecord rec = query.record();
    const int id = rec.indexOf("horse_id");
    QVector<QString> horseIdsRegistered;
    while(query.next()){
        qDebug() << query.value(id).toString();
        horseIdsRegistered.append(query.value(id).toString());
    }

    queryState = "SELECT name FROM contests "
                 "WHERE id = :contestId";
    query.prepare(queryState);
    query.bindValue(":contestId", contestId);
    query.exec();
    rec = query.record();
    const int name = rec.indexOf("name");
    while(query.next()){
        responce += query.value(name).toString();
    }
    queryState = "SELECT h.id, h.name AS horseName, h.sex, h.birth_date AS age, h.available, u.name, u.surname FROM horses AS h "
                 "INNER JOIN users AS u ON u.id = h.owner_id";

    query.prepare(queryState);
    query.exec();

    rec = query.record();
    const int horseId = rec.indexOf("id");
    const int horseName = rec.indexOf("horseName");
    const int horseSex = rec.indexOf("sex");
    const int horseAge = rec.indexOf("age");
    const int horseAvailable = rec.indexOf("available");
    const int ownerName = rec.indexOf("name");
    const int ownerSurname = rec.indexOf("surname");
    QString horses = "";
    QDate current = QDate::currentDate();
    while (query.next()) {
        //qDebug() << query.value(horseAvailable).toString();
        if (query.value(horseAvailable).toString() != "true")
            continue;
        QString id = query.value(horseId).toString();
        int i = 0;
        for (i = 0; i < horseIdsRegistered.size(); ++i) {
            //qDebug() << id << horseIdsRegistered[i] << (id == horseIdsRegistered[i]) << i;
            if (id == horseIdsRegistered[i])
                break;
        }
        if (i == horseIdsRegistered.size()) {
            int age = current.year() - QDate::fromString(query.value(horseAge).toString(), "yyyy-MM-dd").year();
            qDebug() << "Age" << QString::number(age);
            horses += "&" + id +
                      "&" + query.value(horseName).toString() +
                      "&" + query.value(horseSex).toString() +
                      "&" + QString::number(age) +
                      "&" + query.value(ownerName).toString() + " " + query.value(ownerSurname).toString();
        }

    }

    responce += horses;
    qDebug() << responce;
    return responce;
}
//add horse:
//INSERT INTO horses (name, sex, age, owner_id, available)
//VALUES ('Дженифер', 'Кобыла', 4, 5, true);

//altr horse name:
//UPDATE horses
//SET name = 'Звездочка',
//WHERE id = 1;

QString Functions::regForContest(QString contestId, QString horseId, QString jockeyLogin)
{
    QString responce = "registration&";
    QSqlQuery query;
    QString queryState = "SELECT c_u_h.horse_id AS horseid, users.login AS login FROM c_u_h "
                         "JOIN users ON c_u_h.jockey_id = users.id "
                         "WHERE contest_id = :contestId";
    query.prepare(queryState);
    query.bindValue(":contestId", contestId);
    query.exec();
    QSqlRecord rec = query.record();
    const int horse = rec.indexOf("horseid");
    const int login = rec.indexOf("login");
    int count = 0;
    while(query.next()){
        count += 1;
        if (query.value(horse).toString() == horseId) {
            qDebug() <<"horse in use";
            responce += "error&horseAlreadyInUse";
            return responce;
        }
        if (query.value(login).toString() == jockeyLogin) {
            qDebug() << "jockey In use";
            responce += "error&jockeyAlreadyInUse";
            return responce;
        }
    }
    if (count >= 6) {
        responce += "error&noPlace";
        return responce;
    }
    queryState = "SELECT id FROM users "
                 "WHERE login = :login";
    query.prepare(queryState);
    query.bindValue(":login", jockeyLogin);
    query.exec();
    rec = query.record();
    const int jockId = rec.indexOf("id");
    QString jockeyId = "";
    while(query.next()){
        jockeyId += query.value(jockId).toString();
    }
    qDebug() << jockeyId;
    qDebug() << contestId << horseId << jockeyId;
    queryState = "INSERT INTO c_u_h (contest_id, horse_id, jockey_id) "
                 "VALUES (:cid, :hid, :jid)";
    query.prepare(queryState);
    query.bindValue(":cid", contestId);
    query.bindValue(":hid", horseId);
    query.bindValue(":jid", jockeyId);

    query.exec();
    qDebug() << "insert" << query.executedQuery();
    queryState = "SELECT * FROM c_u_h "
                 "WHERE contest_id = :cid AND horse_id = :hid AND jockey_id = :jid";
    query.prepare(queryState);
    query.bindValue(":cid", contestId);
    query.bindValue(":hid", horseId);
    query.bindValue(":jid", jockeyId);
    qDebug() << query.executedQuery();
    query.exec();
    int c = 0;
    while(query.next()){
        qDebug() << query.value(0).toString();
        c += 1;
    }
    if (c == 1) {
        responce += "success";
    }
    else {
        responce += "failed";
    }
    return responce;
    //queryState
}

QString Functions::regNewUser(QString name, QString surname, QString statusId, QString phone, QString address, QString birthDate, QString login, QString password, QString ltok)
{
    QString responce = "userRegistration&";

    password = password.toUtf8().toHex();
    password = getHash512(password);
    qDebug() << "Hash" << password;
    QSqlQuery query;
    QString queryState = "INSERT INTO users (name, surname, phone, login, password, status_id, ltok, address, birth_date) "
                         "VALUES (:name, :surname, :phone, :login, :password, :status_id, :ltok, :address, :birth_date)";
    query.prepare(queryState);
    query.bindValue(":name", name);
    query.bindValue(":surname", surname);
    query.bindValue(":phone", phone);
    query.bindValue(":login", login);
    query.bindValue(":password", password);
    query.bindValue(":status_id", statusId);
    query.bindValue(":ltok", ltok);
    query.bindValue(":address", address);
    query.bindValue(":birth_date", birthDate);
    query.exec();

    qDebug() << "insert" << query.executedQuery();
    queryState = "SELECT id FROM users "
                 "WHERE login = :login";
    query.prepare(queryState);
    query.bindValue(":login", login);
    qDebug() << query.executedQuery();
    query.exec();
    int c = 0;
    while(query.next()){
        c += 1;
    }
    if (c > 0) {
        responce += "success";
    }
    else {
        responce += "failed";
    }
    return responce;
}


QString Functions::regNewContest(QString name, QString date, QString time, QString placeId, QString status, QString info)
{
    QString responce = "contestRegistration&";
    QSqlQuery query;
    QString queryState = "INSERT INTO contests (name, date, time, hippodrome_id, status, info) "
                         "VALUES (:name, :date, :time, :place, :status, :info)";
    query.prepare(queryState);
    query.bindValue(":name", name);
    query.bindValue(":date", date);
    query.bindValue(":time", time);
    query.bindValue(":place", placeId);
    query.bindValue(":status", status);
    query.bindValue(":info", info);
    query.exec();

    qDebug() << "insert" << query.executedQuery();
    queryState = "SELECT id FROM contests "
                 "WHERE name = :name";
    query.prepare(queryState);
    query.bindValue(":name", name);
    qDebug() << query.executedQuery();
    query.exec();
    int c = 0;
    while(query.next()){
        c += 1;
    }
    if (c > 0) {
        responce += "success";
    }
    else {
        responce += "failed";
    }
    return responce;
}


QString Functions::sendPlaces()
{
    QString responce = "places&";
    QSqlQuery query;
    QString queryState = "SELECT * FROM hippodrome";
    query.prepare(queryState);
    query.exec();

    QSqlRecord rec = query.record();
    const int id = rec.indexOf("id");
    const int name = rec.indexOf("name");
    const int address = rec.indexOf("address");
    const int description = rec.indexOf("description");


    QString places = "";
    while(query.next()){
        places += "&" + query.value(id).toString() +
                  "&" + query.value(name).toString() +
                  "&" + query.value(address).toString() +
                  "&" + query.value(description).toString();
    }
    responce += places;
    qDebug() << "Places" << responce;

    return responce;
}

QString Functions::regNewPlace(QString name, QString address, QString description)
{
    QString responce = "placeRegistration&";
    QSqlQuery query;
    QString queryState = "INSERT INTO hippodrome (name, address, description) "
                         "VALUES (:name, :address, :description)";
    query.prepare(queryState);
    query.bindValue(":name", name);
    query.bindValue(":address", address);
    query.bindValue(":description", description);
    query.exec();

    qDebug() << "insert" << query.executedQuery();
    queryState = "SELECT id FROM hippodrome "
                 "WHERE name = :name";
    query.prepare(queryState);
    query.bindValue(":name", name);
    qDebug() << query.executedQuery();
    query.exec();
    int c = 0;
    while(query.next()){
        c += 1;
    }
    if (c > 0) {
        responce += "success";
    }
    else {
        responce += "failed";
    }
    return responce;
}


QString Functions::parse(QString dataFromClient, QMap<QTcpSocket*, QVector<QString>> &sockets, QTcpSocket* socket)
{
    qDebug() << "data client" << dataFromClient;

    QStringList list = dataFromClient.split("&", Qt::SkipEmptyParts);
    //requestOpenKey&userLogin&
    if(list[0] == "requestOpenKey"){
        //requestOpenKey&user1
        return sendOpenKey(list[1]);
    }
    //shortTermOpenKey&userLogin&Server&stok.x&stok.y&
    else if(list[0] == "shortTermOpenKey"){
        if(list[2] == "Server"){
            QStringList keyInfo = sendOpenKey(list[1]).split("&", Qt::SkipEmptyParts);
            Point longTermOpenKey;
            Point shortTermOpenKey;
            longTermOpenKey.x = (uint256_t)keyInfo[2].toStdString();
            longTermOpenKey.y = (uint256_t)keyInfo[3].toStdString();

            shortTermOpenKey.x = (uint256_t)list[3].toStdString();
            shortTermOpenKey.y = (uint256_t)list[4].toStdString();
            //qDebug() << "Before: " << QString::fromStdString(shortTermOpenKey.x.str()) << " " << QString::fromStdString(shortTermOpenKey.y.str());
            //qDebug() << "Before: " << QString::fromStdString(longTermOpenKey.x.str()) << " " << QString::fromStdString(longTermOpenKey.y.str());
            Point W = mqv(longTermOpenKey, shortTermOpenKey);
            qDebug() << "bef" << sockets[socket] << socket;
            QString x = "";
            QString y = "";
            for (int i = 0; i < 64; ++i) {
                x += QString::fromStdString(W.x.str())[i];
                y += QString::fromStdString(W.y.str())[i];
            }
            sockets[socket] = {x, y, list[1]};
            qDebug() << "aft" << sockets[socket] << sockets.size();
            //qDebug() << "After: " << QString::fromStdString(shortTermOpenKey.x.str()) << " " << QString::fromStdString(shortTermOpenKey.y.str());
            //qDebug() << "Before: " << QString::fromStdString(longTermOpenKey.x.str()) << " " << QString::fromStdString(longTermOpenKey.y.str());
            return sendShortTermOpenKey(list[1]);
        }
    }
    //auth&userLogin&password&
    else if (list[0] == "auth") {
        sockets[socket][2] = list[2];
        return checkPassword(list[1], list[2]);
    }
    else if (list[0] == "requestContest") {
        if (list[1] == "generalInfo") {
            return sendContestsGeneralInfo();
        }
        else if (list[1] == "mainPage") {
            return send3FutureContests();
        }
        else {
            return sendContestDetailedInfo(list[1]);
        }
    }
    else if (list[0] == "requestHorsesForContest") {
        return sendHorsesForContest(list[1]);
    }
    else if (list[0] == "regForContest") {
        return regForContest(list[1], list[2], list[3]);
    }
    else if (list[0] == "regNewUser") {
        return regNewUser(list[1], list[2], list[3],
                          list[4], list[5], list[6],
                          list[7], list[8], list[9] + "&" + list[10]);
    }
    else if (list[0] == "regNewContest") {
        return regNewContest(list[1], list[2], list[3], list[4], list[5], list[6]);
    }
    else if (list[0] == "regNewPlace") {
        return regNewPlace(list[1], list[2], list[3]);
    }
    else if (list[0] == "requestPlaces") {
        return sendPlaces();
    }

    return "get it!";

}
