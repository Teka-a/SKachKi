#include "application.h"
#include "application.h"

Application::Application()
{
    Curve *secp256k1 = Curve::getInstance();
    connect(Client::getInstance(), &Client::receivedMyOpenKey, this, &Application::checkSecretKey);
    connect(Client::getInstance(), &Client::receivedServerShortTermOpenKey, this, &Application::shareSecret);
    connect(Client::getInstance(), &Client::receivedAuthResult, this, &Application::authenticate);
    //qDebug() << QCoreApplication::instance();
}

Application::~Application()
{

}

void Application::formString30(std::string &option){
    while (option.length() < 31) {
        option += " ";
    }
}

void Application::authentication()
{
    std::string login = "";
    std::string password = "";
    std::string privateKey = "";

    QTextStream(stdout) << "Login:  ";
    std::getline(std::cin, login);
    Client::getInstance()->setLogin(QString::fromStdString(login));

    char *pass;
    pass = getpass("Password: ");
    int size_arr = sizeof(pass) / sizeof(char);
    for (int x = 0; x < size_arr; x++) {
        password += pass[x];
    }
    Client::getInstance()->setPassword(QString::fromStdString(password));
    pass = getpass("Private Key: ");
    size_arr = sizeof(pass) / sizeof(char);
    for (int x = 0; x < size_arr; x++) {
        privateKey += pass[x];
    }
    Client::getInstance()->setPrivateKey((uint1024_t)privateKey);
    QTextStream(stdout) << "Authentication";
    requestOpenKey(QString::fromStdString(login));

}

void Application::checkSecretKey(Point openKey)
{
    QTextStream(stdout) << ".";
    uint1024_t privateKey = Client::getInstance()->getPrivateKey();

    Point calculatedOpenKey = secp256k1->countComposition(privateKey);

    if(openKey.x != calculatedOpenKey.x || openKey.y != calculatedOpenKey.y){
        QTextStream(stdout) << "Authentication failed! Try again!" << Qt::endl;
        Client::getInstance()->resetPersonalData();
        //std::cout << "Wrong parameter! You'll be disconnected!";
        //Client::getInstance()->disconnect();
    }
    else{
        uint1024_t shortTermPrivateKey = generateShortTermPrivateKey();
        Client::getInstance()->setMyServerShortTermPrivateKey(shortTermPrivateKey);
        Point shortTermOpenKey = Curve::getInstance()->countComposition(shortTermPrivateKey);
        sendShortTermOpenKey(shortTermOpenKey);
    }


}

void Application::shareSecret(Point longTermOpenKey, Point shortTermOpenKey)
{
    QTextStream(stdout) << ".";
    uint1024_t dA = Client::getInstance()->getPrivateKey();
    uint1024_t kA = Client::getInstance()->getMyServerShortTermPrivateKey();
    Point qA = Curve::getInstance()->countComposition(dA);
    Point rA = Curve::getInstance()->countComposition(kA);
    Point W = MQV("Server", dA, kA, qA, rA, longTermOpenKey, shortTermOpenKey);
    //QString keyStr = QString::fromStdString(keyPoint.x.str()) + QString::fromStdString(keyPoint.y.str());
    //qDebug() << "Key str" << keyStr << " length: " << keyStr.length();
    //qDebug() << keyStr.toUtf8().toHex();
    QString x = "";
    QString y = "";
    for (int i = 0; i < 64; ++i) {
        x += QString::fromStdString(W.x.str())[i];
        y += QString::fromStdString(W.y.str())[i];
    }
    Client::getInstance()->setServerKey(x);
    Client::getInstance()->setServerIV(y);
    sendCredentials();
}

void Application::authenticate(QString status, QString userStatus)
{
    QTextStream(stdout) << "." << Qt::endl;
    if (status == "success"){
        Client::getInstance()->setUserStatus(userStatus);
        QTextStream(stdout) << "Welcome " << Client::getInstance()->getLogin() << "!" << Qt::endl;
    } else if (status == "failed"){
        QTextStream(stdout) << "Authentication failed! Try again!" << Qt::endl;
        Client::getInstance()->resetPersonalData();
    }
    getUserOption();
}


