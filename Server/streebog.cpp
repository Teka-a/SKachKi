#include "streebog.h"

void X(const bytesVector &vec1, const bytesVector &vec2, bytesVector &res)
{
    for (int i = 0; i < 64; ++i){
        res[i] = vec1[i] ^ vec2[i];
    }
}

void addInRingOFResidues(const bytesVector &vec1, const bytesVector &vec2, bytesVector &result)
{
    int internal = 0;
    for (int i = 0; i < 64; i++) {
        internal = vec1[i] + vec2[i] + (internal >> 8);
        result[i] = internal & 0xFF;
    }
}

void S(bytesVector &vec)
{
    for (int i = 0; i < 64; ++i){
        vec[i] = pi[vec[i]];
    }
}

void P(bytesVector &vec)
{
    bytesVector temp (64, 0x00);
    for (int i = 0; i < 64; ++i){
        temp[i] = vec[Tau[i]];
    }
    vec = temp;
}

void L(bytesVector &vec)
{
    qDebug() << "In L func";
    QString hexStr = convertBytesToHexStr(vec);
    QString binStr = convertHexStrToBinStr(hexStr);
    QString b = "1010111101010010000011";
    QString binResult = "";
    qDebug() << convertBinStrToHexStr(b);
    b = "aa";
    qDebug() << convertHexStrToBinStr(b);
    for (int i = 7; i > 0; --i) {
        unsigned long long temp = 0;
        for (int j = 63; j > 0; --j) {
            if (binStr[i*j] == "1"){
                if (temp != 0)
                    temp ^= A[j];
                else
                    temp = A[j];
            }
        }
        qDebug() << i << QString("%1").arg((long long)temp, 2, 16, QLatin1Char( '0' ));;
    }



}

QString getHash512(const QString &text)
{
    bytesVector h (64, 0x00);
    bytesVector N (64, 0x00);
    bytesVector res (64, 0x00);
    X(h, N, res);
    S(res);
    qDebug() << "S: " << convertBytesToHexStr(res);
    P(res);
    qDebug() << "P: " << convertBytesToHexStr(res);
    L(res);
    qDebug() << "L: " << convertBytesToHexStr(res);
    return text;
}