void Application::showHelp()
{
    QTextStream(stdout) << "GENERAL hhhhh" << Qt::endl;
    std::string option = "";

    option = "\thelp";
    formString30(option);
    QTextStream(stdout) << QString::fromStdString(option);
    QTextStream(stdout) << "Show this message" << Qt::endl;

    option = "\tauth";
    formString30(option);
    QTextStream(stdout) << QString::fromStdString(option);
    QTextStream(stdout) << "Log in to your account" << Qt::endl;

    option = "\tstatus";
    formString30(option);
    QTextStream(stdout) << QString::fromStdString(option);
    QTextStream(stdout) << "Check account and connection information" << Qt::endl;

    QTextStream(stdout) << "SHOW" << Qt::endl;
    option = "\tshow contest";
    formString30(option);
    QTextStream(stdout) << QString::fromStdString(option);
    QTextStream(stdout) << "List all (or get info about particular one) available and passed contests" << Qt::endl;

    option = "\tshow horse";
    formString30(option);
    QTextStream(stdout) << QString::fromStdString(option);
    QTextStream(stdout) << "List all (or get info about particular one) horses" << Qt::endl;

    option = "\tshow jockeys";
    formString30(option);
    QTextStream(stdout) << QString::fromStdString(option);
    QTextStream(stdout) << "List all (or get info about particular one) jockeys" << Qt::endl;

    option = "\tshow hippodromes";
    formString30(option);
    QTextStream(stdout) << QString::fromStdString(option);
    QTextStream(stdout) << "List all (or get info about particular one) hippodromes" << Qt::endl;

    QString  userStatus = Client::getInstance()->getUserStatus();
    if (userStatus != "anonymous") {
        QTextStream(stdout) << "REGISTRATION" << Qt::endl;
        if (userStatus == "admin") {

            option = "\treg user";
            formString30(option);
            QTextStream(stdout) << QString::fromStdString(option);
            QTextStream(stdout) << "Register new user" << Qt::endl;

            option = "\treg hippodrome";
            formString30(option);
            QTextStream(stdout) << QString::fromStdString(option);
            QTextStream(stdout) << "Register new hippodrome" << Qt::endl;

            option = "\treg contest";
            formString30(option);
            QTextStream(stdout) << QString::fromStdString(option);
            QTextStream(stdout) << "Register new contest" << Qt::endl;

            option = "\treg results";
            formString30(option);
            QTextStream(stdout) << QString::fromStdString(option);
            QTextStream(stdout) << "Register results for passed contest" << Qt::endl;

        }
        else if (userStatus == "owner") {

            option = "\treg horse";
            formString30(option);
            QTextStream(stdout) << QString::fromStdString(option);
            QTextStream(stdout) << "Register your new horse" << Qt::endl;
        }
        else if (userStatus == "jockey") {
            option = "\tregister";
            formString30(option);
            QTextStream(stdout) << QString::fromStdString(option);
            QTextStream(stdout) << "Register for the contest" << Qt::endl;
        }
    }
    QTextStream(stdout) << "To exit press Ctrl+C" << Qt::endl;
    getUserOption();
}

void Application::showStatusInfo()
{
    //"\033[31mbold red text\033[0m\n"
    QTextStream(stdout) << "\033[1 Status\033[0m\n" << Qt::endl;
    getUserOption();
}


void Application::getUserOption()
{
    QTextStream(stdout) << "SkachKi> ";
    std::string input = "";
    getline(std::cin, input);
    if (input == "help") {
        showHelp();
    }
    else if (input == "auth") {
        authentication();
    }
    else if (input == "encrypt") {
        std::string plaintext = "1122334455667700ffeeddccbbaa998800112233445566778899aabbcceeff0a112233445566778899aabbcceeff0a002233445566778899aabbcceeff0a0011";
        std::string key = "8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef";
        std::string IV =  "1234567890abcef0a1b2c3d4e5f0011223344556677889901213141516171819";


        QTextStream(stdout) << "Enter string to encrypt: ";
        getline(std::cin, plaintext);
        //std::cout << "Enter key: ";
        //getline(std::cin, key);
        //std::cout << "Enter IV: ";
        //getline(std::cin, IV);
        QString m = encryptKuznechikOFB(QString::fromStdString(plaintext).toUtf8().toHex(), QString::fromStdString(key), QString::fromStdString(IV));
        QString p = decryptKuznechikOFB(m, QString::fromStdString(key), QString::fromStdString(IV));
        QByteArray hex = QByteArray::fromHex(p.toUtf8());
        qDebug() << hex.data();
        QString str = QString::fromUtf8(hex);

        getUserOption();
    }
    else if (input == "status") {
        showStatusInfo();
    }
    else {
        getUserOption();
    }
}
